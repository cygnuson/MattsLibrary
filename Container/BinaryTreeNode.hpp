#pragma once

#include "Node.hpp"

#define _DEBUGBINARYTREENODE _DEBUG & 1

namespace cg {


/**
\tparam T A type that is valid for use with operator<*/
template<typename T>
class BinaryTreeNode : public cg::Node<T>
{
public:
	/**The self-type of the tree.*/
	using self_type = BinaryTreeNode<T>;
	/**The self-type of the tree.*/
	using SelfType = self_type;
	/**Empty ctor.*/
	BinaryTreeNode() :m_ptr(nullptr) {};
	/**Create a node.
	\param ptr A pointer to the data.*/
	BinaryTreeNode(Pointer ptr);
	/**A function that will determine the size of a branch.  A single node with
	no nodes conencted to it is of size 1.
	\param node The node.
	\return The size of the tree started with `node`.*/
	static std::size_t SizeOf(SelfType* node);
	/**Tie a node to another node.  The type T for the class must have
	operator< defined for it.
	\param node A poitner to a node pointer. If `node` is not set (is null) it
	will become the new node.
	\param node the node that will be tied.
	\return A pointer to the parent of the inserted node, or nullptr if `node`
	became the new node.*/
	static SelfType* Insert(SelfType** node, SelfType* other);
	/**Pointer member access.
	\return A pointer to the node.*/
	Pointer operator->();
	/**Dereference the node.
	\return A reference to the underlying data.*/
	Reference operator*() {
		return *(this->operator->());
	}

	/**The data.*/
	Pointer m_ptr;
	/**The left node.*/
	SelfType* m_left;
	/**The right node.*/
	SelfType* m_right;
#if _DEBUGBINARYTREENODE
	SelfType* md_parent = nullptr;
#endif
};

template<typename T>
inline BinaryTreeNode<T>::BinaryTreeNode(Pointer ptr)
	:m_ptr(ptr)
{

}

template<typename T>
inline std::size_t BinaryTreeNode<T>::SizeOf(SelfType * node)
{
	if (node)
	{
		std::size_t ct = 1;
		ct += SizeOf(node->m_left);
		ct += SizeOf(node->m_right);
		return ct;
	}
	else
		return (std::size_t) 0;
}

template<typename T>
inline typename BinaryTreeNode<T>::SelfType*
BinaryTreeNode<T>::Insert(SelfType ** nodep,
	SelfType * other)
{
	SelfType* node = *nodep;
	if (!node)
	{
		/*make the attach pointer node into the new node because it itself
		is null.*/
		*nodep = other;
		return nullptr;
	}
	SelfType* parent = nullptr;
	if (**other < **node)
		parent = Insert(&(node->m_left), other);
	else if (**other == **node)
		return parent;
	else
		parent = Insert(&(node->m_right), other);

	if (!parent)
		/*if the parent is null, that means that a left or right was set.*/
		return *nodep;
	else
		return parent;
}

template<typename T>
inline typename BinaryTreeNode<T>::Pointer
BinaryTreeNode<T>::operator->()
{
	return m_ptr;
}



#if _DEBUGBINARYTREENODE
template class cg::BinaryTreeNode<int>;
#endif
}
