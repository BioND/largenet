/**
 * This work is licensed under the Creative Commons
 * Attribution-NonCommercial 3.0 Unported License. To view a copy of this
 * license, visit http://creativecommons.org/licenses/by-nc/3.0/ or send a
 * letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View,
 * California, 94041, USA.
 */

#include "motif_construction.h"
#include "../NodeMotif.h"
#include "../LinkMotif.h"
#include "../TripleMotif.h"
#include "../QuadLineMotif.h"
#include "../QuadStarMotif.h"

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

template<>
void constructAllMotifs(std::set<QuadLineMotif>& m, node_state_size_t states)
{
	for (node_state_t a = 0; a < states; ++a)
		for (node_state_t b = 0; b < states; ++b)
			for (node_state_t c = 0; c < states; ++c)
				for (node_state_t d = 0; d < states; ++d)
					m.insert(QuadLineMotif(a, b, c, d));
}

template<>
void constructAllMotifs(std::set<QuadStarMotif>& m, node_state_size_t states)
{
	for (node_state_t a = 0; a < states; ++a)
		for (node_state_t b = 0; b < states; ++b)
			for (node_state_t c = 0; c < states; ++c)
				for (node_state_t d = 0; d < states; ++d)
					m.insert(QuadStarMotif(a, b, c, d));
}

}
}
}
