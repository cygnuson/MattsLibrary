/*

(C) Matthew Swanson

This file is part of _PROJECT_NAME_.

_PROJECT_NAME_ is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
any later version.

_PROJECT_NAME_ is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with _PROJECT_NAME_.  If not, see <http://www.gnu.org/licenses/>.

*/

#pragma once

#include "LinkedListIteratorDef.hpp"


namespace cg {

/**Create an iterator with a pointer to a node.
\param p A pointer to a node of type LinkedListNode.*/
template<typename DataType, bool Const, bool Reverse> inline
LinkedListIterator<DataType, Const, Reverse>::LinkedListIterator(
    LinkedListNode<DataType>* p) : m_ptr(p)
{

}

/**Copy antoher iterator to this spot.
\param other The other iterator.*/
template<typename DataType, bool Const, bool Reverse>
inline LinkedListIterator<DataType, Const, Reverse>::LinkedListIterator(
    const SelfType & other)
{
    *this = other;
}

/**Copy antoher iterator to this spot.
\param other The other iterator.*/
template<typename DataType, bool Const, bool Reverse>
inline typename LinkedListIterator<DataType, Const, Reverse>::SelfType &
LinkedListIterator<DataType, Const, Reverse>::operator=(const SelfType & other)
{
    if (this == &other)
        return *this;
    m_ptr = other.m_ptr;
    return *this;
}

/**Move antoher iterator to this spot.
\param other The other iterator.*/
template<typename DataType, bool Const, bool Reverse>
inline LinkedListIterator<DataType, Const, Reverse>::LinkedListIterator(
    SelfType && other) :m_ptr(nullptr)
{
    *this = Move(other);
}
/**Move antoher iterator to this spot.
\param other The other iterator.*/
template<typename DataType, bool Const, bool Reverse>
inline typename LinkedListIterator<DataType, Const, Reverse>::SelfType  &
LinkedListIterator<DataType, Const, Reverse>::operator=(SelfType && other)
{
    if (this == &other)
        return *this;
    m_ptr = other.m_ptr;
#if _DEBUG
    other.m_ptr = nullptr;
#endif
    return *this;
}
/**Validate this iterator.
\return True if the iterator can be dereference, false otherwise.*/
template<typename DataType, bool Const, bool Reverse>
inline LinkedListIterator<DataType, Const, Reverse>::operator bool() const
{
    return m_ptr;
}
/**Dereferenec this iterator to get access to the data inside.
\return A reference to the data the iterator reprisents.*/
template<typename DataType, bool Const, bool Reverse>
inline typename LinkedListIterator<DataType, Const, Reverse>::Ref
LinkedListIterator<DataType, Const, Reverse>::operator*()
{
    CheckAndThrow();
    return m_ptr->m_data;
}
/**Dereferenec this iterator to get access to the data inside.
\return A reference to the data the iterator reprisents.*/
template<typename DataType, bool Const, bool Reverse>
inline const typename LinkedListIterator<DataType, Const, Reverse>::Ref
LinkedListIterator<DataType, Const, Reverse>::operator*() const
{
    CheckAndThrow();
    return m_ptr->m_data;
}
/**Access the data reference by this iterator.
\return A pointer to the data. The -> operator will be applied as far down as
it can be.*/
template<typename DataType, bool Const, bool Reverse>
inline typename LinkedListIterator<DataType, Const, Reverse>::Ptr
LinkedListIterator<DataType, Const, Reverse>::operator->()
{
    CheckAndThrow();
    return &m_ptr->m_data;
}
/**Access the data reference by this iterator.
\return A pointer to the data. The -> operator will be applied as far down as
it can be.*/
template<typename DataType, bool Const, bool Reverse>
inline const typename LinkedListIterator<DataType, Const, Reverse>::Ptr
LinkedListIterator<DataType, Const, Reverse>::operator->() const
{
    CheckAndThrow();
    return &m_ptr->m_data;
}

/**Incrememnt the iterator to its opposite direction.
\return A copy to this iterator after performing the operation.*/
template<typename DataType, bool Const, bool Reverse>
inline typename LinkedListIterator<DataType, Const, Reverse>::SelfType
LinkedListIterator<DataType, Const, Reverse>::operator++(int)
{
    auto copy = *this;
    operator++();
    return copy;
}

/**Incrememnt the iterator to its opposite direction.
\return A reference to this iterator after performing the operation.*/
template<typename DataType, bool Const, bool Reverse>
inline typename LinkedListIterator<DataType, Const, Reverse>::SelfType &
LinkedListIterator<DataType, Const, Reverse>::operator++()
{
    CheckAndThrow();
    if (Reverse)
        m_ptr = m_ptr->m_prev;
    else
        m_ptr = m_ptr->m_next;
    return *this;
}
/**Decrement the iterator to its opposite direction.
\return A copy to this iterator after performing the operation.*/
template<typename DataType, bool Const, bool Reverse>
inline typename LinkedListIterator<DataType, Const, Reverse>::SelfType
LinkedListIterator<DataType, Const, Reverse>::operator--(int)
{
    auto copy = *this;
    operator--();
    return copy;
}
/**Decrement the iterator to its opposite direction.
\return A reference to this iterator after performing the operation.*/
template<typename DataType, bool Const, bool Reverse>
inline typename LinkedListIterator<DataType, Const, Reverse>::SelfType &
LinkedListIterator<DataType, Const, Reverse>::operator--()
{
    CheckAndThrow();
    if (Reverse)
        m_ptr = m_ptr->m_next;
    else
        m_ptr = m_ptr->m_prev;
    return *this;
}
/**Do a comparison on the iterators.
\param other Another iterator of the same type.
\return True if the comparison evaluates to true.*/
template<typename DataType, bool Const, bool Reverse>
inline bool LinkedListIterator<DataType, Const, Reverse>::operator!=(
    const SelfType & other) const
{
    return m_ptr != other.m_ptr;
}
/**Do a comparison on the iterators.
\param other Another iterator of the same type.
\return True if the comparison evaluates to true.*/
template<typename DataType, bool Const, bool Reverse>
inline bool LinkedListIterator<DataType, Const, Reverse>::operator==(
    const SelfType & other) const
{
    return !(*this != other);

}
/**Do a comparison on the iterators.
\param other Another iterator of the same type.
\return True if the comparison evaluates to true.*/
template<typename DataType, bool Const, bool Reverse>
inline bool LinkedListIterator<DataType, Const, Reverse>::operator<(
    const SelfType & other)const
{
    CheckAndThrow();
    bool found = false;
    auto copy = *this;
    while (copy != *this)
    {
        try {
            --copy;
        }
        catch (LinkedListIteratorException::IndexOutOfBounds& ex)
        {
            return false;
        }
    }
    return true;
}
/**Do a comparison on the iterators.
\param other Another iterator of the same type.
\return True if the comparison evaluates to true.*/
template<typename DataType, bool Const, bool Reverse>
inline bool LinkedListIterator<DataType, Const, Reverse>::operator<=(
    const SelfType & other)const
{
    return (*this < other) || (*this == other);
}
/**Do a comparison on the iterators.
\param other Another iterator of the same type.
\return True if the comparison evaluates to true.*/
template<typename DataType, bool Const, bool Reverse>
inline bool LinkedListIterator<DataType, Const, Reverse>::operator>(
    const SelfType & other)const
{
    return !(*this <= other);
}
/**Do a comparison on the iterators.
\param other Another iterator of the same type.
\return True if the comparison evaluates to true.*/
template<typename DataType, bool Const, bool Reverse>
inline bool LinkedListIterator<DataType, Const, Reverse>::operator>=(
    const SelfType & other)const
{
    return !(*this < other);
}
/**Get an iterator offset in the current direction.
\param amt The amount to offset.
\return A new iterator that is offset.*/
template<typename DataType, bool Const, bool Reverse>
inline typename LinkedListIterator<DataType, Const, Reverse>::SelfType
LinkedListIterator<DataType, Const, Reverse>::operator+(
    const SizeType & amt) const
{
    auto copy = *this;
    copy += amt;
    return copy;
}

/**Move this iterator in the current direction \p amt units.
\param amt The amount to move.
\return A reference to this iterator after moving.*/
template<typename DataType, bool Const, bool Reverse>
inline typename LinkedListIterator<DataType, Const, Reverse>::SelfType &
LinkedListIterator<DataType, Const, Reverse>::operator+=(const SizeType & amt)
{
    CheckAndThrow();
    for (SizeType i = 0; i < amt; ++i)
    {
        if (!Reverse)
            operator++();
        else
            operator--();
    }
}
/**Get an iterator offset in the opposide direction of the current direction.
\param amt The amount to offset.
\return A new iterator that is offset.*/
template<typename DataType, bool Const, bool Reverse>
inline typename LinkedListIterator<DataType, Const, Reverse>::SelfType
LinkedListIterator<DataType, Const, Reverse>::operator-(
    const SizeType & amt) const
{
    auto copy = *this;
    copy -= amt;
    return copy;
}
/**Move this iterator in the opposite of the current direction \p amt units.
\param amt The amount to move.
\return A reference to this iterator after moving.*/
template<typename DataType, bool Const, bool Reverse>
inline typename LinkedListIterator<DataType, Const, Reverse>::SelfType &
LinkedListIterator<DataType, Const, Reverse>::operator-=(const SizeType & amt)
{
    CheckAndThrow();
    for (SizeType i = 0; i < amt; ++i)
    {
        if (!Reverse)
            operator--();
        else
            operator++();
    }
}

/**Get element that is offset from the current iterator.
\param i The amount to offset.
\return A reference to the data at offset \p i.*/
template<typename DataType, bool Const, bool Reverse>
inline typename LinkedListIterator<DataType, Const, Reverse>::Ref
LinkedListIterator<DataType, Const, Reverse>::operator[](const DiffType & i)
{
    CheckAndThrow();
    LinkedListNode<DataType> p = m_ptr;
    for (SizeType c = 0; c < i; ++c)
        if (p->m_next == nullptr)
            throw LinkedListIteratorException::IndexOutOfBounds;
        else
            p = p->m_next;
    return m_ptr->m_data;
}
/**Get element that is offset from the current iterator.
\param i The amount to offset.
\return A reference to the data at offset \p i.*/
template<typename DataType, bool Const, bool Reverse>
inline const typename LinkedListIterator<DataType, Const, Reverse>::Ref &
LinkedListIterator<DataType, Const, Reverse>::operator[](
    const DiffType & i) const
{
    CheckAndThrow();
    LinkedListNode<DataType> p = m_ptr;
    for (SizeType c = 0; c < i; ++c)
        if (p->m_next == nullptr)
            throw LinkedListIteratorException::IndexOutOfBounds;
        else
            p = p->m_next;
    return m_ptr->m_data;
}

/**Get the internal address of the thing pointed to by the iterator.
\return A pointer to the address of the data.*/
template<typename DataType, bool Const, bool Reverse>
inline typename LinkedListIterator<DataType, Const, Reverse>::Ptr
LinkedListIterator<DataType, Const, Reverse>::Addr()
{
    CheckAndThrow();
    return &m_ptr->m_data;
}
/**Get the internal address of the thing pointed to by the iterator.
\return A pointer to the address of the data.*/
template<typename DataType, bool Const, bool Reverse>
inline const typename LinkedListIterator<DataType, Const, Reverse>::Ptr
LinkedListIterator<DataType, Const, Reverse>::Addr() const
{
    CheckAndThrow();
    return &m_ptr->m_data;
}
/**Ensure that the iterator is valid.
\throw LinkedListIteratorException::NotDereferenceable if the iterator is not
valid.*/
template<typename DataType, bool Const, bool Reverse>
inline void
LinkedListIterator<DataType, Const, Reverse>::CheckAndThrow() const
{
    if (m_ptr == nullptr)
        throw LinkedListIteratorException::NotDereferenceable;
}


}