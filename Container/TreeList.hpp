#pragma once

#include "TreeIterator.hpp"
#include "List.hpp"

#define _DEBUGTREELIST _DEBUG && _DEBUGTREEITERATOR && 1

namespace cg {

template<typename T,
	template <typename, typename, bool, bool> typename IteratorType>
class TreeListImpl : public cg::List<T>
{
public:
	/**Create a Tree list.*/
	TreeListImpl() {};
private:

};





template<typename T>
using TreeList = TreeListImpl<T, cg::TreeIterator>;

#if _DEBUGTREELIST
/**Force some instantiations*/
template class TreeListImpl<int, cg::TreeIterator>;
#endif
}