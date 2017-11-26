/*

(C) Matthew Swanson

This file is part of ColdStorage.

ColdStorage is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
any later version.

ColdStorage is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with ColdStorage.  If not, see <http://www.gnu.org/licenses/>.

*/
#pragma once

#include "ArrayIteratorDef.hpp"

namespace cg {

/**Set the iterator to a certian pointer.
\pre The paramter \p p must not be null.
\param p The pointer to set this iterator too.*/
template<typename DataType, bool Const, bool Reverse>
inline ArrayIterator<DataType, Const, Reverse>::ArrayIterator(Ptr p)
	:m_ptr(p)
{

}
/**Copy an iterator of the proper type.
\param other The other iterator to copy.*/
template<typename DataType, bool Const, bool Reverse>
inline ArrayIterator<DataType, Const, Reverse>::ArrayIterator(
	const SelfType & other)
{
	m_ptr = other.m_ptr;
}
/**Copy an iterator of the proper type.
\param other The other iterator to copy.
\return A reference to this.*/
template<typename DataType, bool Const, bool Reverse>
inline typename ArrayIterator<DataType, Const, Reverse>::SelfType &
ArrayIterator<DataType, Const, Reverse>::operator=(const SelfType & other)
{
	m_ptr = other.m_ptr;
	return *this;
}
/**Move an iterator of the proper type.
\post The parameter \p other will be invalid after the operation.
\param other The other iterator to move.*/
template<typename DataType, bool Const, bool Reverse>
inline ArrayIterator<DataType, Const, Reverse>::ArrayIterator(
	SelfType && other)
{
	m_ptr = other.m_ptr;
#ifdef _DEBUG
	other.m_ptr = nullptr;
#endif // _DEBUG

}
/**move an iterator of the proper type.
\post The parameter \p other will be invalid after the operation.
\param other The other iterator to move.
\return A reference to this.*/
template<typename DataType, bool Const, bool Reverse>
inline typename ArrayIterator<DataType, Const, Reverse>::SelfType &
ArrayIterator<DataType, Const, Reverse>::operator=(SelfType && other)
{
	m_ptr = other.m_ptr;
#ifdef _DEBUG
	other.m_ptr = nullptr;
#endif // _DEBUG
	return *this;
}
/**Dereference the iterator.
\pre The object must point to something.
\return A reference to the data.*/
template<typename DataType, bool Const, bool Reverse>
inline typename ArrayIterator<DataType, Const, Reverse>::Ref
ArrayIterator<DataType, Const, Reverse>::operator*()
{
    CheckAndThrow();
    return *m_ptr;
}
/**Dereference the iterator.
\pre The object must point to something.
\return A reference to the data.*/
template<typename DataType, bool Const, bool Reverse>
inline const typename ArrayIterator<DataType, Const, Reverse>::Ref
ArrayIterator<DataType, Const, Reverse>::operator*() const
{
    CheckAndThrow();
    return *m_ptr;
}
/**Pointer to member access.
\pre The object must point to something.
\return A reference to the data.*/
template<typename DataType, bool Const, bool Reverse>
inline typename ArrayIterator<DataType, Const, Reverse>::Ptr
ArrayIterator<DataType, Const, Reverse>::operator->()
{
    CheckAndThrow();
    return m_ptr;
}
/**Pointer to member access.
\pre The object must point to something.
\return A reference to the data.*/
template<typename DataType, bool Const, bool Reverse>
inline const typename ArrayIterator<DataType, Const, Reverse>::Ptr
ArrayIterator<DataType, Const, Reverse>::operator->() const
{
    CheckAndThrow();
    return m_ptr;
}
/**Postincrement the iterator one space.
\pre The object must point to something.
\return A copy of this object before the increment happened.*/
template<typename DataType, bool Const, bool Reverse>
inline typename ArrayIterator<DataType, Const, Reverse>::SelfType
ArrayIterator<DataType, Const, Reverse>::operator++(int)
{
	CheckAndThrow();
	SelfType copy = *this;
	if (Reverse)
		--m_ptr;
	else
		++m_ptr;
	return copy;
}
/**Preincrement the iterator one space.
\pre The object must point to something.
\return A reference to this object after increment happens.*/
template<typename DataType, bool Const, bool Reverse>
inline typename ArrayIterator<DataType, Const, Reverse>::SelfType &
ArrayIterator<DataType, Const, Reverse>::operator++()
{
	CheckAndThrow();
	if (Reverse)
		--m_ptr;
	else
		++m_ptr;
	return *this;
}
/**Posdecrement the iterator one space.
\pre The object must point to something.
\return A copy of this object before the decrement happened.*/
template<typename DataType, bool Const, bool Reverse>
inline typename ArrayIterator<DataType, Const, Reverse>::SelfType
ArrayIterator<DataType, Const, Reverse>::operator--(int)
{
	CheckAndThrow();
	SelfType copy = *this;
	if (!Reverse)
		--m_ptr;
	else
		++m_ptr;
	return copy;
}
/**Predecrement the iterator one space.
\pre The object must point to something.
\return A reference to this object after decrement happens.*/
template<typename DataType, bool Const, bool Reverse>
inline typename ArrayIterator<DataType, Const, Reverse>::SelfType &
ArrayIterator<DataType, Const, Reverse>::operator--()
{
	CheckAndThrow();
	if (!Reverse)
		--m_ptr;
	else
		++m_ptr;
	return *this;
}
/**Determine if two iterators are different.
\param other The other iterator.
\return True if both iterators do not point to the same slot in memory.*/
template<typename DataType, bool Const, bool Reverse>
inline bool ArrayIterator<DataType, Const, Reverse>::operator!=(
	const SelfType & other) const
{
	return m_ptr != other.m_ptr;
}
/**Determine if two iterators are the same.
\param other The other iterator.
\return True if both iterators point to the same slot in memory.*/
template<typename DataType, bool Const, bool Reverse>
inline bool ArrayIterator<DataType, Const, Reverse>::operator==(
	const SelfType & other) const
{
	return m_ptr == other.m_ptr;
}
/**Compare this iterator to another.
\param o The other iterator.
\return A true or false value that is the result of the arithmatic
comparison.*/
template<typename DataType, bool Const, bool Reverse>
inline bool ArrayIterator<DataType, Const, Reverse>::operator<(
	const SelfType & other)const
{
	if (Reverse)
		return m_ptr > other.m_ptr;
	else
		return m_ptr < other.m_ptr;
}
/**Compare this iterator to another.
\param o The other iterator.
\return A true or false value that is the result of the arithmatic
comparison.*/
template<typename DataType, bool Const, bool Reverse>
inline bool ArrayIterator<DataType, Const, Reverse>::operator<=(
	const SelfType & other)const
{
	return (*this < other) || (*this == other);
}
/**Compare this iterator to another.
\param o The other iterator.
\return A true or false value that is the result of the arithmatic
comparison.*/
template<typename DataType, bool Const, bool Reverse>
inline bool ArrayIterator<DataType, Const, Reverse>::operator>(
	const SelfType & other)const
{
	return !(*this <= other);
}
/**Compare this iterator to another.
\param o The other iterator.
\return A true or false value that is the result of the arithmatic
comparison.*/
template<typename DataType, bool Const, bool Reverse>
inline bool ArrayIterator<DataType, Const, Reverse>::operator>=(
	const SelfType & other)const
{
	return !(*this < other);
}
/**Increment this iterator by \p amt units.
\pre The object must point to something.
\param amt The amount to add.
\return A copy of the iterator, adjusted by \p amt units.*/
template<typename DataType, bool Const, bool Reverse>
inline typename ArrayIterator<DataType, Const, Reverse>::SelfType
ArrayIterator<DataType, Const, Reverse>::operator+(const SizeType & amt) const
{
	CheckAndThrow();
	auto copy = *this;
	copy += amt;
	return copy;
}
/**Increment this iterator by \p amt units.
\pre The object must point to something.
\param amt The amount to add.
\return A reference to the iterator after adjusting.*/
template<typename DataType, bool Const, bool Reverse>
inline typename ArrayIterator<DataType, Const, Reverse>::SelfType &
ArrayIterator<DataType, Const, Reverse>::operator+=(const SizeType & amt)
{
	CheckAndThrow();
	if (Reverse)
		m_ptr -= amt;
	else
		m_ptr += amt;
	return *this;
}
/**Decrememnt this iterator by \p amt amount.
\pre The object must point to something.
\param amt The amount to decrement.
\return A copy of the iterator after adjustment.*/
template<typename DataType, bool Const, bool Reverse>
inline typename ArrayIterator<DataType, Const, Reverse>::SelfType
ArrayIterator<DataType, Const, Reverse>::operator-(const SizeType & amt) const
{
	auto copy = *this;
	copy -= amt;
	return copy;
}
/**Perform iterator subtraction on this and another iterator.
\pre The object must point to something.
\param it The other iterator to subtract.
\return A copy of the iterator after adjustment.*/
template<typename DataType, bool Const, bool Reverse>
inline DiffType
ArrayIterator<DataType, Const, Reverse>::operator-(const SelfType & it) const
{
	return m_ptr - it.m_ptr;
}
/**Decrememnt this iterator by \p amt amount.
\pre The object must point to something.
\param amt The amount to decrement.
\return A reference to the iterator after adjusting*/
template<typename DataType, bool Const, bool Reverse>
inline typename ArrayIterator<DataType, Const, Reverse>::SelfType &
ArrayIterator<DataType, Const, Reverse>::operator-=(const SizeType & amt)
{
	CheckAndThrow();
	if (Reverse)
		m_ptr += amt;
	else
		m_ptr -= amt;
	return *this;
}
/**Dereference the iterator with an offset.
\pre The object must point to something.
\param i The ofset to adjust by.
\return A reference to the data ofset by \p i.*/
template<typename DataType, bool Const, bool Reverse>
inline typename ArrayIterator<DataType, Const, Reverse>::Ref
ArrayIterator<DataType, Const, Reverse>::operator[](const DiffType & i)
{
	if (Reverse)
		return m_ptr[-i];
	else
		return m_ptr[i];
}
/**Dereference the iterator with an offset.
\pre The object must point to something.
\param i The ofset to adjust by.
\return A reference to the data ofset by \p i.*/
template<typename DataType, bool Const, bool Reverse>
inline const typename ArrayIterator<DataType, Const, Reverse>::Ref 
ArrayIterator<DataType, Const, Reverse>::operator[](const DiffType & i) const
{
	if (Reverse)
		return m_ptr[-i];
	else
		return m_ptr[i];
}
/**Get the raw address of the data in the iterator.
\return The address of the stored data.*/
template<typename DataType, bool Const, bool Reverse>
inline typename ArrayIterator<DataType, Const, Reverse>::Ptr 
ArrayIterator<DataType, Const, Reverse>::Addr()
{
    return m_ptr;
}
/**Get the raw address of the data in the iterator.
\return The address of the stored data.*/
template<typename DataType, bool Const, bool Reverse>
inline const  typename ArrayIterator<DataType, Const, Reverse>::Ptr
ArrayIterator<DataType, Const, Reverse>::Addr() const
{
    return m_ptr;
}
/**Check for validity*/
template<typename DataType, bool Const, bool Reverse>
inline void ArrayIterator<DataType, Const, Reverse>::CheckAndThrow() const
{
	if (!m_ptr)
		throw ArrayIteratorException::NotDereferenceable;
}

template class ArrayIterator<int, true, true>;
template class ArrayIterator<int, false, true>;
template class ArrayIterator<int, false, false>;
template class ArrayIterator<int, true, false>;


}