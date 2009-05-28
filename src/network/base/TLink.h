/*
 * TLink.h
 *
 *  Created on: 12.05.2009
 *      Author: gerd
 */

#ifndef TLINK_H_
#define TLINK_H_

#include "types.h"
#include "Link.h"
#include <set>
#include <utility>

namespace lnet
{

/**
 * A network link tracking the triples it is part of.
 */
class TLink
{
public:
	typedef std::multiset<triple_id_t> TripleSet;
	typedef TripleSet::iterator TripleIDIterator;
	typedef std::pair<TripleIDIterator, TripleIDIterator> TripleIDIteratorRange;

public:
	/**
	 * Default constructor
	 */
	TLink();
	/**
	 * Constructor.
	 */
	TLink(node_id_t source, node_id_t target);

///@{ Link interface
	/**
	 * Get ID of source node.
	 * @return Source node ID.
	 */
	node_id_t source() const;
	/**
	 * Get ID of target node.
	 * @return Target node ID.
	 */
	node_id_t target() const;

	/**
	 * Set source node ID of the link.
	 * @param source Source node ID.
	 */
	void setSource(node_id_t source);
	/**
	 * Set target node ID of the link.
	 * @param target Target node ID.
	 */
	void setTarget(node_id_t target);

	/**
	 * Check whether @p n is a link end.
	 * @param n %Node ID to check.
	 * @return True if @p n is either @p source or @p target.
	 */
	bool connectsTo(node_id_t n) const;
	/**
	 * Check if link is a loop.
	 * @return True if the link @p source equals the link @p target.
	 */
	bool isLoop() const;
///@}

	/**
	 * Return the number of triples this link is part of. Similar to a node's
	 * degree we call this the link's degree.
	 * @return %Link degree
	 */
	triple_id_t degree() const;

	/**
	 * Clear the link's list of triples. Note that this does not
	 * affect the %Triple objects this link might be associated with.
	 */
	void clear();

	/**
	 * Insert triple ID @p t into the link's triple set.
	 * @param t %Triple ID
	 */
	void addTriple(triple_id_t t);

	/**
	 * Remove triple ID @p from the link's triple set.
	 * @param t %Triple ID
	 */
	void removeTriple(triple_id_t t);

	/**
	 * Return true if %Triple ID @p t is in the link's triple set.
	 * @param t %Triple ID
	 * @return True if found
	 */
	bool inTriple(triple_id_t t) const;

	TripleIDIteratorRange triples() const;

private:
	Link link_;		///< Underlying Link object
	TripleSet triples_;  ///< Multiset of triple IDs
};

inline TLink::TLink() : link_()
{
}

inline TLink::TLink(const node_id_t source, const node_id_t target) :
	link_(source, target)
{
}

inline node_id_t TLink::source() const
{
	return link_.source();
}

inline node_id_t TLink::target() const
{
	return link_.target();
}

inline void TLink::setSource(const node_id_t source)
{
	link_.setSource(source);
}

inline void TLink::setTarget(const node_id_t target)
{
	link_.setTarget(target);
}

inline bool TLink::connectsTo(const node_id_t n) const
{
	return link_.connectsTo(n);
}

inline bool TLink::isLoop() const
{
	return link_.isLoop();
}

inline triple_id_t TLink::degree() const
{
	return triples_.size();
}

inline void TLink::clear()
{
	triples_.clear();
}

inline void TLink::addTriple(const triple_id_t t)
{
	triples_.insert(t);
}

inline void TLink::removeTriple(const triple_id_t t)
{
	TripleSet::iterator it = triples_.find(t);
	if (it != triples_.end())
		triples_.erase(it);
}

inline bool TLink::inTriple(const triple_id_t t) const
{
	return (triples_.find(t) != triples_.end());
}

inline TLink::TripleIDIteratorRange TLink::triples() const
{
	return std::make_pair(triples_.begin(), triples_.end());
}

}

#endif /* TLINK_H_ */
