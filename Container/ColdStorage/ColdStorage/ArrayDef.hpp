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

#include "ArrayIterator.hpp"


namespace cg {

/**Array exceptions*/
enum class ArrayException {
    /**Tried to access data outside the bounds of the array.*/
    IndexOutOfBounds,
    /**The array is full*/
    ArrayIsFull,
    /**An invalid parameter, probably to big array.*/
    InvalidParameter,
    /**The list is empty.*/
    ListEmpty,
};


///////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////STACK HERE/////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

/**The data holding portion of the Array.
\tparam DataType The type of data to use.
\tparam Size, The size of the list on the stack, or ZERO to be heap-expanding.
*/
template<typename DataType, cg::SizeType SizeP>
class Storage
{
public:
    /**The type of this object.*/
    using SelfType = typename Storage<DataType, SizeP>;
    /**The max size.*/
    const static cg::SizeType MaxSize = SizeP;
    /**default ctor*/
    Storage(cg::SizeType = 8);

    Storage(SelfType&& other);

    void operator=(SelfType&& other);

    Storage(const DataType* arr, cg::SizeType aSize);

    Storage(std::initializer_list<DataType>&& vals);

    bool CanInsert() const;

    template<typename NType>
    void Insert(cg::SizeType i, NType&& o);

    template<typename...Ts>
    void Emplace(cg::SizeType i, Ts&&... nums);
protected:
    DataType* Addr(cg::SizeType i = 0);

    const DataType* Addr(cg::SizeType i = 0)const;
    /**Stop copying*/
    Storage(const SelfType&) = delete;
    /**Stop copying*/
    void operator=(const SelfType&) = delete;
    /**The maximum capacity of the storage*/
    cg::SizeType m_cap;
    /**The size of used up slots*/
    cg::SizeType m_size;
    /**Dummy expand, required for compatibility.*/
    void ExpandTo(SizeType amt) {};
private:
    /**The storage area.  Units of char so that the values are not initialized.
    */
    char m_data[MaxSize * sizeof(DataType)];
};

///////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////HEAP HERE//////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

/**The data holding portion of the Array.
\tparam DataType The type of data to use.
\tparam Size, The size of the list on the stack, or ZERO to be heap-expanding.
*/
template<typename DataType>
class Storage<DataType, 0>
{
public:
    /**The type of this object.*/
    using SelfType = typename Storage<DataType, 0>;
    /**The amount to expand when re allocating.*/
    const static cg::SizeType ExpandAmount = 8;

    Storage(cg::SizeType cap = 0);

    Storage(const DataType* arr, cg::SizeType aSize);

    Storage(SelfType&& other);

    Storage(std::initializer_list<DataType>&& vals);

    void operator=(SelfType&& other);

    virtual ~Storage();

    bool CanInsert() const;

    template<typename NType>
    void Insert(cg::SizeType i, NType&& o);

    template<typename...Ts>
    void Emplace(cg::SizeType i, Ts&&... nums);
private:
    /**The storage area.*/
    DataType* m_data;
protected:
    /**Stop copying*/
    Storage(const SelfType&) = delete;
    /**Stop copying*/
    void operator=(const SelfType&) = delete;
    /**The maximum capacity of the storage*/
    cg::SizeType m_cap;
    /**The size of used up slots*/
    cg::SizeType m_size;

    void ExpandTo(cg::SizeType amt);

    DataType* Addr(cg::SizeType i = 0);

    const DataType* Addr(cg::SizeType i = 0)const;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////LIST HERE//////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

/**An array based list.
\tparam DataType The type of data.
\tparam SizeP THe size of the array. `0` to be auto-expanding.*/
template<typename DataType, cg::SizeType SizeP>
class Array : public Storage<DataType, SizeP>
{
public:
    /**The type of this object.*/
    using SelfType = typename Array<DataType, SizeP>;
    /**A reverse moving iterator type*/
    using ReverseIterator = ArrayIterator<DataType, false, true>;
    /**The standard forward iterator*/
    using Iterator = ArrayIterator<DataType, false, false>;
    /**A const reverse moving iterator*/
    using ConstReverseIterator = ArrayIterator<DataType, true, true>;
    /**A forward moving const iterator*/
    using ConstIterator = ArrayIterator<DataType, true, false>;
    /**The amount to expand when re allocating.*/
    const static cg::SizeType ExpandAmount = 8;

    Array(cg::SizeType initCap);

    Array();

    Array(const DataType* arr, cg::SizeType aSize);

    void operator=(SelfType&& other);

    Array(SelfType&& other);

    Array(std::initializer_list<DataType>&& vals);

    template<typename DataType2, cg::SizeType SizeP2>
    Array(const Array<DataType2, SizeP2>& other);

    Array(const SelfType& other);

    template<typename DataType2, cg::SizeType SizeP2>
    Array<DataType, SizeP>& operator=(const Array<DataType2, SizeP2>& other);

    Array<DataType, SizeP>& operator=(const SelfType& other);

    Array(DataType* beg, DataType* end);

    void FillUnused(const DataType& x);

    cg::SizeType Size() const;


    template<typename...Ts>
    void EmplaceBack(Ts&&... o);

    template<typename...Ts>
    void EmplaceFront(Ts&&... o);

    void PushBack(DataType&& o);

    void PushFront(DataType&& o);

    DataType& Get(cg::SizeType i);

    const DataType& Get(cg::SizeType i) const;

    DataType& operator[](cg::SizeType i);

    const DataType& operator[](cg::SizeType i) const;


    Iterator Begin();

    ConstIterator Begin() const;

    ReverseIterator RBegin();

    ConstReverseIterator RBegin() const;

    Iterator End();

    ConstIterator End() const;

    ReverseIterator REnd();

    ConstReverseIterator REnd() const;

    void Erase(cg::SizeType i);

    void Pop(cg::SizeType i);

    void Erase(cg::SizeType i, cg::SizeType s);

    void Pop(cg::SizeType i, cg::SizeType s);

    void PopBack();

    void PopBack(cg::SizeType amt);

    void PopFront();

    void PopFront(cg::SizeType amt);

    ReverseIterator PushAfter(ReverseIterator& it, DataType&& d);

    Iterator  PushAfter(Iterator& it, DataType&& d);

    ReverseIterator  Push(ReverseIterator& it, DataType&& d);

    Iterator  Push(Iterator& it, DataType&& d);

    void Push(cg::SizeType i, DataType&& d);

    template<typename...Ts>
    ReverseIterator EmplaceAfter(ReverseIterator& it, Ts&&...ts);

    template<typename...Ts>
    Iterator  EmplaceAfter(Iterator& it, Ts&&...ts);

    template<typename...Ts>
    ReverseIterator Emplace(ReverseIterator& it, Ts&&...ts);

    template<typename...Ts>
    Iterator Emplace(Iterator& it, Ts&&...ts);

    cg::SizeType MaxSize() const;

    const DataType& Back() const;

    DataType& Back();

    const DataType& Front() const;

    DataType& Front();

    bool Reserve(SizeType amt);
};

template class Array<int, 0>;
template class Array<int, 12>;

}