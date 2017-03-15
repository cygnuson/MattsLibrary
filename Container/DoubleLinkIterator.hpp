#pragma once

#include "Iterator.hpp"

#define _DEBUGDLI _DEBUG

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
		static void CreateInitialPair(Node& beforeBegin, Node& afterEnd)
		{
			beforeBegin.m_prev = nullptr;
			beforeBegin.m_ptr = nullptr;
			beforeBegin.m_next = &afterEnd;
			afterEnd.m_ptr = nullptr;
			afterEnd.m_prev = &beforeBegin;
			afterEnd.m_next = nullptr;
#if defined(_DEBUGDLI)
			beforeBegin.md_beforeFirst = true;
			afterEnd.md_afterLast = true;
#endif
		}
		/**A pointer to the data.*/
		pointer m_ptr;
		/**A link to the next iterator.*/
		Node* m_next;
		/**A link to the previous iterator.*/
		Node* m_prev;
		/**Determine if this is the before_first node.
		\return True if this node exists before the first value node.*/
		bool IsBeforeFirst() const
		{
			return !m_ptr && !m_prev && m_next;
		}
		/**Determine if this is the after_last node.
		\return True if this node exists after the last value node.*/
		bool IsAfterLast() const
		{
			return !m_ptr && m_prev && !m_next;
		}
#if defined(_DEBUGDLI)
		/**Debug only member to see the address of this object.*/
		const Node* md_self = this;
		/**Debug member for knowing if this is the before first.*/
		bool md_beforeFirst = false;
		/**Debug member for knowing if this is the after last.*/
		bool md_afterLast = false;
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
	DoubleLinkIterator(Node* node)
	{
		m_node = node;
	}
	/**Copy asignment contructor.
	\param other The other thing to copy.*/
	DoubleLinkIterator(const self_type& other)
		:m_node(other.m_node){}
	/**Move asignment contructor.
	\param other The other thing to move.*/
	DoubleLinkIterator(self_type&& other)
		:m_node(std::move(other.m_node)){}
	/**Copy assignment operator.
	\param other The other iterator.*/
	inline void operator=(const self_type& other)
	{
		m_node = other.m_node;
	}
	/**Move assignment operator.
	\param other The other iterator.*/
	inline void operator=(self_type&& other)
	{
		m_node = std::move(other.m_node);
	}
	/**Increment the poitner by 1. (prefix)
	\return This same type.*/
	inline self_type& operator++()
	{
#if defined(_DEBUGDLI)
		CheckBounds();
#endif
		m_node = m_node->m_next;
		return *this;
	}
	/**Increment this by 1. (postfix)
	\param nothing Ignored pointer.
	\return a copy of this befor incrementing.*/
	inline self_type operator++(int nothing)
	{
		auto copy = *this;
		++(*this);
		return copy;
	}
	/**Decrement the poitner by 1. (prefix)
	\return This same type after incrementing.*/
	inline self_type& operator--()
	{
#if defined(_DEBUGDLI)
		CheckBounds();
#endif
		m_node = m_node->m_prev;
		return *this;
	}
	/**Decrement this by 1. (postfix)
	\param nothing Ignored pointer.
	\return a copy of this before decrementing.*/
	inline self_type operator--(int nothing)
	{
		auto copy = *this;
		--(*this);
		return copy;
	}
	/**Dereference operator.
	\return A reference to the data contained.*/
	inline RefReturnType operator*()
	{
		return *(this->operator->());
	}
	/**Poitner member access.
	\return A pointer to this (-> is appolied to the pointer too).*/
	inline PointerReturnType operator->()
	{
#if defined(_DEBUGDLI)
		CheckValid();
#endif
		return m_node->m_ptr;
	}
	/**Compare for equality.
	\return True if the iterators are the same.*/
	inline bool operator==(const self_type& rhs) const
	{
		return m_node == rhs.m_node;
	}
	/**Compare for equality.
	\return False if the iterators are the same.*/
	inline bool operator!=(const self_type& rhs) const
	{
		return !(*this == rhs);
	}
	/**Add a new link right after this one.
	\param v A value to copy or move to the new node data location.
	\return An iterator for the newly inserted object.*/
	self_type InsertAfter(value_type v)
	{
		if (m_node->IsAfterLast())
		{
			throw IteratorOutOfBoundsException();
		}
		auto newNode = cg::New<Node>();
		newNode->m_ptr = cg::New<value_type>(v);
		newNode->m_prev = m_node;
		newNode->m_next = m_node->m_next;
		m_node->m_next = newNode;
		return self_type(newNode);
	}
	/**Add a new link right before this one.
	\param v A value to copy or move to the new node data location.
	\return An iterator for the newly inserted object.*/
	self_type InsertBefore(value_type v)
	{
		if (m_node->IsBeforeFirst())
		{
			throw IteratorOutOfBoundsException();
		}
		auto newNode = cg::New<Node>();
		newNode->m_ptr = cg::New<value_type>(v);
		newNode->m_next = m_node;
		newNode->m_prev = m_node->m_prev;
		m_node->m_prev = newNode;
		return self_type(newNode);
	}
private:

#if defined(_DEBUGDLI)
	/**Throw if trying to move past an cap iterator.*/
	void CheckBounds()
	{
		if (!m_node->m_ptr)
			throw IteratorOutOfBoundsException();
	}
	/**Throw an error if the poitner is not dereferencable.*/
	void CheckValid() const
	{
		if (!m_node->m_ptr)
			throw NullPtrException();
	}
#endif
	Node* m_node;
};


}
