#pragma once

#include "../Memory.hpp"

#include "Iterator.hpp"

namespace cg {


template<typename T>
class ForwardIterator : public IteratorBase<T>
{
public:
	/**A type that is this current type.*/
	using self_type = ForwardIterator<value_type>;
	/**An iterator tag to make this work with std stuff.*/
	using iterator_category = std::forward_iterator_tag;
	ForwardIterator() {};
	/**Constructor for taking over apointer.
	\param node The pointer to be contained.*/
	ForwardIterator(pointer node)
		: m_node(node) { }
	/**Constructor for taking over apointer.
	\param other The other iterator to be copied.*/
	ForwardIterator(const self_type& other)
		: m_node(other.m_node) { }
	/**Constructor for taking over apointer.
	\param other The other iterator to be copied.*/
	ForwardIterator(self_type&& other)
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
class ConstForwardIterator : public IteratorBase<T>
{
public:
	/**A type that is this current type.*/
	using self_type = ConstForwardIterator<value_type>;
	/**An iterator tag to make this work with std stuff.*/
	using iterator_category = std::forward_iterator_tag;
	ConstForwardIterator() {};
	/**Constructor for taking over apointer.
	\param node The pointer to be contained.*/
	ConstForwardIterator(pointer node)
		: m_node(node) { }
	/**Constructor for taking over apointer.
	\param other The other iterator to be copied.*/
	ConstForwardIterator(const self_type& other)
		: m_node(other.m_node) { }
	/**Constructor for taking over apointer.
	\param other The other iterator to be copied.*/
	ConstForwardIterator(self_type&& other)
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

#include "ForwardIterator_impl.hpp"



