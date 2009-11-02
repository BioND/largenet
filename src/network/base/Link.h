/**
 * @file Link.h
 * @author Gerd Zschaler
 * @date Aug 15, 2008
 * Declaration of the lnet::Link class.
 */
#ifndef LINK_H_
#define LINK_H_

#include "types.h"
#include <stdexcept>

namespace lnet
{

/**
 * Class representing one link in a network.
 *
 * Stores the IDs of the link's source and target nodes and a state value.
 */
class Link
{
public:
	// Default constructor
	Link();
	/**
	 * Basic constructor. Creates a link with end node IDs @p source and @p target.
	 * The new link will have zero state.
	 * @param source %Node ID to link from.
	 * @param target %Node ID to link to.
	 */
	Link(node_id_t source, node_id_t target);
	/*
	 * Constructor. Creates a link with end node IDs @p source and @p target with
	 * initial state @p state.
	 * @param source %Node ID to link from.
	 * @param target %Node ID to link to.
	 * @param state Initial link state.
	 */
	//	Link(node_id_t source, node_id_t target, link_state_t state);

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
	 * Get ID of node opposite to given link end ID.
	 * @param ID of one link end
	 * @return ID of other link end
	 */
	node_id_t opposite(node_id_t n) const;

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

	/**
	 * Get link state.
	 * @return %Link state.
	 */
	//	link_state_t state() const;
	/**
	 * Set link state.
	 * @param state New link state.
	 */
	//	void setState(link_state_t state);

private:
	// We use the identifiers source and target
	// even though this is an undirected link (for now).
	node_id_t source_; ///< Unique ID of source node.
	node_id_t target_; ///< Unique ID of target node.

	//	link_state_t state_; ///< Link state.
};

inline Link::Link() :
	source_(0), target_(0)
{
}

inline Link::Link(const node_id_t source, const node_id_t target) :
	source_(source), target_(target)
{
}

inline node_id_t Link::source() const
{
	return source_;
}

inline node_id_t Link::target() const
{
	return target_;
}

inline node_id_t Link::opposite(const node_id_t n) const
{
	if (n == source_)
		return target_;
	else if (n == target_)
		return source_;
	else throw std::invalid_argument("Cannot retrieve link opposite, link does not connect to given node.");
}

inline bool Link::connectsTo(const node_id_t n) const
{
	return ((source_ == n) || (target_ == n));
}

inline bool Link::isLoop() const
{
	return source_ == target_;
}

//inline link_state_t Link::state() const
//{
//	return state_;
//}

//inline void Link::setState(const link_state_t state)
//{
//	state_ = state;
//}

inline void Link::setSource(const node_id_t source)
{
	source_ = source;
}

inline void Link::setTarget(const node_id_t target)
{
	target_ = target;
}

}

#endif /*LINK_H_*/
