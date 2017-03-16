#pragma once

#include "Iterator.hpp"

#define _DEBUGDLI _DEBUG && 1

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
#if _DEBUGDLI
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

template<typename T, bool Const = false>
class DoubleLinkIterator : public Iterator<T, Const>
{
public:
	/**The type of node to use.*/
	using NodeType = DoubleLinkNode<value_type>;
	/**The same type as this object.*/
	using self_type = DoubleLinkIterator<T, Const>;
	/**The same type as this object.*/
	using SelfType = self_type;
	/**An iterator tag to make this work with std stuff.*/
	using iterator_category = std::bidirectional_iterator_tag;
	/**An iterator tag to make this work with std stuff.*/
	using IteratorCategory = iterator_category;
	/**Construct into a chain.
	\param ptr THe pointer to the data.
	\param node The pointer to the active node.*/
	DoubleLinkIterator(NodeType* node) {
		m_node = node;
	}
	/**Copy asignment contructor.
	\param other The other thing to copy.*/
	DoubleLinkIterator(const self_type& other)
		:m_node(other.m_node) {}
	/**Move asignment contructor.
	\param other The other thing to move.*/
	DoubleLinkIterator(self_type&& other)
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
	/**Add a new link right after this one.
	\param v A value to copy or move to the new node data location.
	\return An iterator for the newly inserted object.*/
	self_type InsertAfter(value_type&& v);
	/**Add a new link right before this one.
	\param v A value to copy or move to the new node data location.
	\return An iterator for the newly inserted object.*/
	self_type InsertBefore(value_type&& v);
	/**A lexigraphic function. InsertBefore is the same. The index that the
	iterator is currently on, will be the index for the new variable.
	\sa InsertBefore
	\param v The value to be inserted.*/
	self_type InsertHere(value_type&& v) {
		return InsertBefore(std::forward<value_type>(v));
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

#if _DEBUGDLI
	/**Throw if trying to move past an cap iterator.*/
	void D_CheckBounds()const;
	/**Throw an error if the poitner is not dereferencable.*/
	void D_CheckValid() const;
#endif
	/**A pointer to the node for this iterator.*/
	NodeType* m_node;
};


template<typename T, bool Const>
inline void
DoubleLinkIterator<T, Const>::PrintChain(std::streambuf * strm)
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

template<typename T, bool Const>
inline void
DoubleLinkIterator<T, Const>::PrintChainReverse(std::streambuf * strm)
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

template<typename T, bool Const>
inline typename DoubleLinkIterator<T, Const>::self_type &
DoubleLinkIterator<T, Const>::operator++()
{
#if _DEBUGDLI
	D_CheckBounds();
#endif
	m_node = m_node->m_next;
	return *this;
}

template<typename T, bool Const>
inline typename DoubleLinkIterator<T, Const>::self_type
DoubleLinkIterator<T, Const>::operator++(int nothing)
{
	auto copy = *this;
	++(*this);
	return copy;
}

template<typename T, bool Const>
inline typename DoubleLinkIterator<T, Const>::self_type &
DoubleLinkIterator<T, Const>::operator--()
{
#if _DEBUGDLI
	D_CheckBounds();
#endif
	m_node = m_node->m_prev;
	return *this;
}

template<typename T, bool Const>
inline typename DoubleLinkIterator<T, Const>::self_type
DoubleLinkIterator<T, Const>::operator--(int nothing)
{
	auto copy = *this;
	--(*this);
	return copy;
}

template<typename T, bool Const>
inline typename DoubleLinkIterator<T, Const>::PointerReturnType
DoubleLinkIterator<T, Const>::operator->()
{
#if _DEBUGDLI
	D_CheckValid();
#endif
	return m_node->m_ptr;
}

template<typename T, bool Const>
inline bool DoubleLinkIterator<T, Const>::
operator==(const self_type & rhs) const {
	auto a = this->m_node->m_ptr == rhs.->m_node->m_ptr;
	auto b = this->m_node->m_next == rhs.->m_next->m_ptr;
	auto c = this->m_node->m_prev == rhs.->m_next->m_prev;
	return a && b && c;
}

template<typename T, bool Const>
inline typename DoubleLinkIterator<T, Const>::self_type
DoubleLinkIterator<T, Const>::InsertAfter(value_type&& v)
{
#if _DEBUGDLI
	if (m_node->IsAfterLast())
	{
		cg::Logger::LogError(__FUNCSTR__,
			"The iterator is out of bounds.");
		throw IteratorOutOfBoundsException();

	}
#endif
	return self_type(NodeType::
		InsertNewBefore(m_node, std::forward<value_type>(v)));
}

template<typename T, bool Const>
inline typename DoubleLinkIterator<T, Const>::self_type
DoubleLinkIterator<T, Const>::InsertBefore(value_type&& v)
{
#if _DEBUGDLI
	if (m_node->IsBeforeFirst())
	{
		cg::Logger::LogError(__FUNCSTR__,
			"The iterator is out of bounds.");
		throw IteratorOutOfBoundsException();

	}
#endif
	return self_type(NodeType::
		InsertNewBefore(m_node, std::forward<value_type>(v)));
}



#if _DEBUGDLI
template<typename T, bool Const>
inline void DoubleLinkIterator<T, Const>::D_CheckBounds() const
{
	if (!m_node->m_ptr)
	{
		cg::Logger::LogError(__FUNCSTR__,
			"The iterator is out of bounds.");
		throw IteratorOutOfBoundsException();
	}
}
template<typename T, bool Const>
inline void DoubleLinkIterator<T, Const>::D_CheckValid() const
{
	if (!m_node->m_ptr)
	{
		cg::Logger::LogError(__FUNCSTR__,
			"The iterator does not have a valid data pointer.");
		throw NullPtrException();
	}
}
#endif



/*****************************************************************************/

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
#if _DEBUGDLI
	beforeBegin.md_beforeFirst = true;
	afterEnd.md_afterLast = true;
#endif
}

template<typename T>
inline DoubleLinkNode<T>::~DoubleLinkNode()
{
	if (!IsBeforeFirst() && !IsAfterLast())
	{
#if _DEBUGDLI
		D_CheckValid();
#endif
		cg::Delete(m_ptr);
	}
}

template<typename T>
inline void DoubleLinkNode<T>::AdvanceNext(self_type** node)
{
#if _DEBUGDLI
	node.D_CheckBounds();
#endif
	(*node) = (*node)->m_next;
}

template<typename T>
inline void DoubleLinkNode<T>::AdvancePrev(self_type** node)
{
#if _DEBUGDLI
	node.D_CheckBounds();
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
#if _DEBUGDLI
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

#if _DEBUGDLI
void TestDoubleLinkIterator()
{
	cg::DoubleLinkIterator<int, false>::NodeType beforeBegin;
	cg::DoubleLinkIterator<int, false>::NodeType afterEnd;
	cg::DoubleLinkIterator<int, false>::NodeType
		::CreateLinkedEnds(beforeBegin, afterEnd);

	cg::DoubleLinkIterator<int, false> itEnd(&beforeBegin);
	cg::DoubleLinkIterator<int, false> it(&afterEnd);
	it.InsertBefore(11);
	it.InsertBefore(22);
	it.InsertBefore(33);
	it.InsertBefore(44);
	it.InsertBefore(55);
	it.InsertBefore(66);
	it.InsertBefore(77);
	it.InsertBefore(88);
	it.InsertBefore(99);
	itEnd.PrintChain(std::cout.rdbuf());
	it.PrintChainReverse(std::cout.rdbuf());
}
#endif
}
