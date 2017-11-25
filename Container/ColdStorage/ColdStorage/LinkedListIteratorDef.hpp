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

#include "cgdef.hpp"
#include "LinkedListNode.hpp"


namespace cg {

/**Exceptions for iterators.*/
enum class LinkedListIteratorException
{
    /**The iterator was not dereferencable.*/
    NotDereferenceable,
    /**Some requested index is out of bounds.*/
    IndexOutOfBounds,
};

/**A linked list iterator.
\tparam DataType The type of data.
\tparam Const True for a const iterator.
\tparam Reverse True to move in reverese direction.*/
template<typename DataType, bool Const, bool Reverse>
class LinkedListIterator {
public:
    /**The type of data reference to return.*/
    using Ref = ConditionalType<Const, const DataType&, DataType&>;
    /**The type of data reference to return.*/
    using Ptr = ConditionalType<Const, const DataType*, DataType*>;
    /**The type of this object.*/
    using SelfType = LinkedListIterator<DataType, Const, Reverse>;

    /**Cosntruct an uninitialized iterator.
    \post The iterator is NOT in a usable state.*/
    LinkedListIterator() :m_ptr(nullptr) {}

    LinkedListIterator(LinkedListNode<DataType>* p);

    LinkedListIterator(const SelfType& other);

    SelfType& operator=(const SelfType& other);

    LinkedListIterator(SelfType&& other);

    SelfType& operator=(SelfType&& other);

    operator bool() const;

    Ref operator*();

    const Ref operator*() const;

    Ptr operator->();

    const Ptr operator->() const;

    SelfType operator++(int);

    SelfType& operator++();

    SelfType operator--(int);

    SelfType& operator--();

    bool operator!=(const SelfType& other) const;

    bool operator==(const SelfType& other) const;

    bool operator<(const SelfType& other);

    bool operator<=(const SelfType& other);

    bool operator>(const SelfType& other);

    bool operator>=(const SelfType& other);

    SelfType operator+(const SizeType& amt) const;

    SelfType& operator+=(const SizeType& amt);

    SelfType operator-(const SizeType& amt) const;

    SelfType& operator-=(const SizeType& amt);

    Ref operator[](const DiffType& i);

    const Ref& operator[](const DiffType& i) const;

    Ptr Addr();

    const Ptr Addr() const;
private:
    template<typename>
    friend class LinkedList;
    /**Check and throw and exceptions needed*/
    void CheckAndThrow() const;
    /**The iterators current location*/
    LinkedListNode<DataType>* m_ptr;

};


/**Allow N + I where N is a number and I is an iterator.
\pre \p i must be initialized and point to some valid memory.
\param n Some positive number.
\param i An initialized iterator that points to valid memory.
\return A copy of \p i after adjustment.*/
template<typename DataType, bool Const, bool Reverse>
auto operator+(SizeType n, 
    const LinkedListIterator<DataType, Const, Reverse>& i)
{
    auto copy = i;
    copy += n;
    return copy;
}

}