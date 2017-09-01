#pragma once

#include "Iterator.hpp"
#include "LinkNode.hpp"

#define _DEBUGFORWARDITERATOR _DEBUG && 1

namespace cg {

/**A one directional forward iterator.*/
template<typename T, typename NodeTypeT, bool Const = false>
class ForwardIterator : public Iterator<T, Const>
{
public:
	/**The type of node to use.*/
	using NodeType = NodeTypeT;
	/**The same type as this object.*/
	using self_type = ForwardIterator<T, NodeTypeT, Const>;
	/**The same type as this object.*/
	using SelfType = self_type;
	/**An iterator tag to make this work with std stuff.*/
	using iterator_category = std::forward_iterator_tag;
	/**An iterator tag to make this work with std stuff.*/
	using IteratorCatagory = iterator_category;
	/**default ctor*/
	ForwardIterator() :m_node(nullptr) {};
	/**Construct into a chain.
	\param node The pointer to the active node.*/
	ForwardIterator(NodeType* node) {
		m_node = node;
	}
	/**Copy asignment contructor.
	\param other The other thing to copy.*/
	ForwardIterator(const self_type& other)
		:m_node(other.m_node) {}
	/**Move asignment contructor.
	\param other The other thing to move.*/
	ForwardIterator(self_type&& other)
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
private:

#if _DEBUGFORWARDITERATOR
	/**Throw if trying to move past an cap iterator.*/
	void D_CheckBounds()const;
	/**Throw an error if the poitner is not dereferencable.*/
	void D_CheckValid() const;
#endif
	/**A pointer to the node for this iterator.*/
	NodeType* m_node;
};

template<typename T, typename NodeTypeT, bool Const>
inline typename ForwardIterator<T, NodeTypeT, Const>::self_type &
ForwardIterator<T, NodeTypeT, Const>::operator++()
{
#if _DEBUGFORWARDITERATOR
	D_CheckBounds();
#endif

	NodeType::AdvanceNext(&m_node);
	return *this;
}
template<typename T, typename NodeTypeT, bool Const>
inline typename ForwardIterator<T, NodeTypeT, Const>::self_type
ForwardIterator<T, NodeTypeT, Const>::operator++(int nothing)
{
	auto copy = *this;
	++(*this);
	return copy;
}

template<typename T, typename NodeTypeT, bool Const>
inline typename ForwardIterator<T, NodeTypeT, Const>::PointerReturnType
ForwardIterator<T, NodeTypeT, Const>::operator->()
{
#if _DEBUGFORWARDITERATOR
	D_CheckValid();
#endif
	return NodeType::Get(m_node);
}

template<typename T, typename NodeTypeT, bool Const>
inline bool ForwardIterator<T, NodeTypeT, Const>::
operator==(const self_type & rhs) const {
	auto a = m_node->m_ptr == rhs.m_node->m_ptr;
	auto b = m_node->m_next == rhs.m_node->m_next;
	return a && b;
}

template<typename T, typename NodeTypeT, bool Const>
inline void ForwardIterator<T, NodeTypeT, Const>::PrintChain(std::streambuf * strm)
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
#if _DEBUGFORWARDITERATOR
template<typename T, typename NodeTypeT, bool Const>
inline void ForwardIterator<T, NodeTypeT, Const>::D_CheckBounds() const
{
	m_node->D_CheckBounds();
}

template<typename T, typename NodeTypeT, bool Const>
inline void ForwardIterator<T, NodeTypeT, Const>::D_CheckValid() const
{
	m_node->D_CheckValid();
}
#endif

#if _DEBUGBIDIRECTIONITERATOR
template class ForwardIterator<int, cg::LinkNode<int>, false>;
template class ForwardIterator<int, cg::LinkNode<int>, true>;
#endif

}