#pragma once

#include "Link.hpp"

namespace cg {

/**
The array link is just a single link that will traverse a raw array pointer.
*/
template<typename T>
class RandomAccessLink : public cg::Link<T>
{
public:
	/**The type of this type.*/
	using self_type = RandomAccessLink<T>;
	/**Create the link.
	\param ptr The initial pointer value to set.*/
	RandomAccessLink(T* ptr = nullptr);
	/**Copy ctor.
	\param other The other thing to copy.*/
	RandomAccessLink(const self_type& other);
	/**Move ctor
	\param other The other thing to move.*/
	RandomAccessLink(self_type&& other);
	/**Move assignment operator.
	\param other The other iterator.*/
	void operator=(self_type&& other);
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
	/**Dereference operator.
	\return A reference to the data contained.*/
	reference operator*();
	/**Poitner member access.
	\return A pointer to this (-> is appolied to the pointer too).*/
	pointer operator->();
	/**Dereference operator.
	\return A reference to the data contained.*/
	const_reference operator*() const;
	/**Poitner member access.
	\return A pointer to this (-> is appolied to the pointer too).*/
	const_pointer operator->() const;
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

};

}

#include "RandomAccessLink_impl.hpp"