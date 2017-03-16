#pragma once

#include "Iterator.hpp"

#define _DEBUGLI _DEBUG && 1

namespace cg {


/**A Node type that will exist under the hood.*/
template<typename T>
struct LinkNode : public Node<T> {
	/**This type.*/
	using self_type = LinkNode<T>;
	/**Create an initial pair of nodes.  These nodes will always be the
	first and lost nodes in a chain.
	\param beforeBegin The very first node.
	\param afterEnd The last node.*/
	static void CreateLinkedEnds(self_type& beforeBegin, self_type& afterEnd);
	/**Delete the data pointers. If before_begin node is deleted,
	than the whole chain gets deleted.*/
	~LinkNode();
	/**Advance the node to the next node in the chain.
	\param node The node to advance.*/
	static void AdvanceNext(self_type** node);
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
	/**A pointer to the data.*/
	pointer m_ptr;
	/**A link to the next iterator.*/
	self_type* m_next = nullptr;
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
#if _DEBUGLI
	/**Debug only member to see the address of this object.*/
	const self_type* md_self = this;
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

template<typename T, bool Const = false>
class LinkIterator : public Iterator<T, Const>
{
public:
	/**The type of node to use.*/
	using NodeType = LinkNode<value_type>;
	/**The same type as this object.*/
	using self_type = LinkIterator<T, Const>;
	/**The same type as this object.*/
	using SelfType = self_type;
	/**An iterator tag to make this work with std stuff.*/
	using iterator_category = std::forward_iterator_tag;
	/**An iterator tag to make this work with std stuff.*/
	using IteratorCatagory = iterator_category;
	/**Construct into a chain.
	\param ptr THe pointer to the data.
	\param node The pointer to the active node.*/
	LinkIterator(NodeType* node) {
		m_node = node;
	}
	/**Copy asignment contructor.
	\param other The other thing to copy.*/
	LinkIterator(const self_type& other)
		:m_node(other.m_node) {}
	/**Move asignment contructor.
	\param other The other thing to move.*/
	LinkIterator(self_type&& other)
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
	/**Add a new link right after this one.
	\param v A value to copy or move to the new node data location.
	\return An iterator for the newly inserted object.*/
	self_type InsertAfter(value_type&& v);
	/**Print the chain from the next link to the last link. The best way to do
	it is to call print on the beforeBegin iterator.
	\param strm A stream buffer to print to.*/
	void PrintChain(std::streambuf* strm);
private:

#if _DEBUGLI
	/**Throw if trying to move past an cap iterator.*/
	void D_CheckBounds()const;
	/**Throw an error if the poitner is not dereferencable.*/
	void D_CheckValid() const;
#endif
	/**A pointer to the node for this iterator.*/
	NodeType* m_node;
};


template<typename T>
inline LinkNode<T>::~LinkNode()
{
	if (!IsBeforeFirst() && !IsAfterLast())
	{
#if _DEBUGLI
		D_CheckValid();
#endif
		cg::Delete(m_ptr);
	}
}
template<typename T>
inline void LinkNode<T>::AdvanceNext(self_type ** node)
{
#if _DEBUGLI
	D_CheckBounds();
#endif
	(*node) = (*node)->m_next;
}
template<typename T>
inline void LinkNode<T>::InsertAfter(self_type * node, self_type * inserted)
{
#if _DEBUGLI
	if (node->IsAfterLast())
		node->D_CheckBounds();
#endif
	inserted->m_next = node->m_next;
	node->m_next = inserted;
}
template<typename T>
template<typename ...Args>
inline typename LinkNode<T>::self_type *
LinkNode<T>::InsertNewAfter(self_type * node, 
	Args && ...args)
{
	self_type* newNode = cg::New<self_type>();
	newNode->m_ptr = cg::New<value_type>(std::forward<Args>(args)...);
	InsertAfter(node, newNode);
	return newNode;
}
template<typename T>
inline void LinkNode<T>::CreateLinkedEnds(
	self_type & beforeBegin,
	self_type & afterEnd)
{
	beforeBegin.m_ptr = nullptr;
	beforeBegin.m_next = &afterEnd;
	afterEnd.m_ptr = nullptr;
	afterEnd.m_next = nullptr;
#if _DEBUGLI
	beforeBegin.md_beforeFirst = true;
	afterEnd.md_afterLast = true;
#endif
}

#if _DEBUGLI
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


/*****************************************************************************/



template<typename T, bool Const>
inline typename LinkIterator<T, Const>::self_type &
LinkIterator<T, Const>::operator++()
{
#if _DEBUGLI
	D_CheckBounds();
#endif

	AdvanceNext(&m_node);
}
template<typename T, bool Const>
inline typename LinkIterator<T, Const>::self_type
LinkIterator<T, Const>::operator++(int nothing)
{
	auto copy = *this;
	++(*this);
	return copy;
}

template<typename T, bool Const>
inline typename LinkIterator<T, Const>::PointerReturnType
LinkIterator<T, Const>::operator->()
{
#if _DEBUGLI
	D_CheckValid();
#endif
	return *m_node->m_ptr;
}

template<typename T, bool Const>
inline bool LinkIterator<T, Const>::
operator==(const self_type & rhs) const {
	auto a = m_node->m_ptr == rhs.m_node->m_ptr;
	auto b = m_node->m_next == rhs.m_node->m_next;
	return a && b;
}

template<typename T, bool Const>
inline typename LinkIterator<T, Const>::self_type
LinkIterator<T, Const>::InsertAfter(value_type&& v)
{
	return SelfType(
		NodeType::InsertNewAfter(m_node, std::forward<value_type>(v)));
}

template<typename T, bool Const>
inline void LinkIterator<T, Const>::PrintChain(std::streambuf * strm)
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
#if _DEBUGLI
template<typename T, bool Const>
inline void LinkIterator<T, Const>::D_CheckBounds() const
{
	m_node->D_CheckBounds();
}

template<typename T, bool Const>
inline void LinkIterator<T, Const>::D_CheckValid() const
{
	m_node->D_CheckValid();
}
#endif


#if _DEBUGLI
void TestLinkIterator()
{
	cg::LinkIterator<int, false>::NodeType beforeBegin;
	cg::LinkIterator<int, false>::NodeType afterEnd;
	cg::LinkIterator<int, false>::NodeType
		::CreateLinkedEnds(beforeBegin, afterEnd);

	cg::LinkIterator<int, false> it(&beforeBegin);
	it.InsertAfter(11);
	it.InsertAfter(22);
	it.InsertAfter(33);
	it.InsertAfter(44);
	it.InsertAfter(55);
	it.InsertAfter(66);
	it.InsertAfter(77);
	it.InsertAfter(88);
	it.InsertAfter(99);
	it.PrintChain(std::cout.rdbuf());
}
#endif


}