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

#include "ArrayDef.hpp"


namespace cg {

/**default ctor*/
template<typename DataType, cg::SizeType SizeP>
Storage<DataType, SizeP>::Storage(cg::SizeType) :m_cap(SizeP), m_size(0)
{

};
/**Move ctor, to make it work with
\param other The thing to move.*/
template<typename DataType, cg::SizeType SizeP>
Storage<DataType, SizeP>::Storage(SelfType&& other)
    : m_cap(SizeP), m_size(other.m_size)
{
    for (cg::SizeType i = 0; i < other.m_size; ++i)
        new (Addr() + i) DataType(Move(other.Addr()[i]));
#if _DEBUG
    other.m_size = 0;
#endif
}
/**Move assign.
\param other The thing to move.*/
template<typename DataType, cg::SizeType SizeP>
void Storage<DataType, SizeP>::operator=(SelfType&& other)
{
    m_size = Move(other.m_size);
    for (cg::SizeType i = 0; i < other.m_size; ++i)
        new (Addr() + i) DataType(Move(other.Addr()[i]));
#if _DEBUG
    other.m_size = 0;
#endif
}
/**Create with an array of things.
\param arr The array to add.
\param aSize The size of the array.*/
template<typename DataType, cg::SizeType SizeP>
Storage<DataType, SizeP>::Storage(const DataType* arr, cg::SizeType aSize)
{
    if (aSize > SizeP)
        throw ArrayException::InvalidParameter;
    auto end = arr + aSize;
    for (cg::SizeType i = 0; arr != end; ++arr)
        Emplace(i++, *arr);
}
/**Create the int with initial values.

If SizeP != 0:
only the first SizeP values will be used.
If amount of values < SizeP the rest will be set to 0.
If SizeP == 0
All values will be inserted to the storage.

\param vals The values to insert.*/
template<typename DataType, cg::SizeType SizeP>
Storage<DataType, SizeP>::Storage(std::initializer_list<DataType>&& vals)
    :m_cap(SizeP)
{
    cg::SizeType smaller = vals.size() < SizeP ? vals.size() : SizeP;
    for (cg::SizeType i = 0; i < smaller; ++i)
        new (Addr() + i) DataType(Move(*(vals.begin() + i)));
    m_size = smaller;
}
/**Determine if another element can be inserted.
\return True if an insert now would NOT throw an exception.*/
template<typename DataType, cg::SizeType SizeP>
bool Storage<DataType, SizeP>::CanInsert() const
{
    if (m_size == m_cap)
        return false;
    return true;
}
/**Push an object to an index.
\param i The place to put the object.
\param o The thing to push.*/
template<typename DataType, cg::SizeType SizeP>
template<typename NType>
void Storage<DataType, SizeP>::Insert(cg::SizeType i, NType&& o)
{
    using U = std::decay_t<NType>;
    if (i > m_size)
        throw ArrayException::IndexOutOfBounds;
    if (m_size == m_cap)
        throw ArrayException::ArrayIsFull;
    if (m_size != i)
        std::memmove(Addr() + i + 1, Addr() + i,
            sizeof(NType)*(m_size - i));
    new (Addr() + i)U(Forward<NType>(o));
    ++m_size;
}
/**Emplace an object to an index.
\param i The place to put the object.
\param nums The args to send to the ctor of type T.*/
template<typename DataType, cg::SizeType SizeP>
template<typename...Ts>
void Storage<DataType, SizeP>::Emplace(cg::SizeType i, Ts&&... nums)
{
    if (i > m_size)
        throw std::runtime_error("Index out of bounds.");
    if (m_size == m_cap)
        throw std::runtime_error("The list is full.");
    if (m_size != i)
        std::memmove(Addr() + i + 1, Addr() + i,
            sizeof(DataType)*(m_size - i));
    new (Addr() + i)DataType(Forward<Ts>(nums)...);
    ++m_size;
}
/**Get the address of the data.
\param i the offset.
\return The address of the data.*/
template<typename DataType, cg::SizeType SizeP>
DataType* Storage<DataType, SizeP>::Addr(cg::SizeType i)
{
    return ((DataType*)(m_data + (i * sizeof(DataType))));
}
/**Get the address of the data.
\param i the offset.
\return The address of the data.*/
template<typename DataType, cg::SizeType SizeP>
const DataType* Storage<DataType, SizeP>::Addr(cg::SizeType i)const
{
    return ((const DataType*)(m_data + (i * sizeof(DataType))));
}


///////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////HEAP HERE//////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

    /**default ctor
    \param cap The initial capacity.*/
template<typename DataType>
Storage<DataType, 0>::Storage(cg::SizeType cap) :m_cap(0), m_size(0),
m_data(nullptr)
{
    if (cap == 0)
        m_data = nullptr;
    else
        ExpandTo(cap);
};
/**Create with an array of things.
\param arr The array to add.
\param aSize The size of the array.*/
template<typename DataType>
Storage<DataType, 0>::Storage(const DataType* arr, cg::SizeType aSize)
    :m_cap(0), m_size(0)
{
    ExpandTo(aSize);
    auto end = arr + aSize;
    for (cg::SizeType i = 0; arr != end; ++arr)
        Emplace(i++, *arr);
}
/**Move ctor
\param other The thing to move.*/
template<typename DataType>
Storage<DataType, 0>::Storage(SelfType&& other)
    :m_cap(other.m_cap), m_data(other.m_data), m_size(other.m_size)
{
#ifdef _DEBUG
    other.m_data = nullptr;
    other.m_size = 0;
#endif
};
/**Create the int with initial values.

If SizeP != 0:
only the first SizeP values will be used.
If amount of values < SizeP the rest will be set to 0.
If SizeP == 0
All values will be inserted to the storage.

\param vals The values to insert.*/
template<typename DataType>
Storage<DataType, 0>::Storage(std::initializer_list<DataType>&& vals)
{
    cg::SizeType sz = vals.size();
    ExpandTo(sz);
    for (cg::SizeType i = 0; i < sz; ++i)
        new (Addr() + i) DataType(Move(*(vals.begin() + i)));
    m_size = sz;
    m_cap = sz;
}
/**Move op
\param other The thing to move.*/
template<typename DataType>
void Storage<DataType, 0>::operator=(SelfType&& other)
{
    m_data = other.m_data;
#ifdef _DEBUG
    other.m_data = nullptr;
    other.m_size = 0;
#endif
    m_size = other.m_size;
    m_cap = other.m_cap;
}
/**Clean up the data.*/
template<typename DataType>
Storage<DataType, 0>::~Storage()
{
    if (m_data)
        free(m_data);
}
/**Determine if another element can be inserted.
\return True if an insert now would NOT throw an exception.*/
template<typename DataType>
bool Storage<DataType, 0>::CanInsert() const
{
    return true;
}
/**Push an object to an index.
\param i The place to put the object.
\param o The thing to push.*/
template<typename DataType>
template<typename NType>
void Storage<DataType, 0>::Insert(cg::SizeType i, NType&& o)
{
    using U = std::decay_t<NType>;
    if (i > m_size)
        throw ArrayException::IndexOutOfBounds;
    if (m_size == m_cap)
        ExpandTo(m_cap + ExpandAmount);
    if (m_size != i)
        std::memmove(Addr() + i + 1, Addr() + i,
            sizeof(NType)*(m_size - i));
    new (Addr() + i)U(Forward<NType>(o));
    ++m_size;
}
/**Emplace an object to an index.
\param i The place to put the object.
\param nums The args to send to the ctor of type T.*/
template<typename DataType>
template<typename...Ts>
void Storage<DataType, 0>::Emplace(cg::SizeType i, Ts&&... nums)
{
    if (i > m_size)
        throw ArrayException::IndexOutOfBounds;
    if (m_size == m_cap)
        ExpandTo(m_cap + ExpandAmount);
    if (m_size != i)
        std::memmove(Addr() + i + 1, Addr() + i,
            sizeof(DataType)*(m_size - i));
    new (Addr() + i)DataType(Forward<Ts>(nums)...);
    ++m_size;
}
///////////////////////////////////////////////////////////////////////////
/**Expand the array to X amount of elements.
\param amt The amount to hold. If m_cap is >=, nothing happens.*/
template<typename DataType>
void Storage<DataType, 0>::ExpandTo(cg::SizeType amt)
{
    if (m_cap >= amt)
        return;
    if (!m_data)
    {
        /**Dont initialize...*/
        m_data = (DataType*)std::malloc(sizeof(DataType)*amt);
        m_cap = amt;
        return;
    }
    /**Dont initialize...*/
    DataType* nData = (DataType*)std::malloc(sizeof(DataType)*amt);
    //std::memmove(nData, Addr(), sizeof(DataType) * m_size);
    for (SizeType i = 0; i < m_size; ++i)
        new (nData + i) DataType(Move(m_data[i]));
    m_cap = amt;

    free(m_data);
    m_data = nData;
}
/**Get the address of the data.
\param i the offset.
\return The address of the data.*/
template<typename DataType>
DataType* Storage<DataType, 0>::Addr(cg::SizeType i)
{
    if (!m_data)
        throw ArrayException::ListEmpty;
    return m_data + i;
}
/**Get the address of the data.
\param i the offset.
\return The address of the data.*/
template<typename DataType>
const DataType* Storage<DataType, 0>::Addr(cg::SizeType i)const
{
    return m_data + i;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////LIST HERE//////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

    /**Create the list.
    \param initCap The initial capacity to start with.  Not relevent for
    SizeP > 0.*/
template<typename DataType, cg::SizeType SizeP>
Array<DataType, SizeP>::Array(cg::SizeType initCap) : Storage(initCap) {};
/**Create the list with cap of 8.*/
template<typename DataType, cg::SizeType SizeP>
Array<DataType, SizeP>::Array() : Storage(8) {};
/**Create with an array of things.
\param arr The array to add.
\param aSize The size of the array.*/
template<typename DataType, cg::SizeType SizeP>
Array<DataType, SizeP>::Array(const DataType* arr, cg::SizeType aSize)
    :Storage(arr, aSize) {}
/**Move ctor
\param other The thing to move.*/
template<typename DataType, cg::SizeType SizeP>
Array<DataType, SizeP>::Array(SelfType&& other)
    : Storage(Move(other)) {};
/**Move assign.
\param other The thing to move.*/
template<typename DataType, cg::SizeType SizeP>
void Array<DataType, SizeP>::operator=(SelfType&& other)
{
    Storage::operator=(Move(other));
}
/**Create the int with initial values.

If SizeP != 0:
only the first SizeP values will be used.
If amount of values < SizeP the rest will be set to 0.
If SizeP == 0
All values will be inserted to the storage.

\param vals The values to insert.*/
template<typename DataType, cg::SizeType SizeP>
Array<DataType, SizeP>::Array(std::initializer_list<DataType>&& vals)
    :Storage(Forward<std::initializer_list<DataType>>(vals))
{

}
/**Copy from another array.
\param other The other array to copy.*/
template<typename DataType, cg::SizeType SizeP>
template<typename DataType2, cg::SizeType SizeP2>
Array<DataType, SizeP>::Array(const Array<DataType2, SizeP2>& other)
    :Storage(ExpandAmount) /*storage is set with expandamount incase its heap
                           allocated. If not,  it does not  matter anyway.*/
{
    *this = other;
}
/**Copy from another array.
\param other The other array to copy.*/
template<typename DataType, cg::SizeType SizeP>
Array<DataType, SizeP>::Array(const SelfType& other)
    :Storage(ExpandAmount) /*storage is set with expandamount incase its heap
                           allocated. If not,  it does not  matter anyway.*/
{
    *this = other;
}
/**Copy from another array.
\param other The other array to copy.*/
template<typename DataType, cg::SizeType SizeP>
template<typename DataType2, cg::SizeType SizeP2>
Array<DataType, SizeP>&
Array<DataType, SizeP>::operator=(const Array<DataType2, SizeP2>& other)
{
    const static bool Check1 = SizeP2 == 0 && SizeP == 0;
    const static bool Check2 = SizeP2 <= SizeP;
    const static bool Check3 = SizeP == 0;
    static_assert(Check1 || Check2 || Check3, "The array sizes are"
        " incompatible.");
    auto beg = other.Begin();
    auto end = other.End();
    for (; beg != end; ++beg)
        PushBack(Forward<DataType>(DataType(*beg)));
    return *this;
}
/**Copy from another array.
\param other The other array to copy.*/
template<typename DataType, cg::SizeType SizeP>
Array<DataType, SizeP>&
Array<DataType, SizeP>::operator=(const SelfType& other)
{
    auto beg = other.Begin();
    auto end = other.End();
    for (; beg != end; ++beg)
        PushBack(Forward<DataType>(DataType(*beg)));
    return *this;
}
/**Create the object with a begin and end.
\param beg The first poitner.
\param end One-past-last pointer.*/
template<typename DataType, cg::SizeType SizeP>
Array<DataType, SizeP>::Array(DataType* beg, DataType* end)
{
    cg::SizeType sz = end - beg;
    if (sz < 1)
        throw ArrayException::InvalidParameter;
    for (cg::SizeType i = 0; i < sz; ++i)
        PushBack(Forward<DataType>(*(beg + i)));
}
/**Set all available space. This will fill memory that is allocated, but
not marked as "used".
\param x The thing to set the space too.*/
template<typename DataType, cg::SizeType SizeP>
void Array<DataType, SizeP>::FillUnused(const DataType& x)
{
    auto p = Begin().Addr();
    for (cg::SizeType i = m_size; i < m_cap; ++i)
        new ((p + i))DataType(x);
}
/**Get the size of the storage.
\return The amount of elements.*/
template<typename DataType, cg::SizeType SizeP>
cg::SizeType Array<DataType, SizeP>::Size() const
{
    return m_size;
}
/**Emplace an object to the back of the list.
\param o The object.*/
template<typename DataType, cg::SizeType SizeP>
template<typename...Ts>
void Array<DataType, SizeP>::EmplaceBack(Ts&&... o)
{
    Emplace(m_size, Forward<Ts>(o)...);
}
/**Emplace an object to the back of the list.
\param o The object.*/
template<typename DataType, cg::SizeType SizeP>
template<typename...Ts>
void Array<DataType, SizeP>::EmplaceFront(Ts&&... o)
{
    Emplace(0, Forward<Ts>(o)...);
}
/**Emplace an element after the position \p it.  \p it will point to the same
element as it did before.
\param it The iterator to emplace at.
\param ts A parameter pack of arguments to send to the constructor
of \p DataType.
\return An iterator the the new element.*/
template<typename DataType, cg::SizeType SizeP>
template<typename ...Ts>
inline typename Array<DataType, SizeP>::ReverseIterator
Array<DataType, SizeP>::EmplaceAfter(ReverseIterator& it, Ts && ...ts)
{
    SizeType position = m_size - (RBegin() - it) - 1;
    Storage<DataType, SizeP>::Emplace(position, Forward<Ts>(ts)...);
    //move it back to its element.
    it = cg::ArrayIterator<DataType, false, true>(Addr() + (position + 1));
    auto ret = cg::ArrayIterator<DataType, false, true>(Addr() + position);
    return ret;
}
/**Emplace an element after the position \p it.  \p it will point to the same
element as it did before.
\param it The iterator to emplace at.
\param ts A parameter pack of arguments to send to the constructor
of \p DataType.
\return An iterator the the new element.*/
template<typename DataType, cg::SizeType SizeP>
template<typename ...Ts>
inline typename Array<DataType, SizeP>::Iterator
Array<DataType, SizeP>::EmplaceAfter(Iterator& it, Ts && ...ts)
{
    SizeType position = (it - Begin()) + 1;
    Storage<DataType, SizeP>::Emplace(position, Forward<Ts>(ts)...);
    //move it back to its element.
    it = cg::ArrayIterator<DataType, false, false>(Addr() + (position - 1));
    auto ret = cg::ArrayIterator<DataType, false, false>(Addr() + position);
    return ret;
}
/**Emplace an element at the position \p it.  \p it will point to the same
element as it did before.
\param it The iterator to emplace at.
\param ts A parameter pack of arguments to send to the constructor
of \p DataType.
\return An iterator the the new element.*/
template<typename DataType, cg::SizeType SizeP>
template<typename ...Ts>
inline typename Array<DataType, SizeP>::ReverseIterator
Array<DataType, SizeP>::Emplace(ReverseIterator& it, Ts && ...ts)
{
    SizeType position = m_size - (RBegin() - it);
    Storage<DataType, SizeP>::Emplace(position, Forward<Ts>(ts)...);
    //move it back to its element.
    it = cg::ArrayIterator<DataType, false, true>(Addr() + (position - 1));
    auto ret = cg::ArrayIterator<DataType, false, true>(Addr() + position);
    return ret;
}
/**Emplace an element at the position \p it.  \p it will point to the same
element as it did before.
\param it The iterator to emplace at.
\param ts A parameter pack of arguments to send to the constructor
of \p DataType.
\return An iterator the the new element.*/
template<typename DataType, cg::SizeType SizeP>
template<typename ...Ts>
inline typename Array<DataType, SizeP>::Iterator
Array<DataType, SizeP>::Emplace(Iterator& it, Ts && ...ts)
{
    SizeType position = (it - Begin());
    Storage<DataType, SizeP>::Emplace(position, Forward<Ts>(ts)...);
    //move it back to its element.
    it = cg::ArrayIterator<DataType, false, false>(Addr() + (position + 1));
    auto ret = cg::ArrayIterator<DataType, false, false>(Addr() + position);
    return ret;
}
/**Push an object to the back of the list.
\param o The object.*/
template<typename DataType, cg::SizeType SizeP>
void Array<DataType, SizeP>::PushBack(DataType&& o)
{
    Insert(m_size, Forward<DataType>(o));
}
/**Push an object to the front of the list.
\param o The object.*/
template<typename DataType, cg::SizeType SizeP>
void Array<DataType, SizeP>::PushFront(DataType&& o)
{
    Insert(0, Forward<DataType>(o));
}
/**Push an object at the provided index.
\param i the index to be the index of the pushed item.
\param d The item to push.*/
template<typename DataType, cg::SizeType SizeP>
inline void Array<DataType, SizeP>::Push(cg::SizeType i, DataType&& d)
{
    Insert(i, Forward<DataType>(d));
}
/**Push an element to the position of this iterator. The iterator will point
to the same element it started with when finished.
\param it The iterator that is the position to push to.
\param d The data to push.
\return An iterator to the new element.*/
template<typename DataType, cg::SizeType SizeP>
inline typename Array<DataType, SizeP>::ReverseIterator
cg::Array<DataType, SizeP>::Push(ReverseIterator& it, DataType && d)
{
    SizeType position = m_size - (RBegin() - it);
    Push(position, Forward<DataType>(d));
    //move it back to its element.
    it = cg::ArrayIterator<DataType, false, true>(Addr() + (position - 1));
    auto ret = cg::ArrayIterator<DataType, false, true>(Addr() + position);
    return ret;
}
/**Push an element to the position of this iterator. The iterator will point
to the same element it started with when finished.
\param it The iterator that is the position to push to.
\param d The data to push.
\return An iterator to the new element.*/
template<typename DataType, cg::SizeType SizeP>
inline typename Array<DataType, SizeP>::Iterator
cg::Array<DataType, SizeP>::Push(Iterator& it, DataType && d)
{
    SizeType position = (it - Begin());
    Push(position, Forward<DataType>(d));
    //move it back to its element.
    it = cg::ArrayIterator<DataType, false, false>(Addr() + (position + 1));
    auto ret = cg::ArrayIterator<DataType, false, false>(Addr() + position);
    return ret;
}
/**Push an object after the provided iterator. The iterator will point
to the same element it started with when finished.
\param it The iterator to insert after.
\param d The item to push.
\return an iterator pointing to the newly inserted element.*/
template<typename DataType, cg::SizeType SizeP>
inline typename Array<DataType, SizeP>::Iterator
Array<DataType, SizeP>::PushAfter(Iterator& it, DataType&& d)
{
    SizeType position = (it - Begin()) + 1;
    Push(position, Forward<DataType>(d));
    //move it back to its element.
    it = cg::ArrayIterator<DataType, false, false>(Addr() + (position - 1));
    auto ret = cg::ArrayIterator<DataType, false, false>(Addr() + position);
    return ret;
}
/**Push an object after the provided iterator. The iterator will point
to the same element it started with when finished.
\param it The iterator to insert after.
\param d The item to push.
\return an iterator pointing to the newly inserted element.*/
template<typename DataType, cg::SizeType SizeP>
inline typename Array<DataType, SizeP>::ReverseIterator
Array<DataType, SizeP>::PushAfter(ReverseIterator& it, DataType&& d)
{
    SizeType position = m_size - (RBegin() - it) - 1;
    Push(position, Forward<DataType>(d));
    //move it back to its element.
    it = cg::ArrayIterator<DataType, false, true>(Addr() + (position + 1));
    auto ret = cg::ArrayIterator<DataType, false, true>(Addr() + position);
    return ret;
}
/**Get an element.
\param i The index to get.
\return The object at i.*/
template<typename DataType, cg::SizeType SizeP>
DataType& Array<DataType, SizeP>::Get(cg::SizeType i)
{
    if (i >= m_size)
        throw ArrayException::IndexOutOfBounds;
    return Addr()[i];
}
/**Get an element without any dereference layer.
\param i The index to get.
\return The object at i.*/
template<typename DataType, cg::SizeType SizeP>
const DataType& Array<DataType, SizeP>::Get(cg::SizeType i) const
{
    if (i >= m_size)
        throw ArrayException::IndexOutOfBounds;
    return Addr()[i];
}
/**Get an element.
\param i The index to get.
\return The object at i.*/
template<typename DataType, cg::SizeType SizeP>
DataType& Array<DataType, SizeP>::operator[](cg::SizeType i)
{
    if (i > m_size)
        throw ArrayException::IndexOutOfBounds;
    return Get(i);
}
/**Get an element.
\param i The index to get.
\return The object at i.*/
template<typename DataType, cg::SizeType SizeP>
const DataType& Array<DataType, SizeP>::operator[](cg::SizeType i) const
{
    if (i >= m_size)
        throw ArrayException::IndexOutOfBounds;
    return Get(i);
}
/**Get the begin iterator.
\return An iterator to the front of the list.*/
template<typename DataType, cg::SizeType SizeP>
typename Array<DataType, SizeP>::Iterator Array<DataType, SizeP>::Begin()
{
    return Iterator(Addr());
}
/**Get the begin iterator.
\return An iterator to the front of the list.*/
template<typename DataType, cg::SizeType SizeP>
typename Array<DataType, SizeP>::ConstIterator Array<DataType, SizeP>
::Begin()const
{
    return ConstIterator(Addr());
}
/**Get the begin iterator.
\return An iterator to the front of the list.*/
template<typename DataType, cg::SizeType SizeP>
typename Array<DataType, SizeP>::ReverseIterator Array<DataType, SizeP>
::RBegin()
{
    return ReverseIterator(Addr() + (m_size - 1));
}
/**Get the begin iterator.
\return An iterator to the front of the list.*/
template<typename DataType, cg::SizeType SizeP>
typename Array<DataType, SizeP>::ConstReverseIterator Array<DataType, SizeP>
::RBegin() const
{
    return ConstReverseIterator(Addr() + (m_size - 1));
}
/**Get an iterator to the end+1 of the list.
\return An iterator to End+1*/
template<typename DataType, cg::SizeType SizeP>
typename Array<DataType, SizeP>::Iterator Array<DataType, SizeP>::End()
{
    return Iterator(Addr() + m_size);
}
/**Get an iterator to the end+1 of the list.
\return An iterator to End+1*/
template<typename DataType, cg::SizeType SizeP>
typename Array<DataType, SizeP>::ConstIterator Array<DataType, SizeP>
::End() const
{
    return ConstIterator(Addr() + m_size);
}
/**Get an iterator to the end+1 of the list.
\return An iterator to End+1*/
template<typename DataType, cg::SizeType SizeP>
typename Array<DataType, SizeP>::ReverseIterator Array<DataType, SizeP>
::REnd()
{
    return ReverseIterator(Addr() - 1);
}
/**Get an iterator to the end+1 of the list.
\return An iterator to End+1*/
template<typename DataType, cg::SizeType SizeP>
typename Array<DataType, SizeP>::ConstReverseIterator Array<DataType, SizeP>
::REnd() const
{
    return ConstReverseIterator(Addr() - 1);
}
/**Erase a unit from the storage.
\param i The index to erase.*/
template<typename DataType, cg::SizeType SizeP>
void Array<DataType, SizeP>::Erase(cg::SizeType i)
{
    Array<DataType, SizeP>::Erase(i, 1);
}
/**Erase a unit from the storage.
\param i The index to erase.*/
template<typename DataType, cg::SizeType SizeP>
void Array<DataType, SizeP>::Pop(cg::SizeType i)
{
    Erase(i);
}
/**Erase a unit from the storage.
\param i The index to erase.
\param s The amount to erase in elements.*/
template<typename DataType, cg::SizeType SizeP>
void Array<DataType, SizeP>::Erase(cg::SizeType i, cg::SizeType s)
{
    if (!Addr())
        throw ArrayException::ListEmpty;
    std::memmove(Addr() + i, Addr() + i + s,
        (m_size - (i + s)) * sizeof(DataType));
    m_size -= s;
}
/**Erase a unit from the storage.
\param i The index to erase.
\param s The amount to erase in elements.*/
template<typename DataType, cg::SizeType SizeP>
void Array<DataType, SizeP>::Pop(cg::SizeType i, cg::SizeType s)
{
    Erase(i, s);
}
/**Pop off the last element.*/
template<typename DataType, cg::SizeType SizeP>
void Array<DataType, SizeP>::PopBack()
{
    Erase(m_size - 1);
}
/**Pop off the last element.
\param amt The amount to pop at the end.*/
template<typename DataType, cg::SizeType SizeP>
void Array<DataType, SizeP>::PopBack(cg::SizeType amt)
{
    Erase(m_size - amt, amt);
}
/**Pop the front element.*/
template<typename DataType, cg::SizeType SizeP>
void Array<DataType, SizeP>::PopFront()
{
    Erase(0);
}
/**Pop the front element.
\param amt The amount to pop at the front */
template<typename DataType, cg::SizeType SizeP>
void Array<DataType, SizeP>::PopFront(cg::SizeType amt)
{
    Erase(0, amt);
}
/**Determine the total cap of the list.
\return The max amout for this list, or 0 for no maximum.*/
template<typename DataType, cg::SizeType SizeP>
cg::SizeType Array<DataType, SizeP>::MaxSize() const
{
    return SizeP;
}

/**Get the item at the back of the list.
\return The item at the back of the list.*/
template<typename DataType, cg::SizeType SizeP>
inline const DataType & Array<DataType, SizeP>::Back() const
{
    return Addr()[Size() - 1];
}
/**Get the item at the back of the list.
\return The item at the back of the list.*/
template<typename DataType, cg::SizeType SizeP>
inline DataType & Array<DataType, SizeP>::Back()
{
    return Addr()[Size() - 1];
}
/**Get the item at the front of the list.
\return The item at the front of the list.*/
template<typename DataType, cg::SizeType SizeP>
inline const DataType & Array<DataType, SizeP>::Front() const
{
    return Addr()[0];
}
/**Get the item at the front of the list.
\return The item at the front of the list.*/
template<typename DataType, cg::SizeType SizeP>
inline DataType & Array<DataType, SizeP>::Front()
{
    return Addr()[0];
}
/**Ensure that the array can hold \p amt of units.
\param amt The amount to reserve.
\return True if the array can hold at least \p amt. False if the array cannot
hold \p amt for any reason (like it has a compiletime size restriction).*/
template<typename DataType, cg::SizeType SizeP>
inline bool Array<DataType, SizeP>::Reserve(SizeType amt)
{
    if (MaxSize() != 0 && MaxSize() < amt)
        return false;
    Storage<DataType, SizeP>::ExpandTo(amt);
    return true;
}

template class Array<int, 0>;
template class Array<int, 1>;

}