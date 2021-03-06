/**
 * This work is licensed under the Creative Commons
 * Attribution-NonCommercial 3.0 Unported License. To view a copy of this
 * license, visit http://creativecommons.org/licenses/by-nc/3.0/ or send a
 * letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View,
 * California, 94041, USA.
 */

#ifndef MOTIFSET_H_
#define MOTIFSET_H_

#include <largenet/motifs/detail/motif_construction.h>

namespace lnet
{
namespace motifs {


template<class Motif>
class MotifSet
{
public:
	typedef	typename std::set<Motif>::const_iterator const_iterator;
	typedef typename std::set<Motif>::const_iterator iterator;
	MotifSet(node_state_size_t states);
	unsigned int size() const;
	iterator begin() const;
	iterator end() const;
private:
	std::set<Motif> motifs_;
};

template<class Motif>
MotifSet<Motif>::MotifSet(const node_state_size_t states)
{
	detail::constructAllMotifs(motifs_, states);
}

template<class Motif>
inline typename MotifSet<Motif>::iterator MotifSet<Motif>::begin() const
{
	return motifs_.begin();
}

template<class Motif>
inline typename MotifSet<Motif>::iterator MotifSet<Motif>::end() const
{
	return motifs_.end();
}

template<class Motif>
inline unsigned int MotifSet<Motif>::size() const
{
	return motifs_.size();
}

}
}
#endif /* MOTIFSET_H_ */
