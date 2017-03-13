#pragma once

#include "RandomAccessIterator.hpp"

namespace cg {
template<typename T>
inline void RandomAccessIterator<T>::operator=(const self_type & other)
{
	m_ptr = other.m_ptr;
}
template<typename T>
typename RandomAccessIterator<T>::self_type &
RandomAccessIterator<T>::operator++()
{
	++m_ptr;
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
	--m_ptr;
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
	return *m_ptr;
}
template<typename T>
typename RandomAccessIterator<T>::pointer
RandomAccessIterator<T>::operator->()
{
	return m_ptr;
}
template<typename T>
bool RandomAccessIterator<T>::operator==(const self_type& rhs) const
{
	return m_ptr == rhs.m_ptr;
}
template<typename T>
bool RandomAccessIterator<T>::operator!=(const self_type& rhs) const 
{
	return m_ptr != rhs.m_ptr;
}
template<typename T>
inline bool RandomAccessIterator<T>::operator<(const self_type & other) const
{
	return m_ptr < other.m_ptr;
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
	m_ptr += amt;
	return *this;
}
template<typename T>
inline typename RandomAccessIterator<T>::self_type &
RandomAccessIterator<T>::operator-=(std::size_t amt)
{
	m_ptr -= amt;
	return *this;
}
template<typename T>
typename RandomAccessIterator<T>::self_type
RandomAccessIterator<T>::operator+(std::size_t amt)
{
	auto copy = *this;
	return copy += amt;
}
template<typename T>
inline typename RandomAccessIterator<T>::self_type
RandomAccessIterator<T>::operator-(std::size_t amt)
{
	auto copy = *this;
	return copy -= amt;
}
/***************************************************************************/


template<typename T>
inline void ConstRandomAccessIterator<T>::operator=(const self_type & other)
{
	m_ptr = other.m_ptr;
}

template<typename T>
typename ConstRandomAccessIterator<T>::self_type &
ConstRandomAccessIterator<T>::operator++()
{
	++m_ptr;
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
	--m_ptr;
	return *this;
}
template<typename T>
typename ConstRandomAccessIterator<T>::const_reference
ConstRandomAccessIterator<T>::operator*()
{
	return *m_ptr;
}
template<typename T>
typename ConstRandomAccessIterator<T>::const_pointer
ConstRandomAccessIterator<T>::operator->()
{
	return m_ptr;
}
template<typename T>
bool ConstRandomAccessIterator<T>::operator==(const self_type& rhs) const
{
	return m_ptr == rhs.m_ptr;
}
template<typename T>
bool ConstRandomAccessIterator<T>::operator!=(const self_type& rhs) const
{
	return m_ptr != rhs.m_ptr;
}
template<typename T>
inline bool ConstRandomAccessIterator<T>::operator<(
	const self_type & other) const
{
	return m_ptr < other.m_ptr;
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
	m_ptr += amt;
	return *this;
}

template<typename T>
inline typename ConstRandomAccessIterator<T>::self_type &
ConstRandomAccessIterator<T>::operator-=(std::size_t amt)
{
	m_ptr -= amt;
	return *this;
}

template<typename T>
typename ConstRandomAccessIterator<T>::self_type
ConstRandomAccessIterator<T>::operator+(std::size_t amt)
{
	auto copy = *this;
	return copy += amt;
}

template<typename T>
inline typename ConstRandomAccessIterator<T>::self_type
ConstRandomAccessIterator<T>::operator-(std::size_t amt)
{
	auto copy = *this;
	return copy -= amt;
}

}



