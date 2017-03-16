#pragma once

#include "Node.hpp"

#define _DEBUGLINKNODE _DEBUG && 1

namespace cg {


/**A Node type that will exist under the hood.*/
template<typename T>
struct LinkNode : public Node<T> {
	/**This type.*/
	using self_type = LinkNode<T>;
	/**This type.*/
	using SelfType = self_type;
	/**Create an initial pair of nodes.  These nodes will always be the
	first and lost nodes in a chain.
	\param beforeBegin The very first node.
	\param afterEnd The last node.*/
	static void CreateLinkedEnds(SelfType& beforeBegin, SelfType& afterEnd);
	/**Delete the data pointers. If before_begin node is deleted,
	than the whole chain gets deleted.*/
	~LinkNode();
	/**Advance the node to the next node in the chain.
	\param node The node to advance.*/
	static void AdvanceNext(SelfType** node);
	/**Insert a node after another node.
	\param node The node for which the data will be inserted after.
	\param inserted The node that will be inserted.*/
	static void InsertAfter(SelfType* node, SelfType* inserted);
	/**insert a new node into the chain.
	\param node The node to insert the new node after.
	\param args Any args to pass to the ctor of the new value inside the new
	node.
	\return A pointer to the new node.*/
	template<typename...Args>
	static SelfType* InsertNewAfter(SelfType* node, Args&&...args);
	/**A pointer to the data.*/
	pointer m_ptr;
	/**A link to the next iterator.*/
	SelfType* m_next = nullptr;
	/**Determine if this is the before_first node.
	\return True if this node exists before the first value node.*/
	bool IsBeforeFirst() const {
		return !m_ptr && m_next;
	}
	/**Determine if this is the after_last node.
	\return True if this node exists after the last value node.*/
	bool IsAfterLast() const {
		return !m_ptr && !m_next;
	}
	/**Dereference a node.
	\return A reference to the nodes data.*/
	static Pointer Dereference(SelfType* node) {
		return node->m_ptr;
	}
#if _DEBUGLINKNODE
	/**Debug only member to see the address of this object.*/
	const SelfType* md_self = this;
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
inline LinkNode<T>::~LinkNode()
{
	if (!IsBeforeFirst() && !IsAfterLast())
	{
#if _DEBUGLINKNODE
		D_CheckValid();
#endif
		cg::Delete(m_ptr);
	}
}
template<typename T>
inline void LinkNode<T>::AdvanceNext(SelfType ** node)
{
#if _DEBUGLINKNODE
	(*node)->D_CheckBounds();
#endif
	(*node) = (*node)->m_next;
}
template<typename T>
inline void LinkNode<T>::InsertAfter(SelfType * node, SelfType * inserted)
{
#if _DEBUGLINKNODE
	if (node->IsAfterLast())
		node->D_CheckBounds();
#endif
	inserted->m_next = node->m_next;
	node->m_next = inserted;
}
template<typename T>
template<typename ...Args>
inline typename LinkNode<T>::SelfType *
LinkNode<T>::InsertNewAfter(SelfType * node,
	Args && ...args)
{
	SelfType* newNode = cg::New<SelfType>();
	newNode->m_ptr = cg::New<value_type>(std::forward<Args>(args)...);
	InsertAfter(node, newNode);
	return newNode;
}
template<typename T>
inline void LinkNode<T>::CreateLinkedEnds(
	SelfType & beforeBegin,
	SelfType & afterEnd)
{
	beforeBegin.m_ptr = nullptr;
	beforeBegin.m_next = &afterEnd;
	afterEnd.m_ptr = nullptr;
	afterEnd.m_next = nullptr;
#if _DEBUGLINKNODE
	beforeBegin.md_beforeFirst = true;
	afterEnd.md_afterLast = true;
#endif
}

#if _DEBUGLINKNODE
template<typename T>
inline void LinkNode<T>::D_CheckBounds() const
{
	if (!m_next)
	{
		cg::Logger::LogError(__FUNCSTR__,
			"The iterator is out of bounds.");
		throw IteratorOutOfBoundsException();
	}
}
template<typename T>
inline void LinkNode<T>::D_CheckValid() const
{
	if (!m_ptr)
	{
		cg::Logger::LogError(__FUNCSTR__,
			"The iterator does not have a valid data pointer.");
		throw NullPtrException();
	}
}
#endif


#if _DEBUGLINKNODE
/**Force some instantiations*/
template struct LinkNode<int>;
#endif

}