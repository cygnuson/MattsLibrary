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

#include "LinkedListDef.hpp"


namespace cg {
/**Copy during construction.
\param o The other list to copy.*/
template<typename DataType>
inline LinkedList<DataType>::LinkedList(const LinkedList<DataType>& o)
{
    *this = o;
}
/**Move during construction.
\param o The other list to move.*/
template<typename DataType>
inline LinkedList<DataType>::LinkedList(LinkedList<DataType>&& o)
{
    *this = Forward<LinkedList<DataType>>(o);
}
/**Cosntruct with a set of iterators.
\param begin The first iterator
\param end The one past the end iterator.*/
template<typename DataType>
inline LinkedList<DataType>::LinkedList(Iterator begin, Iterator end)
{
    for (; begin != end; ++begin)
        EmplaceBack(*begin);
}
/**Cosntruct with a set of iterators.
\param begin The first iterator
\param end The one past the end iterator.*/
template<typename DataType>
inline LinkedList<DataType>::LinkedList(ReverseIterator begin,
    ReverseIterator end)
{
    for (; begin != end; ++begin)
        EmplaceFront(*begin);
}
/**Copy during assignment.
\param o The other list to copy.
\return A reference to this object.*/
template<typename DataType>
inline LinkedList<DataType> & LinkedList<DataType>::operator=(
    const LinkedList<DataType>& o)
{
    if (this == &o)
        return *this;
    auto it = o.Begin();
    auto end = o.End();
    for (; it != end; ++it)
        EmplaceBack(*it);
    return *this;
}
/**Move during assignment.
\param o The other list to move.
\return A reference to this object.*/
template<typename DataType>
inline LinkedList<DataType> & LinkedList<DataType>::operator=(
    LinkedList<DataType>&& o)
{
    if (this == &o)
        return *this;
    m_first = o.m_first;
    m_last = o.m_last;
    o.m_first = nullptr;
    o.m_last = nullptr;
    return *this;
}

/**Create a list from a pointer of data.
\param ptr A pointer to an array of data.
\param size THe amount of units in the array \p ptr.*/
template<typename DataType>
inline LinkedList<DataType>::LinkedList(const DataType * ptr, SizeType size)
{
    for (SizeType i = 0; i < size; ++i)
        PushBack(Forward<DataType>(DataType(ptr[i])));
}
/**Determine if this list is empty.
\return True if the list has no elements.*/
template<typename DataType>
inline cg::SizeType LinkedList<DataType>::Empty() const
{
    return m_first == nullptr;
}
template<typename DataType>
inline void LinkedList<DataType>::PushBack(DataType && o)
{
    if (!m_last)
    {
        m_first = new LinkedListNode<DataType>(Forward<DataType>(o),
            nullptr, nullptr);
        m_last = m_first;
        return;
    }
    auto oldLast = m_last;
    m_last = new LinkedListNode<DataType>(Forward<DataType>(o),
        nullptr, oldLast);
    oldLast->m_next = m_last;
}
/**Push an object to the front of the list.
\param o The object to push to the front of the list.*/
template<typename DataType>
inline void LinkedList<DataType>::PushFront(DataType && o)
{
    if (!m_first)
    {
        m_first = new LinkedListNode<DataType>(Forward<DataType>(o),
            nullptr, nullptr);
        m_last = m_first;
        return;
    }
    auto oldFirst = m_first;
    m_first = new LinkedListNode<DataType>(Forward<DataType>(o),
        oldFirst, nullptr);
    oldFirst->m_prev = m_first;
}
/**Emplace data onto the back of the list.
\param o The args to emplace with.*/
template<typename DataType>
template<typename ...Ts>
inline void LinkedList<DataType>::EmplaceBack(Ts && ...o)
{
    if (!m_first)
    {
        m_first = new LinkedListNode<DataType>();
        new (&m_first->m_data) DataType(Forward<Ts>(o)...);
        m_last = m_first;
        return;
    }
    auto oldLast = m_last;
    m_last = new LinkedListNode<DataType>();
    new (&m_last->m_data) DataType(Forward<Ts>(o)...);
    oldLast->m_next = m_last;
    m_last->m_prev = oldLast;
}
/**Emplace data onto the front of the list.
\param o The args to emplace with.*/
template<typename DataType>
template<typename ...Ts>
inline void LinkedList<DataType>::EmplaceFront(Ts && ...o)
{
    if (!m_first)
    {
        m_first = new LinkedListNode<DataType>();
        new (&m_first->m_data) DataType(Forward<Ts>(o)...);
        m_last = m_first;
        return;
    }
    auto oldFirst = m_first;
    m_first = new LinkedListNode<DataType>();
    new (&m_first->m_data) DataType(Forward<Ts>(o)...);
    oldFirst->m_prev = m_first;
    m_first->m_next = oldFirst;
}
/**Get an iterator to the first element.
\return An iterator to the firs element.*/
template<typename DataType>
inline typename LinkedList<DataType>::Iterator
LinkedList<DataType>::Begin()
{
    return LinkedList<DataType>::Iterator(m_first);
}
/**Get an iterator to the first element.
\return An iterator to the firs element.*/
template<typename DataType>
inline typename LinkedList<DataType>::ConstIterator
LinkedList<DataType>::Begin() const
{
    return LinkedList<DataType>::ConstIterator(m_first);
}
/**Get a reverse iterator to the last element.
\return a reverse iterator to the last element*/
template<typename DataType>
inline typename LinkedList<DataType>::ReverseIterator
LinkedList<DataType>::RBegin()
{
    return LinkedList<DataType>::ReverseIterator(m_last);
}
/**Get a reverse iterator to the last element.
\return a reverse iterator to the last element*/
template<typename DataType>
inline typename LinkedList<DataType>::ConstReverseIterator
LinkedList<DataType>::RBegin() const
{
    return LinkedList<DataType>::ConstReverseIterator(m_last);
}
/**Get an end iterator that acts as the last iterator in a seuence.
\return A end iterator*/
template<typename DataType>
inline typename LinkedList<DataType>::Iterator
LinkedList<DataType>::End()
{
    return LinkedList<DataType>::Iterator();
}

/**Get an end iterator that acts as the last iterator in a seuence.
\return A end iterator*/
template<typename DataType>
inline typename LinkedList<DataType>::ConstIterator
LinkedList<DataType>::End() const
{
    return LinkedList<DataType>::ConstIterator();
}

/**Get an end iterator that acts as the last iterator in a seuence.
\return A end iterator*/
template<typename DataType>
inline typename LinkedList<DataType>::ReverseIterator
LinkedList<DataType>::REnd()
{
    return LinkedList<DataType>::ReverseIterator();
}

/**Get an end iterator that acts as the last iterator in a seuence.
\return A end iterator*/
template<typename DataType>
inline typename LinkedList<DataType>::ConstReverseIterator
LinkedList<DataType>::REnd() const
{
    return LinkedList<DataType>::ConstReverseIterator();
}

/**Remove the last element in the list.
\throw LinkedListException::IndexOutOfBounds if the list is empty.*/
template<typename DataType>
inline void LinkedList<DataType>::PopBack()
{
    if (!m_first)
        throw LinkedListException::IndexOutOfBounds;
    if (!m_last->m_prev)
    {
        delete m_last;
        m_last = nullptr;
        m_first = nullptr;
    }
    else
    {
        auto old = m_last->m_prev;
        delete m_last;
        m_last = old;
        m_last->m_next = nullptr;
    }
}
/**Remove the first element in the list.
\throw LinkedListException::IndexOutOfBounds if the list is empty.*/
template<typename DataType>
inline void LinkedList<DataType>::PopFront()
{
    if (!m_first)
        throw LinkedListException::IndexOutOfBounds;

    if (!m_first)
        throw LinkedListException::IndexOutOfBounds;
    if (!m_first->m_next)
    {
        delete m_last;
        m_last = nullptr;
        m_first = nullptr;
    }
    else
    {
        auto old = m_first->m_next;
        delete m_first;
        m_first = old;
        m_first->m_prev = nullptr;
    }
}
/**Push an object to the position after the iterator \p it.
\param it The iterator to be before the newly inserted.
\param d The object to push.
\return An iterator for the new inserted data.*/
template<typename DataType>
inline typename LinkedList<DataType>::ReverseIterator
LinkedList<DataType>::PushAfter(ReverseIterator & it, DataType && d)
{
    if (!it)
        throw LinkedListException::InvalidIterator;
    auto it2 = Push(Iterator(it.m_ptr), Forward<DataType>(d));
    return ReverseIterator(it2.m_ptr);
}
/**Push an object to the position after the iterator \p it.
\param it The iterator to be before the newly inserted.
\param d The object to push.
\return An iterator for the new inserted data.*/
template<typename DataType>
inline typename LinkedList<DataType>::Iterator
LinkedList<DataType>::PushAfter(Iterator & it, DataType && d)
{
    if (!it)
        throw LinkedListException::InvalidIterator;
    auto n = new LinkedListNode<DataType>(Forward<DataType>(d),
        it.m_ptr->m_next, it.m_ptr);
    it.m_ptr->m_next = n;
    n->m_next->m_prev = n;
    return Iterator(n);
}
/**Push an object to the position of the iterator \p it.
\param it The iterator to be after the newly inserted.
\param d The object to push.
\return An iterator for the new inserted data.*/
template<typename DataType>
inline typename LinkedList<DataType>::ReverseIterator
LinkedList<DataType>::Push(ReverseIterator & it, DataType && d)
{
    if (!it)
        throw LinkedListException::InvalidIterator;
    auto it2 = PushAfter(Iterator(it.m_ptr), Forward<DataType>(d));
    return ReverseIterator(it2.m_ptr);
}
/**Push an object to the position of the iterator \p it.
\param it The iterator to be after the newly inserted.
\param d The object to push.
\return An iterator for the new inserted data.*/
template<typename DataType>
inline typename LinkedList<DataType>::Iterator
LinkedList<DataType>::Push(Iterator & it, DataType && d)
{
    if (!it)
        throw LinkedListException::InvalidIterator;
    --it;
    return PushAfter(it, Forward<DataType>(d));
}
/**Get the item at the back of the list.
\return The item at the back of the list.*/
template<typename DataType>
inline const DataType & LinkedList<DataType>::Back() const
{
    return m_last->m_data;
}
/**Get the item at the back of the list.
\return The item at the back of the list.*/
template<typename DataType>
inline DataType & LinkedList<DataType>::Back()
{
    return m_last->m_data;
}
/**Get the item at the front of the list.
\return The item at the front of the list.*/
template<typename DataType>
inline const DataType & LinkedList<DataType>::Front() const
{
    return m_first->m_data;
}
/**Get the item at the front of the list.
\return The item at the front of the list.*/
template<typename DataType>
inline DataType & LinkedList<DataType>::Front()
{
    return m_first->m_data;
}
/**Emplace an object to the position after the iterator \p it.
\param it The iterator to be after the newly inserted.
\param d The object to push.
\return An iterator for the new inserted data.*/
template<typename DataType>
template<typename ...Ts>
inline typename LinkedList<DataType>::ReverseIterator
LinkedList<DataType>::EmplaceAfter(ReverseIterator & it, Ts && ...ts)
{
    if (!it)
        throw LinkedListException::InvalidIterator;
    auto it = Emplace(Iterator(it.m_ptr), Forward<Ts>(ts)...);
    return ReverseIterator(it.m_ptr);
}
/**Emplace an object to the position after the iterator \p it.
\param it The iterator to be after the newly inserted.
\param d The object to push.
\return An iterator for the new inserted data.*/
template<typename DataType>
template<typename ...Ts>
inline typename LinkedList<DataType>::Iterator
LinkedList<DataType>::EmplaceAfter(Iterator & it, Ts && ...ts)
{
    if (!it)
        throw LinkedListException::InvalidIterator;
    auto n = new LinkedListNode<DataType>(it.m_ptr->m_next, it.m_ptr);
    new (&n->m_data) DataType(Forward<Ts>(ts)...);
    it.m_ptr->m_next = n;
    n->m_next->m_prev = n;
    return Iterator(n);
}
/**Emplace an object to the position of the iterator \p it.
\param it The iterator to be after the newly inserted.
\param d The object to push.
\return An iterator for the new inserted data.*/
template<typename DataType>
template<typename ...Ts>
inline typename LinkedList<DataType>::ReverseIterator
LinkedList<DataType>::Emplace(ReverseIterator & it, Ts && ...ts)
{
    if (!it)
        throw LinkedListException::InvalidIterator;
    auto it = EmplaceAfter(Iterator(it.m_ptr), Forward<Ts>(ts)...);
    return ReverseIterator(it.m_ptr);
}
/**Emplace an object to the position of the iterator \p it.
\param it The iterator to be after the newly inserted.
\param d The object to push.
\return An iterator for the new inserted data.*/
template<typename DataType>
template<typename ...Ts>
inline typename LinkedList<DataType>::Iterator
LinkedList<DataType>::Emplace(Iterator & it, Ts && ...ts)
{
    if (!it)
        throw LinkedListException::InvalidIterator;
    --it;
    return EmplaceAfter(it, Forward<Ts>(ts)...);
}

}