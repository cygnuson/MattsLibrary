#pragma once

#include "Iterator.hpp"
#include "BinaryTreeNode.hpp"

#define _DEBUGTREEITERATOR _DEBUG && _DEBUGBINARYTREENODE && 1

namespace cg {

template<typename T, typename NodeTypeT, bool Const = false,
	bool Reverse = false>
	class TreeIterator : public Iterator<T, Const>
{
public:
	/**The type of node to use.*/
	using NodeType = NodeTypeT;
	/**The same type as this object.*/
	using self_type = TreeIterator<T, NodeTypeT, Const, Reverse>;
	/**The same type as this object.*/
	using SelfType = self_type;
	/**An iterator tag to make this work with std stuff.*/
	using iterator_category = std::forward_iterator_tag;
	/**An iterator tag to make this work with std stuff.*/
	using IteratorCatagory = iterator_category;
	/**default ctor*/
	TreeIterator() :m_node(nullptr), m_root(nullptr) {};
	/**Construct into a chain.
	\param node The node for this iterator.
	\param root the root node owning this node.*/
	TreeIterator(NodeType* node, NodeType** root)
		:m_node(node), m_root(root) {}
	/**Construct into a chain.
	\param node The node for this iterator.
	\param root the root node owning this node.*/
	TreeIterator(NodeType* node, NodeType*const* root)
		:m_node(node), m_root(root) {}
	/**Copy asignment contructor.
	\param other The other thing to copy.*/
	TreeIterator(const self_type& other)
		:m_node(other.m_node), m_root(other.m_root) {}
	/**Move asignment contructor.
	\param other The other thing to move.*/
	TreeIterator(self_type&& other)
		:m_node(std::move(other.m_node)),
		m_root(std::move(other.m_root)) {}
	/**Copy assignment operator.
	\param other The other iterator.*/
	inline void operator=(const self_type& other) {
		m_node = other.m_node;
		m_root = other.m_root;
	}
	/**Move assignment operator.
	\param other The other iterator.*/
	inline void operator=(self_type&& other) {
		m_node = std::move(other.m_node);
		m_root = std::move(other.m_root);
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
private:
	/**A pointer to the node for this iterator.*/
	NodeType* m_node;
	/**A pointer to a root node pointer for searching.*/
	NodeType*const* m_root;

};

template<typename T, typename NodeTypeT, bool Const, bool Reverse>
inline typename TreeIterator<T, NodeTypeT, Const, Reverse>::self_type &
TreeIterator<T, NodeTypeT, Const, Reverse>::operator++()
{
	if (!Reverse)
		NodeType::AdvanceNext(&m_node, *m_root);
	else
		NodeType::AdvancePrev(&m_node, *m_root);
	return *this;
}

template<typename T, typename NodeTypeT, bool Const, bool Reverse>
inline typename TreeIterator<T, NodeTypeT, Const, Reverse>::self_type
TreeIterator<T, NodeTypeT, Const, Reverse>::operator++(int nothing)
{
	auto copy = *this;
	++(*this);
	return copy;
}

template<typename T, typename NodeTypeT, bool Const, bool Reverse>
inline typename TreeIterator<T, NodeTypeT, Const, Reverse>::self_type &
TreeIterator<T, NodeTypeT, Const, Reverse>::operator--()
{
	if (Reverse)
		NodeType::AdvanceNext(&m_node, *m_root);
	else
		NodeType::AdvancePrev(&m_node, *m_root);
	return *this;
}

template<typename T, typename NodeTypeT, bool Const, bool Reverse>
inline typename TreeIterator<T, NodeTypeT, Const, Reverse>::self_type 
TreeIterator<T, NodeTypeT, Const, Reverse>::operator--(int nothing)
{
	auto copy = *this;
	--(*this);
	return copy;
}

template<typename T, typename NodeTypeT, bool Const, bool Reverse>
inline typename TreeIterator<T, NodeTypeT, Const, Reverse>::PointerReturnType
TreeIterator<T, NodeTypeT, Const, Reverse>::operator->()
{
	if (NodeType::IsBeforeBegin(m_node) || NodeType::IsAfterEnd(m_node))
		throw OutOfBoundsException();
	return m_node->Get();
}

template<typename T, typename NodeTypeT, bool Const, bool Reverse>
inline bool TreeIterator<T, NodeTypeT, Const, Reverse>::operator==(
	const self_type & rhs) const
{
	return m_node->Hash() == rhs.m_node->Hash();
}

#if _DEBUGTREEITERATOR
template class TreeIterator<int, cg::BinaryTreeNode<int>, true, false>;
template class TreeIterator<int, cg::BinaryTreeNode<int>, false, false>;
#endif
}