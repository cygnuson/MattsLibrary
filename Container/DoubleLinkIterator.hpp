#pragma once

#include "Iterator.hpp"

#define _DEBUGDLI _DEBUG && 1

namespace cg {

template<typename T, bool Const = false>
class DoubleLinkIterator : public Iterator<T, Const>
{
public:
	/**A Node type that will exist under the hood.*/
	struct Node {
		/**Create an initial pair of nodes.  These nodes will always be the
		first and lost nodes in a chain.
		\param beforeBegin The very first node.
		\param afterEnd The last node.*/
		static void CreateLinkedEnds(Node& beforeBegin, Node& afterEnd);
		/**Delete the data pointers. If before_begin node is deleted,
		than the whole chain gets deleted.*/
		~Node();
		/**A pointer to the data.*/
		pointer m_ptr;
		/**A link to the next iterator.*/
		Node* m_next;
		/**A link to the previous iterator.*/
		Node* m_prev;
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
	/**The same type as this object.*/
	using self_type = DoubleLinkIterator<T, Const>;
	/**An iterator tag to make this work with std stuff.*/
	using iterator_category = std::bidirectional_iterator_tag;
	/**Construct into a chain.
	\param ptr THe pointer to the data.
	\param next The pointer to the next link.
	\param prev The previous link in the chain.*/
	DoubleLinkIterator(Node* node) {
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
	inline bool operator==(const self_type& rhs) const {
		return m_node == rhs.m_node;
	}
	/**Compare for equality.
	\return False if the iterators are the same.*/
	inline bool operator!=(const self_type& rhs) const {
		return !(*this == rhs);
	}
	/**Add a new link right after this one.
	\param v A value to copy or move to the new node data location.
	\return An iterator for the newly inserted object.*/
	self_type InsertAfter(value_type v);
	/**Add a new link right before this one.
	\param v A value to copy or move to the new node data location.
	\return An iterator for the newly inserted object.*/
	self_type InsertBefore(value_type v);
	/**A lexigraphic function. InsertBefore is the same. The index that the
	iterator is currently on, will be the index for the new variable.
	\sa InsertBefore
	\param v The value to be inserted.*/
	self_type InsertHere(value_type v) {
		return InsertBefore(v);
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
	Node* m_node;
};


template<typename T, bool Const>
inline void
DoubleLinkIterator<T, Const>::PrintChain(std::streambuf * strm)
{
	std::ostream out(strm);
	std::size_t index = 0;
	for (Node* n = m_node->m_next; n != nullptr; n = n->m_next)
	{
		if (!n->m_ptr)
		{
			out << std::endl;
			break;
		}
		out << "\n" << "[" << ++index << "]= " << *n->m_ptr;
	}
}

template<typename T, bool Const>
inline void
DoubleLinkIterator<T, Const>::PrintChainReverse(std::streambuf * strm)
{
	std::ostream out(strm);
	std::size_t index = 0;
	for (Node* n = m_node->m_prev; n != nullptr; n = n->m_prev)
	{
		if (!n->m_ptr)
		{
			out << std::endl;
			break;
		}
		out << "\n" << "[" << ++index << "]= " << *n->m_ptr;
	}
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
inline typename DoubleLinkIterator<T, Const>::self_type
DoubleLinkIterator<T, Const>::InsertAfter(value_type v)
{
#if _DEBUGDLI
	if (m_node->IsAfterLast())
	{
		cg::Logger::LogError(__FUNCSTR__,
			"The iterator is out of bounds.");
		throw IteratorOutOfBoundsException();

	}
#endif
	auto newNode = cg::New<Node>();
	/*create data space*/
	newNode->m_ptr = cg::New<value_type>(v);
	/*my old next's previous = the new node*/
	m_node->m_next->m_prev = newNode;
	/*new node prev = me*/
	newNode->m_prev = m_node;
	/*new node nex = my old next*/
	newNode->m_next = m_node->m_next;
	/*my next = new node*/
	m_node->m_next = newNode;
	return self_type(newNode);
}

template<typename T, bool Const>
inline typename DoubleLinkIterator<T, Const>::self_type
DoubleLinkIterator<T, Const>::InsertBefore(value_type v)
{
#if _DEBUGDLI
	if (m_node->IsBeforeFirst())
	{
		cg::Logger::LogError(__FUNCSTR__,
			"The iterator is out of bounds.");
		throw IteratorOutOfBoundsException();

	}
#endif
	auto newNode = cg::New<Node>();
	/*set new data ptr*/
	newNode->m_ptr = cg::New<value_type>(v);
	/*new node next = me*/
	newNode->m_next = m_node;
	/*new node prev = my old prev*/
	newNode->m_prev = m_node->m_prev;
	/*my old previous' next = the new node*/
	m_node->m_prev->m_next = newNode;
	/*my prev = new node*/
	m_node->m_prev = newNode;
	return self_type(newNode);
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

template<typename T, bool Const>
inline void DoubleLinkIterator<T, Const>::Node::CreateLinkedEnds(
	Node & beforeBegin,
	Node & afterEnd)
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

template<typename T, bool Const>
inline DoubleLinkIterator<T, Const>::Node::~Node()
{
	if (!IsBeforeFirst() && !IsAfterLast())
	{
#if _DEBUGDLI
		D_CheckValid();
#endif
		cg::Delete(m_ptr);
	}
}

#if _DEBUGDLI
template<typename T, bool Const>
inline void DoubleLinkIterator<T, Const>::Node::D_CheckBounds() const
{
	if (!m_ptr)
	{
		cg::Logger::LogError(__FUNCSTR__,
			"The iterator is out of bounds.");
		throw IteratorOutOfBoundsException();
	}
}
template<typename T, bool Const>
inline void DoubleLinkIterator<T, Const>::Node::D_CheckValid() const
{
	if (!m_ptr)
	{
		cg::Logger::LogError(__FUNCSTR__,
			"The iterator does not have a valid data pointer.");
		throw NullPtrException();
	}
}
#endif

}
