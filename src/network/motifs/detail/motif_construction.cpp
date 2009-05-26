/*
 * motif_construction.cpp
 *
 *  Created on: 04.02.2009
 *      Author: gerd
 */

#include "motif_construction.h"

namespace lnet
{
namespace motifs
{
namespace detail
{

template<>
void constructAllMotifs(std::set<NodeMotif>& m, node_state_size_t states)
{
	for (node_state_t n = 0; n < states; ++n)
		m.insert(NodeMotif(n));
}

template<>
void constructAllMotifs(std::set<LinkMotif>& m, node_state_size_t states)
{
	for (node_state_t a = 0; a < states; ++a)
		for (node_state_t b = 0; b < states; ++b)
			m.insert(LinkMotif(a, b));
}

template<>
void constructAllMotifs(std::set<TripleMotif>& m, node_state_size_t states)
{
	for (node_state_t a = 0; a < states; ++a)
		for (node_state_t b = 0; b < states; ++b)
			for (node_state_t c = 0; c < states; ++c)
				m.insert(TripleMotif(a, b, c));
}

}
}
}
