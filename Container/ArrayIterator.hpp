#pragma once

#include "Iterator.hpp"

#define _DEBUGAI _DEBUG

namespace cg {

template<typename T, bool Const = false>
class ArrayIterator : public Iterator<T, Const>
{
public:
	/**The same type as this object.*/
	using self_type = ArrayIterator<T,Const>;
	/**An iterator tag to make this work with std stuff.*/
	using iterator_category = std::random_access_iterator_tag;
	/**Create the iterator with an address.
	\param ptr The pointer to create with.*/
	ArrayIterator(pointer ptr)
		: m_ptr(ptr) {}
	/**Copy asignment contructor.
	\param other The other thing to copy.*/
	ArrayIterator(const self_type& other)
		:m_ptr(other.m_ptr)
	{
#if defined(_DEBUGAI)

		m_beforeFirst = other.m_beforeFirst;
		m_afterLast = other.m_afterLast;
#endif
	}
	/**Move asignment contructor.
	\param other The other thing to move.*/
	ArrayIterator(self_type&& other)
		:m_ptr(std::move(other.m_ptr))
	{
#if defined(_DEBUGAI)

		m_beforeFirst = other.m_beforeFirst;
		m_afterLast = other.m_afterLast;
#endif
	}
#if defined(_DEBUGAI)
	inline void DebugBounds(pointer beforeFirst, pointer afterLast)
	{
		m_beforeFirst = beforeFirst;
		m_afterLast = afterLast;
	}
#endif
	/**Copy assignment operator.
	\param other The other iterator.*/
	inline void operator=(const self_type& other)
	{
		m_ptr = other.m_ptr;
#if defined(_DEBUGAI)
		m_beforeFirst = other.m_beforeFirst;
		m_afterLast = other.m_afterLast;
#endif
	}
	/**Move assignment operator.
	\param other The other iterator.*/
	inline void operator=(self_type&& other)
	{
		m_ptr = std::move(other.m_ptr);
#if defined(_DEBUGAI)
		m_beforeFirst = std::move(other.m_beforeFirst);
		m_afterLast = std::move(other.m_afterLast);
#endif
	}
	/**Increment the poitner by 1. (prefix)
	\return This same type.*/
	inline self_type& operator++()
	{
		++m_ptr;
#if defined(_DEBUGAI)
		CheckForStomp();
#endif
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
		--m_ptr;
#if defined(_DEBUGAI)
		CheckForStomp();
#endif
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
	/**Add to this node.
	\param amt The amount to increment.
	\return A reference to this.*/
	inline self_type& operator+=(std::size_t amt)
	{
		m_ptr += amt;
#if defined(_DEBUGAI)
		CheckForStomp();
#endif
		return *this;
	}
	/**Sub from this node.
	\param amt The amount to decrement.
	\return A reference to this.*/
	inline self_type& operator-=(std::size_t amt)
	{
		m_ptr -= amt;
#if defined(_DEBUGAI)
		CheckForStomp();
#endif
		return *this;
	}
	/**The plus operator.
	\param amt The amount to add to the position.*/
	inline self_type operator+(std::size_t amt) const
	{
		auto copy = *this;
		return copy += amt;
	}
	/**The sub operator.
	\param amt The amount to sub from the position.*/
	inline difference_type operator-(std::size_t amt) const
	{
		return m_ptr - amt;
	}
	/**The sub operator.
	\param ptr The ptr to sub from this.*/
	inline difference_type operator-(pointer ptr) const
	{
		return m_ptr - ptr;
	}
	/**The sub operator.
	\param other The object to sub from the position.
	\return The space between the this iterator and another.*/
	inline difference_type operator-(const self_type& other) const
	{
#if defined(_DEBUGAI)
		CheckValid();
		other.CheckValid();
#endif
		return m_ptr - other.m_ptr;
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
#if defined(_DEBUGAI)
		CheckValid();
#endif
		return m_ptr;
	}
	/**Compare for equality.
	\return True if the iterators are the same.*/
	inline bool operator==(const self_type& rhs) const
	{
		return m_ptr == rhs.m_ptr;
	}
	/**Compare for equality.
	\return False if the iterators are the same.*/
	inline bool operator!=(const self_type& rhs) const
	{
		return !(*this == rhs);
	}
	/**Compare less than.
	\param other The other iterator to compare.
	\return True if other is before this in memory.*/
	inline bool operator<(const self_type& other) const
	{
		return m_ptr < other.m_ptr;
	}
	/**Greater than comparison.
	\param other The other thing to compare.
	\return True if the other thing is after this in memory.*/
	inline bool operator>(const self_type& other) const
	{
		return !((*this) <= other);
	}
	/**Compare less than or equals.
	\param other The other iterator to compare.
	\return True if other is before this in memory or at the same place.*/
	inline bool operator<=(const self_type& other) const
	{
		return (*this < other) || (*this == other);
	}
	/**Greater than or equals comparison.
	\param other The other thing to compare.
	\return True if the other thing is after this in memory or at the same
	place.*/
	inline bool operator>=(const self_type& other) const
	{
		return !(*this < other);
	}
private:
	template<typename T, bool Const>
	friend std::ptrdiff_t operator+(std::size_t,
		const ArrayIterator<T, Const>&);

#if defined(_DEBUGAI)
	pointer m_beforeFirst = nullptr;
	pointer m_afterLast = nullptr;
	/**Throw an error if the pointer is in an invalid location.*/
	void CheckForStomp() const
	{
		if (m_beforeFirst && m_afterLast)
			if (m_ptr < m_beforeFirst || m_ptr > m_afterLast)
				throw IteratorOutOfBoundsException();

		if (!m_ptr)
			throw NullPtrException();
	}
	/**Throw an error if the poitner is not dereferencable.*/
	void CheckValid() const
	{
		if (m_beforeFirst && m_afterLast)
			if (m_ptr <= m_beforeFirst
				|| m_ptr >= m_afterLast)
				throw IteratorOutOfBoundsException();

		if (!m_ptr)
			throw NullPtrException();
	}
#endif
	/**A pointer to the data.*/
	pointer m_ptr;
};

template<typename T, bool Const>
typename ArrayIterator<T,Const>::difference_type
operator+(std::size_t lhs, const ArrayIterator<T, Const>& rhs)
{
	return rhs.m_ptr + lhs;
}


}
