#pragma once

#include "ArrayNode.hpp"

namespace cg {



template<typename T>
inline ArrayNode<T>::ArrayNode(T * ptr)
{
	if (ptr == nullptr)
		;//TODO throw here.
	m_ptr = ptr;
}

template<typename T>
inline ArrayNode<T>::ArrayNode(std::size_t size)
{
	if (size == 0)
		;//TODO throw here
	m_ptr = new T[size];
}

template<typename T>
inline typename ArrayNode<T>::self_type
ArrayNode<T>::operator++(int nothing)
{
	auto copy = *this;
	++m_ptr;
	return copy;
}

template<typename T>
inline typename ArrayNode<T>::self_type
ArrayNode<T>::operator++()
{
	++m_ptr;
	return *this;
}

}