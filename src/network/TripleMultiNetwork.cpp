/**
 * @file TripleMultiNetwork.cpp
 * @date 12.05.2009
 * @author gerd
 */

#include "TripleMultiNetwork.h"
#include <vector>

namespace lnet
{

TripleMultiNetwork::TripleMultiNetwork() :
	TypedNetwork<NodeType, LinkType> (), tripleStore_(new TripleRepo(1, 0)), tsCalc_(
			new ConstTripleState<> ), tscOwn_(true)
{
}

TripleMultiNetwork::TripleMultiNetwork(const id_size_t nNodes,
		const id_size_t nLinks, const node_state_size_t nNodeStates,
		const link_state_size_t nLinkStates,
		const triple_state_size_t nTripleStates, LinkStateCalculator* lsCalc,
		TripleStateCalculator* tsCalc) :
			TypedNetwork<NodeType, LinkType> (nNodes, nLinks, nNodeStates,
					nLinkStates, lsCalc), tripleStore_(new TripleRepo(
					nTripleStates, 0)), tsCalc_(tsCalc), tscOwn_(false)
{
}

TripleMultiNetwork::~TripleMultiNetwork()
{
	tripleStore_->deleteAll();
	delete tripleStore_;
	if (tscOwn_)
		delete tsCalc_;
}

void TripleMultiNetwork::setTripleStateCalculator(TripleStateCalculator* tsCalc)
{
	if (tscOwn_)
	{
		delete tsCalc_;
		tscOwn_ = false;
	}
	if (tsCalc != 0)
	{
		if (isValidTripleStateCalculator(tsCalc))
			tsCalc_ = tsCalc;
		else
			throw(std::invalid_argument(
					"TripleStateCalculator gives impossible triple states!"));
	}
	else
	{
		tsCalc_ = new ConstTripleState<> ;
		tscOwn_ = true;
	}

}

bool TripleMultiNetwork::isValidTripleStateCalculator(
		TripleStateCalculator* tsCalc) const
{
	bool retval = true;
	for (node_state_t i = 0; i < numberOfNodeStates(); ++i)
		for (node_state_t j = 0; j < numberOfNodeStates(); ++j)
			for (node_state_t k = 0; k < numberOfNodeStates(); ++k)
				retval &= ((*tsCalc)(i, j, k) < numberOfTripleStates());
	return retval;
}

node_id_t TripleMultiNetwork::leftNode(const triple_id_t t) const
{
	node_id_t c = source(left(t));
	if ((source(right(t)) == c) || (target(right(t)) == c))
		return target(left(t));
	else
		return c;
}

node_id_t TripleMultiNetwork::rightNode(const triple_id_t t) const
{
	node_id_t c = source(right(t));
	if ((source(left(t)) == c) || (target(left(t)) == c))
		return target(right(t));
	else
		return c;
}

node_id_t TripleMultiNetwork::centerNode(const triple_id_t t) const
{
	node_id_t c = source(left(t));
	if ((source(right(t)) == c) || (target(right(t)) == c))
		return c;
	else
		return target(left(t));
}

void TripleMultiNetwork::onNodeStateChange(const node_id_t n)
{
	TypedNetwork<NodeType, LinkType>::onNodeStateChange(n);
	NeighborLinkIteratorRange iters = neighborLinks(n);
	for (NeighborLinkIterator& it = iters.first; it != iters.second; ++it)
	{
		NeighborTripleIteratorRange niters = neighborTriples(*it);
		for (NeighborTripleIterator& nit = niters.first; nit != niters.second; ++nit)
		{
			tripleStore_->setCategory(*nit, (*tsCalc_)(getNodeState(leftNode(
					*nit)), getNodeState(centerNode(*nit)), getNodeState(
					rightNode(*nit))));
		}
	}
}

std::pair<bool, triple_id_t> TripleMultiNetwork::randomTriple() const
{
	std::pair<bool, triple_id_t> ret = std::make_pair(false, 0);
	if (tripleStore_->size() > 0)
	{
		ret.first = true;
		ret.second = tripleStore_->id(
				static_cast<repo::address_t> (rng.IntFromTo(0,
						tripleStore_->size() - 1)));
	}
	return ret;
}

std::pair<bool, triple_id_t> TripleMultiNetwork::randomTriple(
		const triple_state_t s) const
{
	std::pair<bool, triple_id_t> ret = std::make_pair(false, 0);
	if (tripleStore_->count(s) > 0)
	{
		ret.first = true;
		ret.second = tripleStore_->id(s,
				static_cast<repo::address_t> (rng.IntFromTo(0,
						tripleStore_->count(s) - 1)));
	}
	return ret;
}

void TripleMultiNetwork::addTriple(const link_id_t left, const link_id_t right)
{
	node_id_t l, c, r;
	if (target(left) == source(right))
	{
		l = source(left);
		c = target(left);
		r = target(right);
	}
	else if (target(left) == target(right))
	{
		l = source(left);
		c = target(left);
		r = source(right);
	}
	else if (source(left) == source(right))
	{
		l = target(left);
		c = source(left);
		r = target(right);
	}
	else // source(left) == target(right)
	{
		l = target(left);
		c = source(left);
		r = source(right);
	}

	const triple_id_t t = tripleStore_->insert(new Triple(left, right),
			(*tsCalc_)(getNodeState(l), getNodeState(c), getNodeState(r)));
	link(left).addTriple(t);
	link(right).addTriple(t);
}

link_id_t TripleMultiNetwork::doAddLink(const node_id_t source,
		const node_id_t target)
{
	const link_id_t l = TypedNetwork<NodeType, LinkType>::doAddLink(source,
			target);

	// create a new triple for each adjacent link of source (except the newly created one)
	NeighborLinkIteratorRange siters = neighborLinks(source);
	for (NeighborLinkIterator& it = siters.first; it != siters.second; ++it)
	{
		if (*it == l)
			continue;
		addTriple(l, *it);
	}

	// now do the same for each adjacent link of target
	NeighborLinkIteratorRange titers = neighborLinks(target);
	for (NeighborLinkIterator& it = titers.first; it != titers.second; ++it)
	{
		if (*it == l)
			continue;
		addTriple(l, *it);
	}
	return l;
}

void TripleMultiNetwork::removeTriplesFromLinkEnd(const link_id_t l,
		const bool fromSource)
{
	std::vector<triple_id_t> temp;
	const node_id_t s = fromSource ? link(l).source() : link(l).target();

	// remove all triples containing l where s is the center node
	NeighborTripleIteratorRange iters = neighborTriples(l);
	for (NeighborTripleIterator& it = iters.first; it != iters.second; ++it)
	{
		NeighborLinkIteratorRange niters = neighborLinks(s);
		for (NeighborLinkIterator& nit = niters.first; nit != niters.second; ++nit)
		{
			if ((*nit != l) && triple(*it).containsLink(*nit))
			{
				link(*nit).removeTriple(*it);
				temp.push_back(*nit);
			}
		}
	}
	for (std::vector<triple_id_t>::const_iterator tit = temp.begin(); tit
			!= temp.end(); ++tit)
	{
		link(l).removeTriple(*tit);
		delete tripleStore_->remove(*tit);
	}
}

bool TripleMultiNetwork::doChangeLink(const link_id_t l, const node_id_t source,
		const node_id_t target)
{
	bool retval = true, source_changed = false, target_changed = false;
	if (link(l).source() != source)
	{
		removeTriplesFromLinkEnd(l, true);
		source_changed = true;
	}
	if (link(l).target() != target)
	{
		removeTriplesFromLinkEnd(l, false);
		target_changed = true;
	}

	// now change the link, updating its state
	retval &= TypedNetwork<NodeType, LinkType>::doChangeLink(l, source, target);

	// and add the newly created triples accordingly
	if (source_changed)
	{
		NeighborLinkIteratorRange iters = neighborLinks(source);
		for (NeighborLinkIterator& it = iters.first; it != iters.second; ++it)
		{
			if (*it == l)
				continue;
			addTriple(l, *it);
		}
	}
	if (target_changed)
	{
		NeighborLinkIteratorRange iters = neighborLinks(target);
		for (NeighborLinkIterator& it = iters.first; it != iters.second; ++it)
		{
			if (*it == l)
				continue;
			addTriple(l, *it);
		}
	}
	return retval;
}

void TripleMultiNetwork::doRemoveLink(const link_id_t l)
{
	// delete each triple this link is a part of and update neighboring links accordingly
	NeighborTripleIteratorRange iters = neighborTriples(l);
	for (NeighborTripleIterator& it = iters.first; it != iters.second; ++it)
	{
		link_id_t nl = left(*it) == l ? right(*it) : left(*it);
		link(nl).removeTriple(*it);
		delete tripleStore_->remove(*it);
	}
	TypedNetwork<NodeType, LinkType>::doRemoveLink(l);
}

void TripleMultiNetwork::doRemoveAllLinks()
{
	// removing all links also removes all triples
	// remove all triple references from the links first
	LinkIteratorRange iters = links();
	for (LinkIterator& it = iters.first; it != iters.second; ++it)
	{
		link(*it).clear();
	}
	tripleStore_->deleteAll();
	// now really delete all links and update nodes accordingly
	TypedNetwork<NodeType, LinkType>::doRemoveAllLinks();
}

void TripleMultiNetwork::doRemoveNode(const node_id_t n)
{
	// remove all triples containing this node
	NeighborLinkIteratorRange iters = neighborLinks(n);
	for (NeighborLinkIterator& it = iters.first; it != iters.second; ++it)
	{
		NeighborTripleIteratorRange titers = neighborTriples(*it);
		for (NeighborTripleIterator& tit = titers.first; tit != titers.second; ++tit)
		{
			link_id_t nl = left(*tit) == *it ? right(*tit) : left(*tit);
			link(nl).removeTriple(*tit);
			delete tripleStore_->remove(*tit);
		}
		// link(*it).clear();
	}
	// without clearing link(*it) above, the adjacent links of node n
	// now reference non-existent triples; this, however, is no problem
	// since they are deleted anyway now
	TypedNetwork<NodeType, LinkType>::doRemoveNode(n);
}
}
