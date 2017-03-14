#pragma once

#include "BiIterator.hpp"

namespace cg {

template<typename T>
inline void BiIterator<T>::operator=(const self_type & other)
{
	m_node = other.m_node;
}
template<typename T>
inline void BiIterator<T>::operator=(self_type && other)
{
	m_node = std::move(other.m_node);
	other.m_node = T();
}
template<typename T>
typename BiIterator<T>::self_type &
BiIterator<T>::operator++()
{
	++m_node;
	return *this;
}
template<typename T>
typename BiIterator<T>::self_type 
BiIterator<T>::operator++(int nothing)  const
{
	auto copy = *this;
	return ++copy;
}
template<typename T>
inline typename BiIterator<T>::self_type &
BiIterator<T>::operator--()
{
	--m_node;
	return *this;
}
template<typename T>
inline typename BiIterator<T>::self_type 
BiIterator<T>::operator--(int nothing) const
{
	auto copy = *this;
	return --copy;
}
template<typename T>
typename BiIterator<T>::reference
BiIterator<T>::operator*() 
{
	return *m_node;
}
template<typename T>
typename BiIterator<T>::pointer
BiIterator<T>::operator->()
{
	return m_node;
}
template<typename T>
bool BiIterator<T>::operator==(const self_type& rhs) const
{
	return m_node == rhs.m_node;
}
template<typename T>
bool BiIterator<T>::operator!=(const self_type& rhs) const 
{
	return m_node != rhs.m_node;
}
/***************************************************************************/


template<typename T>
inline void ConstBiIterator<T>::operator=(const self_type & other)
{
	m_node = other.m_node;
}

template<typename T>
inline void ConstBiIterator<T>::operator=(self_type && other)
{
	m_node = std::move(other.m_node);
	other.m_node = T();
}

template<typename T>
typename ConstBiIterator<T>::self_type &
ConstBiIterator<T>::operator++()
{
	++m_node;
	return *this;
}
template<typename T>
typename ConstBiIterator<T>::self_type
ConstBiIterator<T>::operator++(int nothing)  const
{
	auto copy = *this;
	return ++copy;
}
template<typename T>
inline typename ConstBiIterator<T>::self_type&
ConstBiIterator<T>::operator--()
{
	auto copy = *this;
	return --copy;
}
template<typename T>
inline typename ConstBiIterator<T>::self_type
ConstBiIterator<T>::operator--(int nothing) const
{
	--m_node;
	return *this;
}
template<typename T>
typename ConstBiIterator<T>::const_reference
ConstBiIterator<T>::operator*() const
{
	return *m_node;
}
template<typename T>
typename ConstBiIterator<T>::const_pointer
ConstBiIterator<T>::operator->() const
{
	return m_node;
}
template<typename T>
bool ConstBiIterator<T>::operator==(const self_type& rhs) const
{
	return m_node == rhs.m_node;
}
template<typename T>
bool ConstBiIterator<T>::operator!=(const self_type& rhs) const
{
	return m_node != rhs.m_node;
}

}



