#pragma once

#include "Node.hpp"

#define _DEBUGDOUBLELINKNODE _DEBUG && 1

namespace cg {


/**A Node type that will exist under the hood.*/
template<typename T>
struct DoubleLinkNode : public Node<T> {
	/**This type.*/
	using self_type = DoubleLinkNode<T>;
	/**This type.*/
	using SelfType = self_type;
	/**Create an initial pair of nodes.  These nodes will always be the
	first and lost nodes in a chain.
	\param beforeBegin The very first node.
	\param afterEnd The last node.*/
	static void CreateLinkedEnds(self_type& beforeBegin, self_type& afterEnd);
	/**Delete the data pointers. If before_begin node is deleted,
	than the whole chain gets deleted.*/
	~DoubleLinkNode();
	/**Advance the node to the next node in the chain.
	\param node The node to advance.*/
	static void AdvanceNext(self_type** node);
	/**Advance the node to the previous node in the chain.
	\param node The node to advance.*/
	static void AdvancePrev(self_type** node);
	/**Insert a node after another node.
	\param node The node for which the data will be inserted after.
	\param inserted The node that will be inserted.*/
	static void InsertAfter(self_type* node, self_type* inserted);
	/**insert a new node into the chain.
	\param node The node to insert the new node after.
	\param args Any args to pass to the ctor of the new value inside the new
	node.
	\return A pointer to the new node.*/
	template<typename...Args>
	static self_type* InsertNewAfter(self_type* node, Args&&...args);
	/**Insert a node before another node.
	\param node The node for which the data will be inserted before.
	\param inserted The node that will be inserted.*/
	static void InsertBefore(self_type* node, self_type* inserted);
	/**insert a new node into the chain.
	\param node The node to insert the new node before.
	\param args Any args to pass to the ctor of the new value inside the new
	node.
	\return A pointer to the new node.*/
	template<typename...Args>
	static self_type* InsertNewBefore(self_type* node, Args&&...args);
	/**A pointer to the data.*/
	pointer m_ptr;
	/**A link to the next iterator.*/
	self_type* m_next = nullptr;
	/**A link to the previous iterator.*/
	self_type* m_prev = nullptr;
	/**Determine if this is the before_first node.
	\return True if this node exists before the first value node.*/
	bool IsBeforeFirst() const {
		return !m_ptr && !m_prev && m_next;
	}
	/**Determine if this is the after_last node.
	\return True if this node exists after the last value node.*/
	bool IsAfterLast() const {
		return !m_ptr && m_prev && !m_next;
	}
	/**Determine if one node is equal to another.
	\param lhs The first node in the op.
	\param rhs The second node in the op.
	\return true if LHS and RHS are the same Pointer.*/
	static bool Equal(SelfType* lhs, SelfType* rhs);
	/**Dereference a node.
	\return A reference to the nodes data.*/
	static Pointer Dereference(SelfType* node) {
		return node->m_ptr;
	}
#if _DEBUGDOUBLELINKNODE
	/**Debug only member to see the address of this object.*/
	const Node* md_self = this;
	/**Debug member for knowing if this is the before first.*/
	bool md_beforeFirst = false;
	/**Debug member for knowing if this is the after last.*/
	bool md_afterLast = false;
	/**Throw if trying to move past an cap iterator.*/
	void D_CheckBounds()const;
	/**Throw an error if the poitner is not dereferencable.*/
	void D_CheckValid() const;
#endif
};


template<typename T>
inline void DoubleLinkNode<T>::CreateLinkedEnds(
	self_type & beforeBegin,
	self_type & afterEnd)
{
	beforeBegin.m_prev = nullptr;
	beforeBegin.m_ptr = nullptr;
	beforeBegin.m_next = &afterEnd;
	afterEnd.m_ptr = nullptr;
	afterEnd.m_prev = &beforeBegin;
	afterEnd.m_next = nullptr;
#if _DEBUGDOUBLELINKNODE
	beforeBegin.md_beforeFirst = true;
	afterEnd.md_afterLast = true;
#endif
}

template<typename T>
inline DoubleLinkNode<T>::~DoubleLinkNode()
{
	if (!IsBeforeFirst() && !IsAfterLast())
	{
#if _DEBUGDOUBLELINKNODE
		D_CheckValid();
#endif
		cg::Delete(m_ptr);
	}
}

template<typename T>
inline void DoubleLinkNode<T>::AdvanceNext(self_type** node)
{
#if _DEBUGDOUBLELINKNODE
	(*node)->D_CheckBounds();
#endif
	(*node) = (*node)->m_next;
}

template<typename T>
inline void DoubleLinkNode<T>::AdvancePrev(self_type** node)
{
#if _DEBUGDOUBLELINKNODE
	(*node)->D_CheckBounds();
#endif
	(*node) = (*node)->m_prev;
}

template<typename T>
inline void DoubleLinkNode<T>::InsertAfter(self_type * node,
	self_type * inserted)
{
	/*my old next's previous = the new node*/
	node->m_next->m_prev = inserted;
	/*new node prev = me*/
	inserted->m_prev = node;
	/*new node nex = my old next*/
	inserted->m_next = node->m_next;
	/*my next = new node*/
	node->m_next = inserted;
}

template<typename T>
inline void DoubleLinkNode<T>::InsertBefore(self_type * node,
	self_type * inserted)
{
	/*new node next = me*/
	inserted->m_next = node;
	/*new node prev = my old prev*/
	inserted->m_prev = node->m_prev;
	/*my old previous' next = the new node*/
	node->m_prev->m_next = inserted;
	/*my prev = new node*/
	node->m_prev = inserted;
}

template<typename T>
template<typename ...Args>
inline typename DoubleLinkNode<T>::self_type *
DoubleLinkNode<T>::InsertNewAfter(self_type * node, Args && ...args)
{
	self_type* newNode = cg::New<self_type>();
	/*create data space*/
	newNode->m_ptr = cg::New<value_type>(std::forward<Args>(args)...);
	InsertAfter(node, newNode);
	return newNode;
}
template<typename T>
template<typename ...Args>
inline typename DoubleLinkNode<T>::self_type *
DoubleLinkNode<T>::InsertNewBefore(self_type * node, Args && ...args)
{
	self_type* newNode = cg::New<self_type>();
	/*create data space*/
	newNode->m_ptr = cg::New<value_type>(std::forward<Args>(args)...);
	InsertBefore(node, newNode);
	return newNode;
}
template<typename T>
inline bool DoubleLinkNode<T>::Equal(SelfType * lhs, SelfType * rhs)
{
	return lhs->m_ptr == rhs->m_ptr
		&& lhs->m_next == rhs->m_next
		&& lhs->m_prev == rhs->m_prev;
}
#if _DEBUGDOUBLELINKNODE
template<typename T>
inline void DoubleLinkNode<T>::D_CheckBounds() const
{
	if (!m_ptr)
	{
		cg::Logger::LogError(__FUNCSTR__,
			"The iterator is out of bounds.");
		throw IteratorOutOfBoundsException();
	}
}
template<typename T>
inline void DoubleLinkNode<T>::D_CheckValid() const
{
	if (!m_ptr)
	{
		cg::Logger::LogError(__FUNCSTR__,
			"The iterator does not have a valid data pointer.");
		throw NullPtrException();
	}
}
#endif


#if _DEBUGDOUBLELINKNODE
/**Force some instantiations*/
template struct DoubleLinkNode<int>;
#endif

}