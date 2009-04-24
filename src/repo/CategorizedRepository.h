/**
 * @file CategorizedRepository.h
 * @author Gerd Zschaler
 * @date Aug 15, 2008
 * The repo::CategorizedRepository class.
 *
 * @todo Think of more clever ways to implement consistent iterators. It does not
 * 		 seem convenient to have two types of iterators that naively do the same thing.
 * 	     We need an iterator type that
 * 		 	- is not invalidated by changing an item's category
 * 		 	- @em and iterates over items in one category efficiently.
 *			.
 * 	     Is this possible with only one iterator type?
 *
 */
#ifndef CATEGORIZEDREPOSITORY_H_
#define CATEGORIZEDREPOSITORY_H_

#include "types.h"
#include <utility>	// for std::pair
#include <iterator>
#include <exception>
#include <string>
#include <cassert>
#include <memory.h>

namespace repo
{

class RepoAllocException: public std::exception
{
public:
	const char* what() const throw()
	{
		return "Could not resize repository!";
	}
};

/**
 * A categorized repository of items.
 *
 * A convenient way of organizing access to objects
 * that can be grouped into a finite number of categories/classes.
 * @c CategorizedRepository features efficient access to objects and can handle some
 * additional tasks.
 *
 * @author Gerd Zschaler <zschaler@pks.mpg.de>, original concept by Thilo Gross <gross@physics.org>
 *
 * @note @p T should be a pointer type (for now).
 *
 * @see Class @c CRepository by Thilo Gross <gross@physics.org>.
 *
 * @todo Better remove the index operators @c () and @c [], and rename @c item(address_t) and @c item(id_t) to
 * something clearer to prevent confusion and ambiguities.

 * @todo Make item ownership consistent. Items should be created and destroyed
 * 		 in the same context (class). What if @c T is not a pointer type?
 * 		 Should CategorizedRepository generally be templated on a pointer
 * 	   	 type (i.e. @c T*)?
 * 			- Remove deleteAll(), then the user is responsible for destroying items, and
 * 			  @c T does not have to be a pointer type.
 * 			- Still, if @c T is not a pointer type, we need to have clear item ownership (value
 * 			  semantics?) and be careful when inserting items (at the moment we make a copy).
 * 			  Also, with pointers it is easy to just assign zero to a removed item in the items
 * 			  array before moving it into the trash category. How should a value @c T be treated?
 * 			- For value semantics (taking copies of inserted elements), we are likely to need some
 * 			  proper allocator class.
 * 			.
 */
template<class T, unsigned int enlarge_factor = 10, unsigned int max_size =
		1000000000>
class CategorizedRepository
{
public:
	// member types
	/**
	 * Index iterator.
	 *
	 * This should be used for the traversal of the repository. If valid, it will
	 * always dereference to the unique ID of an item actually stored in the repository.
	 * Note that inserting into the repository invalidates all iterators.
	 */
	class IndexIterator: public std::iterator<std::input_iterator_tag, id_t>
	{
	public:
		/**
		 * Default constructor needed for STL compliance. This creates a singular
		 * (invalid) iterator which does not belong to any repository.
		 */
		IndexIterator();
		/**
		 * Basic constructor. This creates an iterator pointing to the first
		 * item in the repository @p repo.
		 * @param repo Repository the iterator belongs to.
		 */
		IndexIterator(const CategorizedRepository& repo);
		/**
		 * Position constructor. This creates an iterator pointing to the @p n'th
		 * item in the repository @p repo.
		 * @param repo Repository the iterator belongs to.
		 * @param n Number of item.
		 */
		IndexIterator(const CategorizedRepository& repo, id_t n);
		/**
		 * Copy constructor.
		 * @param it Iterator to copy from.
		 */
		IndexIterator(const IndexIterator& it);
		/**
		 * Destructor. Does nothing.
		 */
		~IndexIterator();

		/**
		 * Assignment operator.
		 * @param it Iterator to assign from.
		 * @return Reference to self.
		 */
		IndexIterator& operator=(const IndexIterator& it);
		/**
		 * Equality operator.
		 * @param it Iterator to compare with.
		 * @return True if equal.
		 */
		bool operator==(const IndexIterator& it);
		/**
		 * Unequality operator.
		 * @param it Iterator to compare with.
		 * @return True if unequal.
		 */
		bool operator!=(const IndexIterator& it);
		/**
		 * Prefix increment operator.
		 * @return Reference to (incremented) self.
		 */
		IndexIterator& operator++();
		/**
		 * Postfix increment operator.
		 * @return Copy of self before increment.
		 */
		IndexIterator operator++(int);
		/**
		 * Dereference operator.
		 * @return Reference to item.
		 */
		id_t operator*();
	private:
		const CategorizedRepository* rep_; ///< repository the iterator belongs to
		id_t cur_; ///< current iterator position
	}; // class CategorizedRepository::IndexIterator

	/**
	 * Category iterator.
	 *
	 * Iterates over all items in one category. If valid, it will
	 * always dereference to the unique ID of an item actually stored in the repository.
	 * Note that in addition to insertion into the repository, <b>changing an item's category
	 * invalidates this iterator</b>, too.
	 */
	class CategoryIterator: public std::iterator<std::input_iterator_tag, id_t>
	{
	public:
		/**
		 * Default constructor needed for STL compliance. This creates a singular
		 * (invalid) iterator which does not belong to any repository.
		 */
		CategoryIterator();
		/**
		 * Basic constructor. This creates an iterator pointing to the first
		 * item in category @p cat in the repository @p repo.
		 * @param repo Repository the iterator belongs to.
		 * @param cat Category to iterate over.
		 */
		CategoryIterator(const CategorizedRepository& repo, category_t cat);
		/**
		 * Position constructor. This creates an iterator pointing to the @p n'th
		 * item in category @p cat in the repository @p repo.
		 * @param repo Repository the iterator belongs to.
		 * @param cat Category to iterate over.
		 * @param n Number of item.
		 */
		CategoryIterator(const CategorizedRepository& repo, category_t cat,
				address_t n);
		/**
		 * Copy constructor.
		 * @param it Iterator to copy from.
		 */
		CategoryIterator(const CategoryIterator& it);
		/**
		 * Destructor. Does nothing.
		 */
		~CategoryIterator();

		/**
		 * Assignment operator.
		 * @param it Iterator to assign from.
		 * @return Reference to self.
		 */
		CategoryIterator& operator=(const CategoryIterator& it);
		/**
		 * Equality operator.
		 * @param it Iterator to compare with.
		 * @return True if equal.
		 */
		bool operator==(const CategoryIterator& it);
		/**
		 * Unequality operator.
		 * @param it Iterator to compare with.
		 * @return True if unequal.
		 */
		bool operator!=(const CategoryIterator& it);
		/**
		 * Prefix increment operator.
		 * @return Reference to (incremented) self.
		 */
		CategoryIterator& operator++();
		/**
		 * Postfix increment operator.
		 * @return Copy of self before increment.
		 */
		CategoryIterator operator++(int);
		/**
		 * Dereference operator.
		 * @return Reference to item.
		 */
		id_t operator*();
	private:
		const CategorizedRepository* rep_; ///< Repository the iterator belongs to.
		category_t category_; ///< Category the iterator traverses.
		address_t cur_; ///< Current iterator position (index in ids_ array).
	}; // class CategorizedRepository::CategoryIterator

	typedef T value_type; ///< Type of stored items.
	typedef std::pair<IndexIterator, IndexIterator> IndexIteratorRange; ///< IndexIterator range type.
	typedef std::pair<CategoryIterator, CategoryIterator> CategoryIteratorRange; ///< CategoryIterator range type.

public:
	// member functions
	/**
	 * Copy constructor
	 * @param r	CategorizedRepository to copy from
	 */
	CategorizedRepository(const CategorizedRepository& r);
	/**
	 * Constructor for repository for @p cat categories.
	 * @param cat Number of categories to store. This cannot be changed later on.
	 */
	CategorizedRepository(category_t cat);
	/**
	 * Constructor for repository for @p cat categories.
	 * Space will be allocated for @p N items.
	 * @param cat Number of categories.
	 * @param N Number of items.
	 */
	CategorizedRepository(category_t cat, address_t N);
	/**
	 * Destructor
	 */
	~CategorizedRepository();

	/**
	 * Return number of items stored.
	 * @return Number of items stored.
	 */
	id_size_t count() const;
	/**
	 * Convenience function, same as count().
	 * @return Number of items stored.
	 */
	id_size_t size() const;
	/**
	 * Return number of items in category @p cat.
	 * @param cat Category to count items in.
	 * @return Number of items in category @p cat
	 */
	id_size_t count(category_t cat) const;

	/**
	 * Return current maximum number of storable items, i.e. reserved memory.
	 * @return Current capacity.
	 */
	address_t capacity() const;

	/**
	 * Get the number of categories in the repository.
	 * @return Number of categories.
	 */
	category_t numberOfCategories() const;

	/**
	 * Return category of item with number @p n.
	 * @param n Number of item.
	 * @return	Category ID of item.
	 */
	category_t category(address_t n) const;
	/**
	 * Return category of item with unique ID @p id.
	 * @param id Unique ID of item.
	 * @return Category ID of item.
	 */
	category_t category(id_t id) const;

	/**
	 * Set category of item with @p id.
	 * @param id Unique ID of item.
	 * @param cat Category to put item into.
	 */
	void setCategory(id_t id, category_t cat);
	/**
	 * Set category of item with number @p n.
	 * @param n Number of item.
	 * @param cat Category to put item into.
	 */
	void setCategory(address_t n, category_t cat);
	/**
	 * Set category of @p n 'th item in category @p oldCat to @p newCat
	 * @param oldCat Category the item is in.
	 * @param n Number of item in category @p oldCat
	 * @param newCat New category for item.
	 */
	void setCategory(category_t oldCat, address_t n, category_t newCat);

	/**
	 * Return item with @p id.
	 * @param id Unique ID of item
	 * @return Reference to item
	 */
	T& item(id_t id) const;
	/**
	 * Return item with number @p n.
	 * @param n Number of item
	 * @return Reference to item
	 */
	T& item(address_t n) const;
	/**
	 * Return @p n 'th item in category @p cat
	 * @param cat Category of item
	 * @param n Number of item in category
	 * @return Reference to item
	 */
	T& item(category_t cat, address_t n) const;

	/**
	 * Return ID of @p n 'th item
	 * @param n Number of item
	 * @return Unique ID of item
	 */
	id_t id(address_t n) const;
	/**
	 * Return ID of @p n 'th item in category @p cat
	 * @param cat Category of item
	 * @param n Number of item in category @p cat
	 * @return Unique ID of item
	 */
	id_t id(category_t cat, address_t n) const;

	/**
	 * Insert a copy of @p itm into category @p cat
	 * @param itm Item to insert into the repository.
	 * @param cat Category to insert the item into.
	 * @return Unique ID of inserted item.
	 */
	id_t insert(T itm, category_t cat);
	/**
	 * Insert a copy of @p itm into the repository.
	 * @param itm Item to insert into the repository.
	 * @return Unique ID of inserted item.
	 */
	id_t insert(T itm);

	/**
	 * Remove item from repository. The item is not destroyed but returned.
	 * @param id Unique ID of item to remove
	 * @return Removed item
	 */
	T remove(id_t id);
	/**
	 * Remove @p n 'th item from repository.
	 * The item is not destroyed but returned.
	 * @param n Number of item to remove
	 * @return Removed item.
	 */
	T remove(address_t n);
	/**
	 * Remove @p n 'th item in category @p cat from repository.
	 * The item is not destroyed but returned.
	 * @param cat Category of item
	 * @param n Number of item in category @p cat
	 * @return Removed item
	 */
	T remove(category_t cat, address_t n);

	/**
	 * Remove all items from repository. The items are not destroyed.
	 * @todo What if items are pointers? Nothing. The user must take care to
	 * properly destroy objects pointed to. Compare with STL containers (value semantics).
	 */
	void removeAll();
	/**
	 * Remove and delete all items from repository. Delete is called for each item.
	 * @deprecated To be removed in future versions. CategorizedRepository does not take item
	 * ownership.
	 */
	void deleteAll();

	/**
	 * Get item with unique ID @p id
	 * @param id Unique id of item
	 * @return Reference to item
	 */
	T& operator[](id_t id) const;
	/**
	 * Get @p n 'th item in repository
	 * @param n Number of item
	 * @return Reference to item
	 */
	T& operator()(address_t n) const;
	/**
	 * Get @p n 'th item in category @p cat
	 * @param cat Category of item
	 * @param n Number of item in category @p cat
	 * @return Reference to item
	 */
	T& operator()(category_t cat, address_t n) const;

	/**
	 * Insert a copy of item @p itm into the repository.
	 * @param itm Item to insert into the repository
	 * @return Unique ID of inserted item.
	 */
	id_t operator<<(T itm);

	/**
	 * Return an iterator referring to the first item stored in the repository.
	 * Use this in conjunction with end() to iterate
	 * over all items. These iterators will always dereference to valid item IDs.
	 * @return Iterator pointing to first item ID.
	 */
	const IndexIterator begin() const;
	/**
	 * Return an iterator referring to the past-the-end element in the repository.
	 * @return Past-the-end iterator
	 */
	const IndexIterator end() const;
	/**
	 * Return iterator range of the unique IDs of all items stored in the repository.
	 * @return @c std::pair of IndexIterators, where the first points to the
	 * unique ID of the first item stored and the second to the past-the-end element
	 * in the repository.
	 */
	IndexIteratorRange ids() const;
	/**
	 * Return iterator range of the unique IDs of all items in category @p cat stored in
	 * the repository.
	 * @param cat Category to iterate over.
	 * @return @c std::pair of CategoryIterators, where the first points to the
	 * unique ID of the first item stored and the second to the past-the-end element
	 * in the repository.
	 */
	CategoryIteratorRange ids(category_t cat) const;

private:
	void allocate(); ///< allocate memory
	void clear(); ///< clear repository and deallocate memory
	bool enlarge(); ///< enlarge the storage space
	void increaseCat(address_t n, category_t cat); ///< increase category of entry
	void decreaseCat(address_t n, category_t cat); ///< decrease category of entry

	const category_t C_; ///< number of categories
	address_t N_; ///< current max number of items
	address_t nStored_; ///< total number of items stored
	address_t* count_; ///< number of items per class;
	address_t* offset_; ///< start index of a class
	T* items_; ///< array of items @note Why not use std::vector and do not mess with memory management ourselves?
	address_t* nums_; ///< number of an item according to id
	id_t* ids_; ///< id of an item according to number

}; // class CategorizedRepository

// --------------- construction/destruction

template<class T, unsigned int enlarge_factor, unsigned int max_size>
CategorizedRepository<T, enlarge_factor, max_size>::CategorizedRepository(
		const CategorizedRepository& r) :
	C_(r.C_), N_(r.N_), nStored_(r.nStored_)
{
	allocate();
	memcpy(count_, r.count_, (C_ + 1) * sizeof(address_t));
	memcpy(offset_, r.offset_, (C_ + 1) * sizeof(address_t));
	memcpy(items_, r.items_, N_ * sizeof(T));
	memcpy(ids_, r.ids_, N_ * sizeof(T));
	memcpy(nums_, r.nums_, N_ * sizeof(T));
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
CategorizedRepository<T, enlarge_factor, max_size>::CategorizedRepository(
		const category_t cat) :
	C_(cat), N_(100), nStored_(0), count_(0), offset_(0), items_(0), nums_(0),
			ids_(0)
{
	assert(C_> 0);
	allocate();
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
CategorizedRepository<T, enlarge_factor, max_size>::CategorizedRepository(
		const category_t cat, const address_t N) :
	C_(cat), N_(N), nStored_(0), count_(0), offset_(0), items_(0), nums_(0),
			ids_(0)
{
	assert(C_> 0);
	//	assert(N_> 0);
	allocate();
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
CategorizedRepository<T, enlarge_factor, max_size>::~CategorizedRepository()
{
	clear();
}

// --------------- memory management

template<class T, unsigned int enlarge_factor, unsigned int max_size>
void CategorizedRepository<T, enlarge_factor, max_size>::allocate()
{
	delete[] count_;
	delete[] offset_;
	delete[] items_;
	delete[] nums_;
	delete[] ids_;
	count_ = new address_t[C_ + 1];
	offset_ = new address_t[C_ + 1];
	if (N_ > 0)
	{
		items_ = new T[N_];
		nums_ = new address_t[N_];
		ids_ = new id_t[N_];
	}
	else
	{
		items_ = 0;
		nums_ = 0;
		ids_ = 0;
	}
	assert(count_);
	assert(offset_);
	memset(count_, 0, C_ * sizeof(address_t));
	count_[C_] = N_;
	memset(offset_, 0, (C_ + 1) * sizeof(address_t));
	if (items_)
		memset(items_, 0, N_ * sizeof(T));
	for (address_t i = 0; i < N_; i++)
	{
		nums_[i] = i;
		ids_[i] = i;
	}

}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
void CategorizedRepository<T, enlarge_factor, max_size>::clear()
{
	delete[] count_;
	delete[] offset_;
	delete[] items_;
	delete[] nums_;
	delete[] ids_;
	count_ = 0;
	offset_ = 0;
	items_ = 0;
	nums_ = 0;
	ids_ = 0;
	nStored_ = 0;
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
bool CategorizedRepository<T, enlarge_factor, max_size>::enlarge()
{
	address_t newsize = (N_ > 0) ? N_ * enlarge_factor : enlarge_factor;
	if (newsize > max_size)
		newsize = max_size;
	if (newsize <= N_)
		return false;
	else
	{
		// printf("Resizing Storage To %d\n",newsize);
		T* itembuf = items_;
		items_ = new T[newsize];
		assert(items_);
		memset(items_, 0, newsize * sizeof(T));
		memcpy(items_, itembuf, N_ * sizeof(T));
		delete[] itembuf;

		id_t* idbuf = ids_;
		ids_ = new id_t[newsize];
		assert(ids_);
		for (address_t i = N_; i < newsize; i++)
			ids_[i] = i;
		memcpy(ids_, idbuf, N_ * sizeof(id_t));
		delete[] idbuf;

		address_t* numbuf = nums_;
		nums_ = new address_t[newsize];
		assert(nums_);
		for (address_t i = N_; i < newsize; i++)
			nums_[i] = i;
		memcpy(nums_, numbuf, N_ * sizeof(address_t));
		delete[] numbuf;

		count_[C_] += newsize - N_;
		N_ = newsize;
		return true;
	}
}

// --------------- category management

template<class T, unsigned int enlarge_factor, unsigned int max_size>
void CategorizedRepository<T, enlarge_factor, max_size>::increaseCat(
		const address_t n, const category_t cat)
{
	assert(n < N_);
	assert(cat <= C_);
	category_t cls = category(n);
	address_t mynum = n;
	id_t myid = ids_[mynum];
	assert(cls <= cat);
	while (cls < cat) // Keep increasing
	{
		// First go to last pos in current group
		address_t tarnum = offset_[cls] + count_[cls] - 1;
		if (mynum != tarnum)
		{
			id_t idbuf = ids_[tarnum];
			ids_[tarnum] = ids_[mynum];
			ids_[mynum] = idbuf;
			nums_[myid] = tarnum; // This is right cause we switched nums
			nums_[idbuf] = mynum;
			mynum = tarnum;
		}
		--count_[cls]; // Now shift the boundary
		++count_[cls + 1];
		--offset_[cls + 1];
		++cls;
	}
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
void CategorizedRepository<T, enlarge_factor, max_size>::decreaseCat(
		const address_t n, const category_t cat)
{
	assert(n < N_);
	assert(cat <= C_);
	category_t cls = category(n);
	address_t mynum = n;
	id_t myid = ids_[mynum];
	assert(cls >= cat);
	while (cls > cat) // Keep decreasing
	{
		// First go to first pos in current group
		address_t tarnum = offset_[cls];
		if (mynum != tarnum)
		{
			id_t idbuf = ids_[tarnum];
			ids_[tarnum] = ids_[mynum];
			ids_[mynum] = idbuf;
			nums_[myid] = tarnum; // This is right cause we switched nums
			nums_[idbuf] = mynum;
			mynum = tarnum;
		}
		--count_[cls]; // Now shift the boundary
		++count_[cls - 1];
		++offset_[cls];
		--cls;
	}
}

// --------------- PUBLIC interface

template<class T, unsigned int enlarge_factor, unsigned int max_size>
inline id_size_t CategorizedRepository<T, enlarge_factor, max_size>::count() const
{
	return nStored_;
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
inline id_size_t CategorizedRepository<T, enlarge_factor, max_size>::size() const
{
	return count();
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
inline id_size_t CategorizedRepository<T, enlarge_factor, max_size>::count(
		const category_t cat) const
{
	assert(cat < C_);
	return count_[cat];
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
inline address_t CategorizedRepository<T, enlarge_factor, max_size>::capacity() const
{
	return N_;
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
inline category_t CategorizedRepository<T, enlarge_factor, max_size>::numberOfCategories() const
{
	return C_;
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
inline category_t CategorizedRepository<T, enlarge_factor, max_size>::category(
		const id_t id) const
{
	assert(id < N_);
	return category(nums_[id]);
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
inline category_t CategorizedRepository<T, enlarge_factor, max_size>::category(
		const address_t n) const
{
	assert(n < N_);
	address_t na = n;
	category_t c = 0;
	while (na >= count_[c])
	{
		na -= count_[c];
		++c;
	}
	return c;
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
void CategorizedRepository<T, enlarge_factor, max_size>::setCategory(
		const id_t id, const category_t cat)
{
	assert(id < N_);
	setCategory(nums_[id], cat);
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
void CategorizedRepository<T, enlarge_factor, max_size>::setCategory(
		const address_t n, const category_t cat)
{
	assert(cat < C_);
	assert(n <= nStored_);
	category_t cls = category(n);
	if (cls < cat)
		increaseCat(n, cat);
	else
		decreaseCat(n, cat);
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
void CategorizedRepository<T, enlarge_factor, max_size>::setCategory(
		const category_t oldCat, const address_t n, const category_t newCat)
{
	assert(oldCat < C_);
	assert(n < count_[oldCat]);
	assert(newCat < C_);
	address_t num = offset_[oldCat] + n;
	setCategory(num, newCat);
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
inline T& CategorizedRepository<T, enlarge_factor, max_size>::item(
		const id_t id) const
{
	assert(id < N_);
	return items_[id];
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
inline T& CategorizedRepository<T, enlarge_factor, max_size>::operator[](
		const id_t id) const
{
	return item(id);
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
inline T& CategorizedRepository<T, enlarge_factor, max_size>::item(
		const address_t n) const
{
	assert(n < N_);
	return items_[ids_[n]];
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
inline T& CategorizedRepository<T, enlarge_factor, max_size>::operator()(const address_t n) const
{
	return item(n);
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
inline T& CategorizedRepository<T, enlarge_factor, max_size>::item(
		const category_t cat, const address_t n) const
{
	assert(cat < C_);
	assert(n < count_[cat]);
	return items_[ids_[offset_[cat] + n]];
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
inline T& CategorizedRepository<T, enlarge_factor, max_size>::operator()(const category_t cat, const address_t n) const
{
	return item(cat, n);
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
inline id_t CategorizedRepository<T, enlarge_factor, max_size>::id(
		const address_t n) const
{
	assert(n < N_);
	return ids_[n];
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
inline id_t CategorizedRepository<T, enlarge_factor, max_size>::id(
		const category_t cat, const address_t n) const
{
	assert(cat < C_);
	assert(n < N_);
	return ids_[offset_[cat] + n];
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
id_t CategorizedRepository<T, enlarge_factor, max_size>::insert(const T itm,
		const category_t cat)
{
	assert(cat < C_);
	// Basic storage at the end
	address_t curnum = offset_[C_];
	if (curnum >= N_)
	{
		if (!enlarge())
			throw(RepoAllocException());
	}
	id_t uid = ids_[curnum];
	// TODO This stores a copy of itm. If T is not a pointer type, this is proper value semantics.
	// However, then we should take ownership of items as well. How is this done in the STL containers?
	items_[uid] = itm; // Store Item
	decreaseCat(curnum, cat); // Move into right class
	++nStored_;
	return uid;
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
id_t CategorizedRepository<T, enlarge_factor, max_size>::insert(const T itm)
{
	return insert(itm, 0);
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
id_t CategorizedRepository<T, enlarge_factor, max_size>::operator<<(const T itm)
{
	return insert(itm, 0);
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
T CategorizedRepository<T, enlarge_factor, max_size>::remove(const id_t id)
{
	assert(id < N_);
	return remove(nums_[id]);
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
T CategorizedRepository<T, enlarge_factor, max_size>::remove(const address_t n)
{
	assert(n < N_);
	T itm = items_[ids_[n]];
	items_[ids_[n]] = 0; // thus, IDs remain unique and unchanged
	increaseCat(n, C_);
	--nStored_;
	return itm;
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
void CategorizedRepository<T, enlarge_factor, max_size>::removeAll()
{
	while (nStored_)
	{
		remove(static_cast<address_t> (0));
	}
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
void CategorizedRepository<T, enlarge_factor, max_size>::deleteAll()
{
	while (nStored_)
	{
		T x = remove(static_cast<address_t> (0));
		delete x;
	}
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
const/* */typename CategorizedRepository<T, enlarge_factor, max_size>::IndexIterator CategorizedRepository<T, enlarge_factor, max_size>::begin() const
{
	return typename CategorizedRepository<T, enlarge_factor, max_size>::IndexIterator(*this, 0);
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
const typename CategorizedRepository<T, enlarge_factor, max_size>::IndexIterator
CategorizedRepository<T, enlarge_factor, max_size>::end() const
{
	//TODO: wurde von Sebastian Kuehn geaendert

	return typename CategorizedRepository<T, enlarge_factor, max_size>::IndexIterator(*this, nStored_);

	/*
	 if (nStored_> 0)
	 {
	 id_t lastID = static_cast<id_t>(N_) - 1;
	 while (nums_[lastID] >= nStored_)
	 {
	 --lastID;
	 }
	 return typename CategorizedRepository<T, enlarge_factor, max_size>::IndexIterator(*this, lastID + 1); // past-the-end element
	 }
	 else
	 {
	 return typename CategorizedRepository<T, enlarge_factor, max_size>::IndexIterator(*this, 0);
	 }
	 */
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
typename CategorizedRepository<T, enlarge_factor, max_size>::IndexIteratorRange
CategorizedRepository<T, enlarge_factor, max_size>::ids() const
{
	return std::make_pair(begin(), end());
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
typename CategorizedRepository<T, enlarge_factor, max_size>::CategoryIteratorRange
CategorizedRepository<T, enlarge_factor, max_size>::ids(
		const category_t cat) const
{
	assert(cat < C_);
	/*
	 * This is safe, because any empty items are guaranteed to be put in the
	 * private category C_. See increaseCat() and decreaseCat().
	 */
	return std::make_pair(
			typename CategorizedRepository<T, enlarge_factor, max_size>::CategoryIterator(*this, cat, offset_[cat]),
			typename CategorizedRepository<T, enlarge_factor, max_size>::CategoryIterator(*this, cat, offset_[cat] + count_[cat])
	);
}

// --------------- IndexIterator

template<class T, unsigned int enlarge_factor, unsigned int max_size>
CategorizedRepository<T, enlarge_factor, max_size>::IndexIterator::IndexIterator() : rep_(0), cur_(0)
{
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
CategorizedRepository<T, enlarge_factor, max_size>::IndexIterator::IndexIterator(const CategorizedRepository<T, enlarge_factor, max_size>& repo)
: rep_(&repo), cur_(0)
{
	//TODO: wurde von Sebastian Kuehn geaendert
	assert(rep_ != NULL);
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
CategorizedRepository<T, enlarge_factor, max_size>::IndexIterator::IndexIterator(const CategorizedRepository<T, enlarge_factor, max_size>& repo,
		const id_t n) : rep_(&repo), cur_(n)
{
	//TODO: wurde von Sebastian Kuehn geaendert
	assert(rep_ != NULL);
	assert(cur_ <= rep_->nStored_);
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
CategorizedRepository<T, enlarge_factor, max_size>::IndexIterator::IndexIterator(
		const typename CategorizedRepository<T, enlarge_factor, max_size>::IndexIterator& it)
: rep_(it.rep_), cur_(it.cur_)
{
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
CategorizedRepository<T, enlarge_factor, max_size>::IndexIterator::~IndexIterator()
{
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
typename CategorizedRepository<T, enlarge_factor, max_size>::IndexIterator&
CategorizedRepository<T, enlarge_factor, max_size>::IndexIterator::operator=(
		const typename CategorizedRepository<T, enlarge_factor, max_size>::IndexIterator& it)
{
	rep_ = it.rep_;
	cur_ = it.cur_;
	return *this;
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
inline bool CategorizedRepository<T, enlarge_factor, max_size>::IndexIterator::operator==(
		const typename CategorizedRepository<T, enlarge_factor, max_size>::IndexIterator& it)
{
	return ((rep_ == it.rep_) && (cur_ == it.cur_));
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
inline bool CategorizedRepository<T, enlarge_factor, max_size>::IndexIterator::operator!=(
		const typename CategorizedRepository<T, enlarge_factor, max_size>::IndexIterator& it)
{
	return !this->operator==(it);
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
inline typename CategorizedRepository<T, enlarge_factor, max_size>::IndexIterator&
CategorizedRepository<T, enlarge_factor, max_size>::IndexIterator::operator++()
{
	//TODO: wurde von Sebastian Kuehn geaendert
	assert(rep_ != NULL);

	while (cur_ < rep_->nStored_)
	{
		++cur_;
		if (rep_->nums_[cur_] < rep_->nStored_) break;
	}

	return *this;
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
inline typename CategorizedRepository<T, enlarge_factor, max_size>::IndexIterator
CategorizedRepository<T, enlarge_factor, max_size>::IndexIterator::operator++(int)
{
	typename CategorizedRepository<T, enlarge_factor, max_size>::IndexIterator tmp(*this);
	++(*this);
	return(tmp);
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
id_t CategorizedRepository<T, enlarge_factor, max_size>::IndexIterator::operator*()
{
	//TODO: wurde von Sebastian Kuehn geaendert
	assert(rep_ != NULL);
	assert(*this != rep_->end());

	return cur_;
}

// --------------- CategoryIterator

template<class T, unsigned int enlarge_factor, unsigned int max_size>
CategorizedRepository<T, enlarge_factor, max_size>::CategoryIterator::CategoryIterator() : rep_(NULL), category_(0), cur_(0)
{
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
CategorizedRepository<T, enlarge_factor, max_size>::CategoryIterator::CategoryIterator(const CategorizedRepository<T, enlarge_factor, max_size>& repo,
		const category_t cat) : rep_(&repo), category_(cat), cur_(0)
{
	//TODO: wurde von Sebastian Kuehn geaendert
	assert(&repo != NULL);
	assert(cat < repo.C_);
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
CategorizedRepository<T, enlarge_factor, max_size>::CategoryIterator::CategoryIterator(const CategorizedRepository<T, enlarge_factor, max_size>& repo,
		const category_t cat, const address_t n) : rep_(&repo), category_(cat), cur_(n)
{
	//TODO: wurde von Sebastian Kuehn geaendert
	assert(&repo != NULL);
	assert(cat < repo.C_);
	assert(n < repo.count_[cat]);
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
CategorizedRepository<T, enlarge_factor, max_size>::CategoryIterator::CategoryIterator(
		const typename CategorizedRepository<T, enlarge_factor, max_size>::CategoryIterator& it)
: rep_(it.rep_), category_(it.category_), cur_(it.cur_)
{
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
CategorizedRepository<T, enlarge_factor, max_size>::CategoryIterator::~CategoryIterator()
{
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
typename CategorizedRepository<T, enlarge_factor, max_size>::CategoryIterator&
CategorizedRepository<T, enlarge_factor, max_size>::CategoryIterator::operator=(
		const typename CategorizedRepository<T, enlarge_factor, max_size>::CategoryIterator& it)
{
	rep_ = it.rep_;
	category_ = it.category_;
	cur_ = it.cur_;
	return *this;
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
inline bool CategorizedRepository<T, enlarge_factor, max_size>::CategoryIterator::operator==(
		const typename CategorizedRepository<T, enlarge_factor, max_size>::CategoryIterator& it)
{
	// a bit of a crude (and non-deep) equality test here...
	return ((rep_ == it.rep_) && (category_ == it.category_) && (cur_ == it.cur_));
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
inline bool CategorizedRepository<T, enlarge_factor, max_size>::CategoryIterator::operator!=(
		const typename CategorizedRepository<T, enlarge_factor, max_size>::CategoryIterator& it)
{
	return !this->operator==(it);
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
inline typename CategorizedRepository<T, enlarge_factor, max_size>::CategoryIterator&
CategorizedRepository<T, enlarge_factor, max_size>::CategoryIterator::operator++()
{
	//TODO: wurde von Sebastian Kuehn geaendert
	assert(rep_ != NULL);

	if (rep_)
	{
		++cur_;
	}
	return *this;
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
inline typename CategorizedRepository<T, enlarge_factor, max_size>::CategoryIterator
CategorizedRepository<T, enlarge_factor, max_size>::CategoryIterator::operator++(int)
{
	typename CategorizedRepository<T, enlarge_factor, max_size>::CategoryIterator tmp(*this);
	++(*this);
	return(tmp);
}

template<class T, unsigned int enlarge_factor, unsigned int max_size>
id_t CategorizedRepository<T, enlarge_factor, max_size>::CategoryIterator::operator*()
{
	//TODO: wurde von Sebastian Kuehn geaendert
	assert(rep_ != NULL);
	assert(rep_->nums_[cur_] < (rep_->offset_[category_] + rep_->count_[category_]));

	return rep_->ids_[cur_];

	/*
	 if (rep_)
	 {
	 return rep_->ids_[cur_];
	 }
	 else
	 {
	 return 0; // FIXME is this wise?
	 }
	 */
}

}
#endif /* CATEGORIZEDREPOSITORY_H_ */
