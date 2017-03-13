#pragma once

#include "../Memory.hpp"

#include "Iterator.hpp"

namespace cg {


template<typename T>
class RandomAccessIterator : public IteratorBase<T>
{
public:
	/**A type that is this current type.*/
	using self_type = RandomAccessIterator<value_type>;
	/**An iterator tag to make this work with std stuff.*/
	using iterator_category = std::random_access_iterator_tag;
	/**Constructor for taking over apointer.
	\param ptr The pointer to be contained.*/
	RandomAccessIterator(pointer ptr)
		: m_ptr(ptr) { }
	/**Constructor for taking over apointer.
	\param other The other iterator to be copied.*/
	RandomAccessIterator(const self_type& other)
		: m_ptr(other.m_ptr) { }
	/**Copy assignment operator.
	\param other The other iterator.*/
	void operator=(const self_type& other);
	/**Increment the poitner by 1. (prefix)
	\return This same type.*/
	self_type& operator++();
	/**Increment this by 1. (postfix)
	\param nothing Ignored pointer.
	\return a copy of this befor incrementing.*/
	self_type operator++(int nothing) const;
	/**Decrement the poitner by 1. (prefix)
	\return This same type after incrementing.*/
	self_type& operator--();
	/**Decrement this by 1. (postfix)
	\param nothing Ignored pointer.
	\return a copy of this before decrementing.*/
	self_type operator--(int nothing) const;
	/**Add to this ptr.
	\param amt The amount to increment.
	\return A reference to this.*/
	self_type& operator+=(std::size_t amt);
	/**Sub from this ptr.
	\param amt The amount to decrement.
	\return A reference to this.*/
	self_type& operator-=(std::size_t amt);
	/**The plus operator.
	\param amt The amount to add to the position.*/
	self_type operator+(std::size_t amt);
	/**The sub operator.
	\param amt The amount to sub from the position.*/
	self_type operator-(std::size_t amt);
	/**Dereference operator.
	\return A reference to the data contained.*/
	reference operator*();
	/**Poitner member access.
	\return A pointer to this (-> is appolied to the pointer too).*/
	pointer operator->();
	/**Compare for equality.
	\return True if the iterators are the same.*/
	bool operator==(const self_type& rhs) const;
	/**Compare for equality.
	\return False if the iterators are the same.*/
	bool operator!=(const self_type& rhs) const;
	/**Compare less than.
	\param other The other iterator to compare.
	\return True if other is before this in memory.*/
	bool operator<(const self_type& other) const;
	/**Greater than comparison.
	\param other The other thing to compare.
	\return True if the other thing is after this in memory.*/
	bool operator>(const self_type& other) const;
	/**Compare less than or equals.
	\param other The other iterator to compare.
	\return True if other is before this in memory or at the same place.*/
	bool operator<=(const self_type& other) const;
	/**Greater than or equals comparison.
	\param other The other thing to compare.
	\return True if the other thing is after this in memory or at the same
	place.*/
	bool operator>=(const self_type& other) const;
private:
	/**A pointer to the memory location of the thing.*/
	T* m_ptr;
};


template<typename T>
class ConstRandomAccessIterator : public IteratorBase<T>
{
public:
	/**A type that is this current type.*/
	using self_type = ConstRandomAccessIterator<value_type>;
	/**An iterator tag to make this work with std stuff.*/
	using iterator_category = std::random_access_iterator_tag;
	/**Constructor for taking over apointer.
	\param ptr The pointer to be contained.*/
	ConstRandomAccessIterator(pointer ptr)
		: m_ptr(ptr) { }
	/**Constructor for taking over apointer.
	\param other The other iterator to be copied.*/
	ConstRandomAccessIterator(const self_type& other)
		: m_ptr(other.m_ptr) { }
	/**Copy assignment operator.
	\param other The other iterator.*/
	void operator=(const self_type& other);
	/**Increment the poitner by 1. (prefix)
	\return This same type.*/
	self_type& operator++();
	/**Increment this by 1. (postfix)
	\param nothing Ignored pointer.
	\return a copy of this befor incrementing.*/
	self_type operator++(int nothing) const;
	/**Decrement the poitner by 1. (prefix)
	\return This same type after incrementing.*/
	self_type& operator--();
	/**Decrement this by 1. (postfix)
	\param nothing Ignored pointer.
	\return a copy of this before decrementing.*/
	self_type operator--(int nothing) const;
	/**Add to this ptr.
	\param amt The amount to increment.
	\return A reference to this.*/
	self_type& operator+=(std::size_t amt);
	/**Sub from this ptr.
	\param amt The amount to decrement.
	\return A reference to this.*/
	self_type& operator-=(std::size_t amt);
	/**The plus operator.
	\param amt The amount to add to the position.*/
	self_type operator+(std::size_t amt);
	/**The sub operator.
	\param amt The amount to sub from the position.*/
	self_type operator-(std::size_t amt);
	/**Dereference operator.
	\return A reference to the data contained.*/
	const_reference operator*();
	/**Poitner member access.
	\return A pointer to this (-> is appolied to the pointer too).*/
	const_pointer operator->();
	/**Compare for equality.
	\return True if the iterators are the same.*/
	bool operator==(const self_type& rhs) const;
	/**Compare for equality.
	\return False if the iterators are the same.*/
	bool operator!=(const self_type& rhs) const;
	/**Compare less than.
	\param other The other iterator to compare.
	\return True if other is before this in memory.*/
	bool operator<(const self_type& other) const;
	/**Greater than comparison.
	\param other The other thing to compare.
	\return True if the other thing is after this in memory.*/
	bool operator>(const self_type& other) const;
	/**Compare less than or equals.
	\param other The other iterator to compare.
	\return True if other is before this in memory or at the same place.*/
	bool operator<=(const self_type& other) const;
	/**Greater than or equals comparison.
	\param other The other thing to compare.
	\return True if the other thing is after this in memory or at the same 
	place.*/
	bool operator>=(const self_type& other) const;
private:
	/**A pointer to the memory location of the thing.*/
	T* m_ptr;
};

}

#include "RandomAccessIterator_impl.hpp"



