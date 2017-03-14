#pragma once

#include "ForwardIterator.hpp"

namespace cg {

template<typename T>
inline void ForwardIterator<T>::operator=(const self_type & other)
{
	m_node = other.m_node;
}
template<typename T>
inline void ForwardIterator<T>::operator=(self_type && other)
{
	m_node = std::move(other.m_node);
	other.m_node = T();
}
template<typename T>
typename ForwardIterator<T>::self_type &
ForwardIterator<T>::operator++()
{
	++m_node;
	return *this;
}
template<typename T>
typename ForwardIterator<T>::self_type 
ForwardIterator<T>::operator++(int nothing)  const
{
	auto copy = *this;
	return ++copy;
}
template<typename T>
typename ForwardIterator<T>::reference
ForwardIterator<T>::operator*() 
{
	return *m_node;
}
template<typename T>
typename ForwardIterator<T>::pointer
ForwardIterator<T>::operator->()
{
	return m_node;
}
template<typename T>
bool ForwardIterator<T>::operator==(const self_type& rhs) const
{
	return m_node == rhs.m_node;
}
template<typename T>
bool ForwardIterator<T>::operator!=(const self_type& rhs) const 
{
	return m_node != rhs.m_node;
}
/***************************************************************************/


template<typename T>
inline void ConstForwardIterator<T>::operator=(const self_type & other)
{
	m_node = other.m_node;
}

template<typename T>
inline void ConstForwardIterator<T>::operator=(self_type && other)
{
	m_node = std::move(other.m_node);
	other.m_node = T();
}

template<typename T>
typename ConstForwardIterator<T>::self_type &
ConstForwardIterator<T>::operator++()
{
	++m_node;
	return *this;
}
template<typename T>
typename ConstForwardIterator<T>::self_type
ConstForwardIterator<T>::operator++(int nothing)  const
{
	auto copy = *this;
	return ++copy;
}
template<typename T>
typename ConstForwardIterator<T>::const_reference
ConstForwardIterator<T>::operator*() const
{
	return *m_node;
}
template<typename T>
typename ConstForwardIterator<T>::const_pointer
ConstForwardIterator<T>::operator->() const
{
	return m_node;
}
template<typename T>
bool ConstForwardIterator<T>::operator==(const self_type& rhs) const
{
	return m_node == rhs.m_node;
}
template<typename T>
bool ConstForwardIterator<T>::operator!=(const self_type& rhs) const
{
	return m_node != rhs.m_node;
}

}



