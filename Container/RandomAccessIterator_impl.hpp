#pragma once

#include "RandomAccessIterator.hpp"

namespace cg {
template<typename T>
inline void RandomAccessIterator<T>::operator=(const self_type & other)
{
	m_node = other.m_node;
}
template<typename T>
inline void RandomAccessIterator<T>::operator=(self_type && other)
{
	m_node = std::move(other.m_node);
	other.m_node = T();
}
template<typename T>
typename RandomAccessIterator<T>::self_type &
RandomAccessIterator<T>::operator++()
{
	++m_node;
	return *this;
}
template<typename T>
typename RandomAccessIterator<T>::self_type 
RandomAccessIterator<T>::operator++(int nothing)  const
{
	auto copy = *this;
	return ++copy;
}
template<typename T>
inline typename RandomAccessIterator<T>::self_type &
RandomAccessIterator<T>::operator--()
{
	--m_node;
	return *this;
}
template<typename T>
inline typename RandomAccessIterator<T>::self_type 
RandomAccessIterator<T>::operator--(int nothing) const
{
	auto copy = *this;
	return --copy;
}
template<typename T>
typename RandomAccessIterator<T>::reference
RandomAccessIterator<T>::operator*() 
{
	return *m_node;
}
template<typename T>
typename RandomAccessIterator<T>::pointer
RandomAccessIterator<T>::operator->()
{
	return m_node;
}
template<typename T>
bool RandomAccessIterator<T>::operator==(const self_type& rhs) const
{
	return m_node == rhs.m_node;
}
template<typename T>
bool RandomAccessIterator<T>::operator!=(const self_type& rhs) const 
{
	return m_node != rhs.m_node;
}
template<typename T>
inline bool RandomAccessIterator<T>::operator<(const self_type & other) const
{
	return m_node < other.m_node;
}
template<typename T>
inline bool RandomAccessIterator<T>::operator>(
	const self_type & other) const
{
	return !((*this) <= other);
}
template<typename T>
inline bool RandomAccessIterator<T>::operator<=(
	const self_type & other) const
{
	return ((*this) < other) || ((*this) == other);
}
template<typename T>
inline bool RandomAccessIterator<T>::operator>=(
	const self_type & other) const
{
	return !(*this < other);
}
template<typename T>
typename RandomAccessIterator<T>::self_type&
RandomAccessIterator<T>::operator+=(std::size_t amt)
{
	m_node += amt;
	return *this;
}
template<typename T>
inline typename RandomAccessIterator<T>::self_type &
RandomAccessIterator<T>::operator-=(std::size_t amt)
{
	m_node -= amt;
	return *this;
}
template<typename T>
typename RandomAccessIterator<T>::self_type
RandomAccessIterator<T>::operator+(std::size_t amt) const
{
	auto copy = *this;
	return copy += amt;
}
template<typename T>
inline typename RandomAccessIterator<T>::self_type
RandomAccessIterator<T>::operator-(std::size_t amt) const
{
	auto copy = *this;
	return copy -= amt;
}
/***************************************************************************/


template<typename T>
inline void ConstRandomAccessIterator<T>::operator=(const self_type & other)
{
	m_node = other.m_node;
}

template<typename T>
inline void ConstRandomAccessIterator<T>::operator=(self_type && other)
{
	m_node = std::move(other.m_node);
	other.m_node = T();
}

template<typename T>
typename ConstRandomAccessIterator<T>::self_type &
ConstRandomAccessIterator<T>::operator++()
{
	++m_node;
	return *this;
}
template<typename T>
typename ConstRandomAccessIterator<T>::self_type
ConstRandomAccessIterator<T>::operator++(int nothing)  const
{
	auto copy = *this;
	return ++copy;
}
template<typename T>
inline typename ConstRandomAccessIterator<T>::self_type&
ConstRandomAccessIterator<T>::operator--()
{
	auto copy = *this;
	return --copy;
}
template<typename T>
inline typename ConstRandomAccessIterator<T>::self_type
ConstRandomAccessIterator<T>::operator--(int nothing) const
{
	--m_node;
	return *this;
}
template<typename T>
typename ConstRandomAccessIterator<T>::const_reference
ConstRandomAccessIterator<T>::operator*() const
{
	return *m_node;
}
template<typename T>
typename ConstRandomAccessIterator<T>::const_pointer
ConstRandomAccessIterator<T>::operator->() const
{
	return m_node;
}
template<typename T>
bool ConstRandomAccessIterator<T>::operator==(const self_type& rhs) const
{
	return m_node == rhs.m_node;
}
template<typename T>
bool ConstRandomAccessIterator<T>::operator!=(const self_type& rhs) const
{
	return m_node != rhs.m_node;
}
template<typename T>
inline bool ConstRandomAccessIterator<T>::operator<(
	const self_type & other) const
{
	return m_node < other.m_node;
}
template<typename T>
inline bool ConstRandomAccessIterator<T>::operator>(
	const self_type & other) const
{
	return !((*this) <= other);
}
template<typename T>
inline bool ConstRandomAccessIterator<T>::operator<=(
	const self_type & other) const
{
	return ((*this) < other) || ((*this) == other);
}
template<typename T>
inline bool ConstRandomAccessIterator<T>::operator>=(
	const self_type & other) const
{
	return !(*this < other);
}
template<typename T>
typename ConstRandomAccessIterator<T>::self_type&
ConstRandomAccessIterator<T>::operator+=(std::size_t amt)
{
	m_node += amt;
	return *this;
}

template<typename T>
inline typename ConstRandomAccessIterator<T>::self_type &
ConstRandomAccessIterator<T>::operator-=(std::size_t amt)
{
	m_node -= amt;
	return *this;
}

template<typename T>
typename ConstRandomAccessIterator<T>::self_type
ConstRandomAccessIterator<T>::operator+(std::size_t amt) const
{
	auto copy = *this;
	return copy += amt;
}

template<typename T>
inline typename ConstRandomAccessIterator<T>::self_type
ConstRandomAccessIterator<T>::operator-(std::size_t amt) const
{
	auto copy = *this;
	return copy -= amt;
}

}



