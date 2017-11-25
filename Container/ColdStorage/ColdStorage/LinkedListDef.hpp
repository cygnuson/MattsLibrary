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

#include "LinkedListIterator.hpp"


namespace cg {

/**Exceptions for.*/
enum class LinkedListException
{
    /**Some requested index is out of bounds.*/
    IndexOutOfBounds,
    /**The list is empty.*/
    ListEmpty,
    /**Invalid iterator error*/
    InvalidIterator,
};

/**Double linked list
\tparam DataType The type of data to store.*/
template<typename DataType>
class LinkedList
{
public:
    /**The type of this object.*/
    using SelfType = typename LinkedList<DataType>;
    /**A reverse moving iterator type*/
    using ReverseIterator = LinkedListIterator<DataType, false, true>;
    /**The standard forward iterator*/
    using Iterator = LinkedListIterator<DataType, false, false>;
    /**A const reverse moving iterator*/
    using ConstReverseIterator = LinkedListIterator<DataType, true, true>;
    /**A forward moving const iterator*/
    using ConstIterator = LinkedListIterator<DataType, true, false>;

    /**Default ctor*/
    LinkedList() {};

    LinkedList(const LinkedList<DataType>& o);

    LinkedList(LinkedList<DataType>&& o);

    LinkedList(Iterator begin, Iterator end);

    LinkedList(ReverseIterator begin, ReverseIterator end);

    LinkedList<DataType>& operator=(const LinkedList<DataType>& o);

    LinkedList<DataType>& operator=(LinkedList<DataType>&& o);

    LinkedList(const DataType* ptr, SizeType size);

    cg::SizeType Empty() const;

    template<typename...Ts>
    void EmplaceBack(Ts&&... o);

    template<typename...Ts>
    void EmplaceFront(Ts&&... o);

    void PushBack(DataType&& o);

    void PushFront(DataType&& o);

    Iterator Begin();

    ConstIterator Begin() const;

    ReverseIterator RBegin();

    ConstReverseIterator RBegin() const;

    Iterator End();

    ConstIterator End() const;

    ReverseIterator REnd();

    ConstReverseIterator REnd() const;

    void PopBack();

    void PopFront();


    ReverseIterator PushAfter(ReverseIterator& it, DataType&& d);

    Iterator PushAfter(Iterator& it, DataType&& d);

    ReverseIterator Push(ReverseIterator& it, DataType&& d);

    Iterator Push(Iterator& it, DataType&& d);

    template<typename...Ts>
    ReverseIterator EmplaceAfter(ReverseIterator& it, Ts&&...ts);

    template<typename...Ts>
    Iterator EmplaceAfter(Iterator& it, Ts&&...ts);

    template<typename...Ts>
    ReverseIterator Emplace(ReverseIterator& it, Ts&&...ts);

    template<typename...Ts>
    Iterator Emplace(Iterator& it, Ts&&...ts);

    const DataType& Back() const;

    DataType& Back();

    const DataType& Front() const;

    DataType& Front();
private:
    /**The first node.*/
    LinkedListNode<DataType>* m_first;
    /**The last node*/
    LinkedListNode<DataType>* m_last;


};

template class LinkedList<int>;


}