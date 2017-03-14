#pragma once

#include "../Memory.hpp"

#include "Iterator.hpp"

namespace cg {


template<typename T>
class BiIterator : public IteratorBase<T>
{
public:
	/**A type that is this current type.*/
	using self_type = BiIterator<value_type>;
	/**An iterator tag to make this work with std stuff.*/
	using iterator_category = std::bidirectional_iterator_tag;
	BiIterator() {};
	/**Constructor for taking over apointer.
	\param node The pointer to be contained.*/
	BiIterator(pointer node)
		: m_node(node) { }
	/**Constructor for taking over apointer.
	\param other The other iterator to be copied.*/
	BiIterator(const self_type& other)
		: m_node(other.m_node) { }
	/**Constructor for taking over apointer.
	\param other The other iterator to be copied.*/
	BiIterator(self_type&& other)
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
	self_type operator++(int nothing) const;
	/**Decrement the poitner by 1. (prefix)
	\return This same type after incrementing.*/
	self_type& operator--();
	/**Decrement this by 1. (postfix)
	\param nothing Ignored pointer.
	\return a copy of this before decrementing.*/
	self_type operator--(int nothing) const;
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
private:
	/**A pointer to the memory location of the thing.*/
	T* m_node;
};


template<typename T>
class ConstBiIterator : public IteratorBase<T>
{
public:
	/**A type that is this current type.*/
	using self_type = ConstBiIterator<value_type>;
	/**An iterator tag to make this work with std stuff.*/
	using iterator_category = std::bidirectional_iterator_tag;
	ConstBiIterator() {};
	/**Constructor for taking over apointer.
	\param node The pointer to be contained.*/
	ConstBiIterator(pointer node)
		: m_node(node) { }
	/**Constructor for taking over apointer.
	\param other The other iterator to be copied.*/
	ConstBiIterator(const self_type& other)
		: m_node(other.m_node) { }
	/**Constructor for taking over apointer.
	\param other The other iterator to be copied.*/
	ConstBiIterator(self_type&& other)
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
	self_type operator++(int nothing) const;
	/**Decrement the poitner by 1. (prefix)
	\return This same type after incrementing.*/
	self_type& operator--();
	/**Decrement this by 1. (postfix)
	\param nothing Ignored pointer.
	\return a copy of this before decrementing.*/
	self_type operator--(int nothing) const;
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
private:
	/**A pointer to the memory location of the thing.*/
	T* m_node;
};

}

#include "BiIterator_impl.hpp"



