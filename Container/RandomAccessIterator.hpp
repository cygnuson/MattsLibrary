#pragma once

#include "../Memory.hpp"

#include "Iterator.hpp"

namespace cg {


template<typename T, bool Const>
class RandomAccessIterator : public IteratorBase<T,Const>
{
public:
	/**A type that is this current type.*/
	using self_type = RandomAccessIterator<T,Const>;
	/**An iterator tag to make this work with std stuff.*/
	using iterator_category = std::random_access_iterator_tag;
	RandomAccessIterator() {};
	/**Constructor for taking over apointer.
	\param other The other iterator to be copied.*/
	RandomAccessIterator(const self_type& other)
		: m_node(other.m_node) { }
	/**Constructor for taking over apointer.
	\param other The other iterator to be copied.*/
	RandomAccessIterator(self_type&& other)
		: m_node(std::move(other.m_node)) {
		other.m_node = T();
	}
	/**Copy assignment operator.
	\param other The other iterator.*/
	void operator=(const self_type& other);
	/**Move assignment operator.
	\param other The other iterator.*/
	void operator=(self_type&& other);
	/**Increment the poitner by 1. (prefix)
	\return This same type.*/
	self_type& operator++();
	/**Increment this by 1. (postfix)
	\param nothing Ignored pointer.
	\return a copy of this befor incrementing.*/
	self_type operator++(int nothing) ;
	/**Decrement the poitner by 1. (prefix)
	\return This same type after incrementing.*/
	self_type& operator--();
	/**Decrement this by 1. (postfix)
	\param nothing Ignored pointer.
	\return a copy of this before decrementing.*/
	self_type operator--(int nothing) ;
	/**Add to this node.
	\param amt The amount to increment.
	\return A reference to this.*/
	self_type& operator+=(std::size_t amt);
	/**Sub from this node.
	\param amt The amount to decrement.
	\return A reference to this.*/
	self_type& operator-=(std::size_t amt);
	/**The plus operator.
	\param amt The amount to add to the position.*/
	self_type operator+(std::size_t amt) const;
	/**The sub operator.
	\param amt The amount to sub from the position.*/
	self_type operator-(std::size_t amt) const;
	/**The sub operator.
	\param amt The object to sub from the position.*/
	self_type operator-(const self_type& other) const;
	/**Dereference operator.
	\return A reference to the data contained.*/
	RefReturnType operator*();
	/**Poitner member access.
	\return A pointer to this (-> is appolied to the pointer too).*/
	PointerReturnType operator->();
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
public:
	/**A pointer to the memory location of the thing.*/
	T m_node;
};

}




