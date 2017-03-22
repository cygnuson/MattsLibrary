#pragma once

#include <cassert>

#include "Node.hpp"

#define _DEBUGBINARYTREENODE _DEBUG & 1

namespace cg {


/** This node will clean up its data when it destroyes.  It will own any
pointers sent to it.
\tparam T A type that is valid for use with operator<*/
template<typename T>
class BinaryTreeNode : public cg::Node<T>
{
public:
	/**An exception to be thrown if null pointers are encountered.*/
	struct NullPtrException : public cg::Exception
	{
		/**Deliver the message.*/
		virtual std::string ToString() const {
			return "The pointer is null.";
		}
	};
	/**An exception to be thrown if a branch is occupied but expected to be
	unoccupied.*/
	struct InvalidNodeException : public cg::Exception
	{
		/**Deliver the message.*/
		virtual std::string ToString() const {
			return "The node parameter is invalid.";
		}
	};
	/**An exception to be thrown if a branch is occupied but expected to be
	unoccupied.*/
	struct NodeDoesNotExistException : public cg::Exception
	{
		/**Deliver the message.*/
		virtual std::string ToString() const {
			return "The node does not exist.";
		}
	};
	/**The self-type of the tree.*/
	using self_type = BinaryTreeNode<T>;
	/**The self-type of the tree.*/
	using SelfType = self_type;
	/**Empty ctor.*/
	BinaryTreeNode() :m_ptr(nullptr) {};
	/**Create a node.
	\param ptr A pointer to the data.*/
	BinaryTreeNode(Pointer ptr);
	/**deleting dtor.*/
	~BinaryTreeNode();
	/**A function that will determine the size of a branch.  A single node with
	no nodes conencted to it is of size 1.
	\param node The node.
	\return The size of the tree started with `node`.*/
	static std::size_t SizeOf(SelfType* node);
	/**Balance the tree.  Balancing will start at the lowest nodes and work its
	way up.
	\param node A root node to balanace on.  It may change based on how the
	tree moves around, but will always point to the root node.
	*/
	static void BalanceImpl(SelfType** node);
	/**Balance the tree.  Balancing will start at the lowest nodes and work its
	way up.
	\param node A root node to balanace on.  It may change based on how the
	tree moves around, but will always point to the root node.
	*/
	static void Balance(SelfType** node);
	/**Rotate untill each side of the node are within 1 depth value of each.
	\param node The node for which to rotate.  This parameter will become be
	shifted around and when the function is complete, the *node may point to
	something else, which will be the new root node in the position.*/
	static void BalanceOnce(SelfType** node);
	/**Tie a node to another node.  The type T for the class must have
	operator< defined for it.
	\param node A poitner to a node pointer. If `node` is not set (is null) it
	will become the new node.
	\param other the node that will be tied.
	\return True if `other` was attached a node.*/
	static bool Insert(SelfType** node, SelfType* other);
	/**Remove a node from a tree. The node is removed, but NOT deleted or
	deallocated in any way. It is ONLY removed from the tree chain.
	\param node A pointer to the root node to start the search from. If the
	`node` poitner is the node to be removed, it shall be.
	\param vic The node to be removed.
	\return nullptr is returned if nothing is removed. If anyhting is removed,
	a pointer to the root node is returned. The pointer to the root node may
	be different then when the function started if the root node is the one
	that was removed.  In this context, the `root` node is the node the is
	passed as the parameter `node`.*/
	static SelfType* Remove(SelfType** node, SelfType* vic);
	/**Determine the depth of a tree branch.
	\param node The start of the brach to check.
	\return The amount of node levels that exist below `node`.*/
	static std::size_t Depth(SelfType* node);
	/**Replace a node with another node.
	\param nodep A pointer to a pointer to a node that will be replaced.
	\param node A pointer to a node that will be put to nodep.*/
	static void ReplaceNode(SelfType** nodep, SelfType* node);
	/**Get the parent and grand parent of a node.
	\param root The root node to search under.
	\param node The node to get the info for.
	\return A pointer to the nodes parent, or nullptr if the node does not
	exist under `root`.  A nullptr return may mean that root and node are the
	same.
	\throw Throws InvalidNodeException if node == *root .*/
	static SelfType* GetParent(SelfType** root, SelfType* node,
		bool fresh = true);
	/**Determine the amount of imbalance for a node.
	\return The amount of space between the longest branch and the shortest
	branch.*/
	static std::size_t Imbalance(SelfType* node);
	/**Find a value from downwards of a node.  The type of data stored in the
	node must have == and <
	\param node The node to start the seach with.
	\param value The value to find.
	\return A pointer to the node that contains that value, or nullptr if the
	node and its children do not container that value.*/
	static SelfType* Find(SelfType* node, ConstReference value);
	/**Advance a node with respect to a root node.
	\param node A pointer to a pointer to the node to advance.
	\param root A pointer to a node that will act as as the root node for the
	advancement.*/
	static void AdvanceNext(SelfType** node, SelfType* root);
	/**Advance a node backwards with respect to a root node.
	\param node A pointer to a pointer to the node to advance.
	\param root A pointer to a node that will act as as the root node for the
	advancement.*/
	static void AdvancePrev(SelfType** node, SelfType* root);
	/**Get a BeforeBegin node.
	\param root The root node that this beforBegin node will lead to.
	\return A node specially configured for beforeBegin.*/
	static SelfType* BeforeBegin(SelfType* root);
	/**Determine if a node is a before begin node.
	\param node The node to check.
	\return True if the node is a beforeBegin node.*/
	static bool IsBeforeBegin(SelfType* node);
	/**Determine if a node is an after end node.
	\param node The node to check.
	\return True if the node is an after end node.*/
	static bool IsAfterEnd(SelfType* node);
	/**Get a special after end node.
	\param root The root node that this after end node will lead to.
	\return An node that signifies after end.*/
	static SelfType* AfterEnd(SelfType* node);
	/**Pointer member access.
	\return A pointer to the node.*/
	Pointer operator->();
	/**Dereference the node.
	\return A reference to the underlying data.*/
	Reference operator*() {
		return *(this->operator->());
	}
	/**Get the pointer for this node.
	\return The pointer contained within.*/
	Pointer Get();
	/**Hash this node.
	\return The hash.*/
	std::size_t Hash() const;
	/**The data.*/
	Pointer m_ptr;
	/**The left node.*/
	SelfType* m_left;
	/**The right node.*/
	SelfType* m_right;
#if _DEBUGBINARYTREENODE
	SelfType* md_parent = nullptr;
	const SelfType* md_self = nullptr;
#endif
};


template<typename T>
inline BinaryTreeNode<T>::BinaryTreeNode(Pointer ptr)
	:m_ptr(ptr)
{
}

template<typename T>
inline BinaryTreeNode<T>::~BinaryTreeNode()
{
	cg::Delete(m_ptr);
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
inline void BinaryTreeNode<T>::BalanceImpl(SelfType ** nodep)
{
	if ((*nodep)->m_left)
		/*balance the left branches.*/
		BalanceImpl(&(*nodep)->m_left);
	if ((*nodep)->m_right)
		/*balance the right branches.*/
		BalanceImpl(&(*nodep)->m_right);
	/*balance the root node.*/
	while (Imbalance(*nodep) > 1)
	{
		BalanceOnce(nodep);
	}
}

template<typename T>
inline void BinaryTreeNode<T>::Balance(SelfType ** nodep)
{
	if ((*nodep)->m_left)
		/*balance the left branches.*/
		Balance(&(*nodep)->m_left);
	if ((*nodep)->m_right)
		/*balance the right branches.*/
		Balance(&(*nodep)->m_right);
	/*balance the root node.*/
	auto imbal = Imbalance(*nodep);
	while (1)
	{
		BalanceImpl(nodep);
		auto imbal2 = Imbalance(*nodep);
		auto newImbal = imbal - imbal2;
		if (newImbal == 0)
		{
			break;
		}
		else
		{
			imbal = imbal2;
		}
	}
}

template<typename T>
inline void BinaryTreeNode<T>::BalanceOnce(SelfType** node)
{
	auto imbalance = Imbalance(*node);
	if (imbalance > 1)
	{
		auto recycle = *node;
		auto nr = Remove(node, *node);
		if (nr)
			ReplaceNode(node, nr);
		Insert(node, recycle);
	}
}

template<typename T>
inline bool BinaryTreeNode<T>::Insert(SelfType ** nodep,
	SelfType * other)
{
#if _DEBUGBINARYTREENODE
	static SelfType* parent = nullptr;
#endif
	if ((*nodep) == nullptr)
	{
		*nodep = other;
#if _DEBUGBINARYTREENODE
		(*nodep)->md_parent = parent;
		bool a = (*nodep)->md_parent->m_left == other;
		bool b = (*nodep)->md_parent->m_right == other;
		assert(a | b);
#endif
		return true;
	}
	if ((*nodep) == other)
		throw InvalidNodeException();

	bool attached = false;
	if (**other < **(*nodep))
		/*go left*/
	{
#if _DEBUGBINARYTREENODE
		parent = *nodep;
#endif
		attached = Insert(&(*nodep)->m_left, other);
	}
	else if (**other > **(*nodep))
		/*go right*/
	{
#if _DEBUGBINARYTREENODE
		parent = *nodep;
#endif
		attached = Insert(&(*nodep)->m_right, other);
	}
	else
		/*value is equal, do nothing.*/
		attached = false;

	if (attached)
		/*We attached a single node, balance only once.*/
		BalanceOnce(nodep);
	return attached;
}

template<typename T>
inline typename BinaryTreeNode<T>::SelfType*
BinaryTreeNode<T>::Remove(SelfType ** nodep, SelfType * vic)
{
	SelfType* parent = GetParent(nodep, vic);
	if (!parent)
		if (*nodep == vic)
			/*trying to remove root node.*/
		{
			/*step 1: save the children.*/
			auto vL = vic->m_left;
			auto vR = vic->m_right;

			/*step 2: get the depth of each child.*/
			auto lD = Depth(vL);
			auto rD = Depth(vR);

			/*3: clean the vic*/
			vic->m_left = nullptr;
			vic->m_right = nullptr;

			/*step 4: save the opposite child of the new root and remove it.*/
			if (lD > rD)
				/*will use vL for the new node.*/
			{
				SelfType* recycle = nullptr;
				if (vL->m_right && vR)
				{
					/*remove the rightside node from the other child.*/
					recycle = vL->m_right;
					Remove(&vL, vL->m_right);
					/*attach the other child to vL (the new root).*/
					Insert(&vL, vR);
				}
				if (!vL->m_right && vR)
				{
					/*attach vR directly to vL->m_right*/
					Insert(&vL, vR);
				}
				if (recycle)
					Insert(&vL, recycle);
				return vL;
			}
			else
				/*will use vR for the new root.*/
			{
				SelfType* recycle = nullptr;
				if (vR->m_left && vL)
				{
					/*recycle the left node of the right child.*/
					recycle = vR->m_left;
					Remove(&vR, vR->m_left);
					/*attach the other child to vR (the new root).*/
					Insert(&vR, vL);
				}
				if (!vR->m_left && vL)
				{
					/*attach vL directly to vR->m_left*/
					Insert(&vR, vL);
				}
				if (recycle)
					Insert(&vR, recycle);
				return vR;
			}
		}
		else
			/*not exist*/
			return nullptr;

	/*step 1: save the children*/
	auto vL2 = vic->m_left;
	auto vR2 = vic->m_right;

	/*2: clean the vic*/
	vic->m_left = nullptr;
	vic->m_right = nullptr;

	/*3: disconnect the vic from its parent.*/
	if (parent->m_left == vic)
		parent->m_left = nullptr;
	else
		parent->m_right = nullptr;

	/*4: Insert the children back into the tree at the nodep supplied root.*/
	if (vL2)
		Insert(nodep, vL2);
	if (vR2)
		Insert(nodep, vR2);

	return nullptr;
}

template<typename T>
inline std::size_t BinaryTreeNode<T>::Depth(SelfType * node)
{
	if (!node)
		return 0;

	std::size_t r = 0;
	std::size_t l = 0;
	if (!node->m_left && !node->m_right)
		/*will get a one on the bounce back*/
		return 0;

	if (node->m_right)
		r += Depth(node->m_right) + 1;
	if (node->m_left)
		l += Depth(node->m_left) + 1;
	return (r > l ? r : l);
}

template<typename T>
inline void BinaryTreeNode<T>::ReplaceNode(SelfType ** nodep,
	SelfType * node)
{
#if _DEBUGBINARYTREENODE
	/*make sure the parent of the old node is transfered over.*/
	node->md_parent = (*nodep)->md_parent;
#endif
	(*nodep) = node;
}

template<typename T>
inline typename BinaryTreeNode<T>::SelfType *
BinaryTreeNode<T>::GetParent(SelfType ** rootp,
	SelfType * node, bool fresh)
{
	/*step 1: if the root node is set as null, return null.*/
	if (*rootp == nullptr)
		return nullptr;
	static SelfType* parent = nullptr;
	if (fresh)
		/*if we want a fresh set of rucursion on the call.*/
		parent = nullptr;
	SelfType* root = *rootp;
	/*step 2: if the root node and the node to remove are the same, its found
	so return the parent.*/
	if (root == node)
	{
		return parent;
	}
	/*step 3: check if node is < root, if so go down left branch.*/
	if (**node < **root)
		if (root->m_left)
		{
			parent = root;
			return GetParent(&root->m_left, node, false);
		}
		else
			return nullptr;
	/*step 3: check if node is > root, if so go down right branch.*/
	else if (**node > **root)
		if (root->m_right)
		{
			parent = root;
			return GetParent(&root->m_right, node, false);
		}
		else
			return nullptr;
	/*if root and node are euqal in value, check for a duplicate or if its
	the right node.*/
	else
		/*node is qual to root here.*/
		if (root == node)
			return parent;
		else
			/*same value, but not the same node. so node does not exist.*/
			return nullptr;
}

template<typename T>
inline std::size_t BinaryTreeNode<T>::Imbalance(SelfType * node)
{
	std::size_t lDepth = 0;
	if (node->m_left)
		lDepth = Depth(node->m_left) + 1;
	std::size_t rDepth = 0;
	if (node->m_right)
		rDepth = Depth(node->m_right) + 1;
	auto& larger = lDepth > rDepth ? lDepth : rDepth;
	auto& smaller = lDepth > rDepth ? rDepth : lDepth;
	return (larger - smaller);
}

template<typename T>
inline typename BinaryTreeNode<T>::SelfType *
BinaryTreeNode<T>::Find(SelfType * node, ConstReference value)
{
	if (!node)
		return nullptr;
	if (value > **node)
		return Find(node->m_right, value);
	else if (value < **node)
		return Find(node->m_left, value);
	else
		return node;
}

template<typename T>
inline void BinaryTreeNode<T>::AdvanceNext(SelfType ** node, SelfType * root)
{
	if (IsBeforeBegin(*node))
		/*if is before begin, advance to the right node of the iterator which
		is the starting point for the tree iterators.*/
	{
		(*node) = (*node)->m_right;
		return;
	}
	if (IsAfterEnd(*node))
		/*throw is trying to incriment an after end.*/
	{
		throw OutOfBoundsException();
	}
	/**Keep track on if the right node needs done or not.*/
	static bool doRight = true;
	if (*node == root)
		/*this is the root node. the next node is the leftmost node of the
		right branch.*/
	{
		/*if !doRight and we are root, then we have traversed the highest
		value node. Time to set the iterator to be the after end iterator.*/
		if (!doRight || !(*node)->m_right)
		{
			*node = AfterEnd(root);
			return;
		}
		else
			/*move to the right branch and go as left as we can.*/
		{
			*node = (*node)->m_right;
			while (1)
			{
				if ((*node)->m_left)
					*node = (*node)->m_left;
				else
					break;
			}
		}
	}
	else
	{
		/*if there is a right child, go to it.*/
		if ((*node)->m_right && doRight)
		{
			*node = (*node)->m_right;
			/*go as left as we can*/
			while (1)
			{
				if ((*node)->m_left)
					*node = (*node)->m_left;
				else
					break;
			}
		}
		else
		{
			/*get the nodes parent.*/
			SelfType* parent = GetParent(&root, *node);
			/*check if this is the right child of my parent*/
			bool iAmRightChild = parent->m_right == *node;
			if (iAmRightChild)
				/*use the parent as the new node.*/
			{
				doRight = false;
				*node = parent;
				AdvanceNext(node, root);
				doRight = true;
			}
			else
				/*this is the left child, so change to parent.*/
			{
				*node = parent;
				doRight = true;
			}
		}
	}
}

template<typename T>
inline void BinaryTreeNode<T>::AdvancePrev(SelfType ** node, SelfType * root)
{
	/**a static flag for direction controll with recursion down below.*/
	static bool doLeft = true;
	/*if this is the before begin node, we cannot advance further, so throw*/
	if (IsBeforeBegin(*node))
		throw OutOfBoundsException();
	/*if this is the after end iterator, increment to the m_left.*/
	else if (IsAfterEnd(*node))
		(*node) = (*node)->m_left;
	/*if this is the root node, we move to the largest value on the left.*/
	else if (*node == root)
	{
		/*if doLeft is false or there is no left node, and we arrived at the 
		root node its the end of the line.*/
		if (!doLeft || !(*node)->m_left)
			*node = BeforeBegin(root);
		else
		{
			*node = (*node)->m_left;
			while (1)
			{
				if ((*node)->m_right)
					*node = (*node)->m_right;
				else
					break;
			}
			doLeft = true;
		}
	}
	else
	{
		/*if I have a left child, and doLeft is true, move to the left child
		and go as right as I can.*/
		if ((*node)->m_left && doLeft)
		{
			*node = (*node)->m_left;
			while (1)
			{
				if ((*node)->m_right)
					*node = (*node)->m_right;
				else
					break;
			}
		}

		/*If i dont have aleft child or doLeft is false*/
		else
		{
			SelfType* parent = GetParent(&root, *node);
			bool iAmLeftChild = parent->m_left == *node;
			/*if I am a left child, i will goto my parent and set doLeft to no
			and try to advance again.*/
			if (iAmLeftChild)
			{
				doLeft = false;
				*node = parent;
				AdvancePrev(node,root);
				doLeft = true;
			}
			/*if i am a right child, i will go to my parent.*/
			else
			{
				*node = parent;
				doLeft = true;
			}
		}
	}
}

template<typename T>
inline  typename BinaryTreeNode<T>::SelfType*
BinaryTreeNode<T>::BeforeBegin(SelfType * root)
{
	auto ret = cg::New<SelfType>();
	ret->m_left = nullptr;
	ret->m_ptr = nullptr;
	auto node = root;
	while (1)
		/*Set the m_right ptr to the left most node of the root.*/
	{
		auto tNode = node->m_left;
		if (tNode)
			node = tNode;
		else
			break;
	}
	ret->m_right = node;
	return ret;
}

template<typename T>
inline bool BinaryTreeNode<T>::IsBeforeBegin(SelfType* node)
{
	return node->m_left == nullptr
		&& node->m_right != nullptr
		&& node->m_ptr == nullptr;
}

template<typename T>
inline bool BinaryTreeNode<T>::IsAfterEnd(SelfType* node)
{
	return node->m_left != nullptr
		&& node->m_right == nullptr
		&& node->m_ptr == nullptr;
}

template<typename T>
inline  typename BinaryTreeNode<T>::SelfType*
BinaryTreeNode<T>::AfterEnd(SelfType* root)
{
	auto ret = cg::New<SelfType>();
	ret->m_ptr = nullptr;
	ret->m_right = nullptr;
	auto node = root;
	while (1)
		/*Set the m_right ptr to the left most node of the root.*/
	{
		auto tNode = node->m_right;
		if (tNode)
			node = tNode;
		else
			break;
	}
	ret->m_left = node;
	return ret;
}

template<typename T>
inline typename BinaryTreeNode<T>::Pointer
BinaryTreeNode<T>::operator->()
{
#if _DEBUGBINARYTREENODE
	md_self = this;
#endif
	return m_ptr;
}

template<typename T>
inline typename BinaryTreeNode<T>::Pointer
BinaryTreeNode<T>::Get()
{
	return m_ptr;
}

template<typename T>
inline std::size_t BinaryTreeNode<T>::Hash() const
{
	{
		std::size_t hash = 59;
		if (m_ptr)
			hash *= *m_ptr;
		if (m_left)
			hash *= m_left->Hash();
		if (m_right)
			hash *= m_right->Hash();
		return hash;
	}
}

}

#if _DEBUGBINARYTREENODE
template class cg::BinaryTreeNode<int>;
#endif



