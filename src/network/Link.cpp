#include "Link.h"

namespace lnet
{

Link::Link(const node_id_t source, const node_id_t target) :
	source_(source), target_(target)
{
}

//Link::Link(const node_id_t source, const node_id_t target, const link_state_t state) :
//	source_(source), target_(target), state_(state)
//{
//}

}
