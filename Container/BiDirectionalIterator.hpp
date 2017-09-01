#pragma once

#include "Iterator.hpp"
#include "DoubleLinkNode.hpp"

#define _DEBUGBIDIRECTIONITERATOR _DEBUG && 1

namespace cg {


template<typename T, typename NodeTypeT, bool Const = false, 
	bool Reverse = false>
class BiDirectionalIterator : public Iterator<T, Const>
{
public:
	/**The type of node to use.*/
	using NodeType = NodeTypeT;
	/**The same type as this object.*/
	using self_type = BiDirectionalIterator<T, NodeTypeT, Const, Reverse>;
	/**The same type as this object.*/
	using SelfType = self_type;
	/**An iterator tag to make this work with std stuff.*/
	using iterator_category = std::bidirectional_iterator_tag;
	/**An iterator tag to make this work with std stuff.*/
	using IteratorCategory = iterator_category;
	/**default ctor*/
	BiDirectionalIterator() :m_node(nullptr) {};
	/**Construct into a chain.
	\param node The pointer to the active node.*/
	BiDirectionalIterator(NodeType* node) {
		m_node = node;
	}
	/**Copy asignment contructor.
	\param other The other thing to copy.*/
	BiDirectionalIterator(const self_type& other)
		:m_node(other.m_node) {}
	/**Move asignment contructor.
	\param other The other thing to move.*/
	BiDirectionalIterator(self_type&& other)
		:m_node(std::move(other.m_node)) {}
	/**Copy assignment operator.
	\param other The other iterator.*/
	inline void operator=(const self_type& other) {
		m_node = other.m_node;
	}
	/**Move assignment operator.
	\param other The other iterator.*/
	inline void operator=(self_type&& other) {
		m_node = std::move(other.m_node);
	}
	/**Increment the poitner by 1. (prefix)
	\return This same type.*/
	inline self_type& operator++();
	/**Increment this by 1. (postfix)
	\param nothing Ignored pointer.
	\return a copy of this befor incrementing.*/
	inline self_type operator++(int nothing);
	/**Decrement the poitner by 1. (prefix)
	\return This same type after incrementing.*/
	inline self_type& operator--();
	/**Decrement this by 1. (postfix)
	\param nothing Ignored pointer.
	\return a copy of this before decrementing.*/
	inline self_type operator--(int nothing);
	/**Dereference operator.
	\return A reference to the data contained.*/
	inline RefReturnType operator*()
	{
		return *(this->operator->());
	}
	/**Poitner member access.
	\return A pointer to this (-> is appolied to the pointer too).*/
	inline PointerReturnType operator->();
	/**Compare for equality.
	\return True if the iterators are the same.*/
	inline bool operator==(const self_type& rhs) const;
	/**Compare for equality.
	\return False if the iterators are the same.*/
	inline bool operator!=(const self_type& rhs) const {
		return !(*this == rhs);
	}
	/**Print the chain from the next link to the last link. The best way to do
	it is to call print on the beforeBegin iterator.
	\param strm A stream buffer to print to.*/
	void PrintChain(std::streambuf* strm);
	/**Print the entire chain from the previous link to the first link. The
	best way to do it is to call print on the afterEnd iterator.
	\param strm A stream buffer to print to.*/
	void PrintChainReverse(std::streambuf* strm);
private:

#if _DEBUGBIDIRECTIONITERATOR
	/**Throw if trying to move past an cap iterator.*/
	void D_CheckBounds()const;
	/**Throw an error if the poitner is not dereferencable.*/
	void D_CheckValid() const;
#endif
	/**A pointer to the node for this iterator.*/
	NodeType* m_node;
};


template<typename T, typename NodeTypeT, bool Const, bool Reverse>
inline void BiDirectionalIterator<T, NodeTypeT, Const, Reverse>::
PrintChain(std::streambuf * strm)
{
	std::ostream out(strm);
	std::size_t index = 0;
	for (NodeType* n = m_node->m_next; n != nullptr; n = n->m_next)
	{
		if (!n->m_ptr)
		{
			out << std::endl;
			break;
		}
		out << "\n" << "[" << ++index << "]= " << *n->m_ptr;
	}
	out << std::endl;
}

template<typename T, typename NodeTypeT, bool Const, bool Reverse>
inline void
BiDirectionalIterator<T, NodeTypeT, Const, Reverse>::PrintChainReverse(
	std::streambuf * strm)
{
	std::ostream out(strm);
	std::size_t index = 0;
	for (NodeType* n = m_node->m_prev; n != nullptr; n = n->m_prev)
	{
		if (!n->m_ptr)
		{
			out << std::endl;
			break;
		}
		out << "\n" << "[" << ++index << "]= " << *n->m_ptr;
	}
	out << std::endl;
}

template<typename T, typename NodeTypeT, bool Const, bool Reverse>
inline typename
BiDirectionalIterator<T, NodeTypeT, Const, Reverse>::self_type &
BiDirectionalIterator<T, NodeTypeT, Const, Reverse>::operator++()
{
#if _DEBUGBIDIRECTIONITERATOR
	D_CheckBounds();
#endif
	m_node = m_node->m_next;
	return *this;
}

template<typename T, typename NodeTypeT, bool Const, bool Reverse>
inline typename BiDirectionalIterator<T, NodeTypeT, Const, Reverse>::self_type
BiDirectionalIterator<T, NodeTypeT, Const, Reverse>::operator++(int nothing)
{
	auto copy = *this;
	++(*this);
	return copy;
}

template<typename T, typename NodeTypeT, bool Const, bool Reverse>
inline typename 
BiDirectionalIterator<T, NodeTypeT, Const, Reverse>::self_type &
BiDirectionalIterator<T, NodeTypeT, Const, Reverse>::operator--()
{
#if _DEBUGBIDIRECTIONITERATOR
	D_CheckBounds();
#endif
	NodeType::AdvancePrev(&m_node);
	return *this;
}

template<typename T, typename NodeTypeT, bool Const, bool Reverse>
inline typename BiDirectionalIterator<T, NodeTypeT, Const, Reverse>::self_type
BiDirectionalIterator<T, NodeTypeT, Const, Reverse>::operator--(int nothing)
{
	auto copy = *this;
	--(*this);
	return copy;
}

template<typename T, typename NodeTypeT, bool Const, bool Reverse>
inline typename 
BiDirectionalIterator<T, NodeTypeT, Const, Reverse>::PointerReturnType
BiDirectionalIterator<T, NodeTypeT, Const, Reverse>::operator->()
{
#if _DEBUGBIDIRECTIONITERATOR
	D_CheckValid();
#endif
	return NodeType::Get(m_node);
}

template<typename T, typename NodeTypeT, bool Const, bool Reverse>
inline bool BiDirectionalIterator<T, NodeTypeT, Const, Reverse>::
operator==(const self_type & rhs) const {
	return NodeType::Equal(m_node, rhs.m_node);
}

#if _DEBUGBIDIRECTIONITERATOR
template<typename T, typename NodeTypeT, bool Const, bool Reverse>
inline void 
BiDirectionalIterator<T, NodeTypeT, Const, Reverse>::D_CheckBounds() const
{
	m_node->D_CheckBounds();
}
template<typename T, typename NodeTypeT, bool Const, bool Reverse>
inline void 
BiDirectionalIterator<T, NodeTypeT, Const, Reverse>::D_CheckValid() const
{
	m_node->D_CheckValid();
}
#endif


#if _DEBUGBIDIRECTIONITERATOR
template class BiDirectionalIterator<int, cg::DoubleLinkNode<int>, false>;
template class BiDirectionalIterator<int, cg::DoubleLinkNode<int>, true>;
#endif

/*****************************************************************************/

}
