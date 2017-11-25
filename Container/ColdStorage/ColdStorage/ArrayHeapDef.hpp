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

#include "Array.hpp"

namespace cg {

/**Exceptions for the heap*/
enum class ArrayheapException {
    /**The array is empty.*/
    EmptyArray,
    /**The array is full.*/
    FullArray,
};

/**Static way to calculate the amount of nodes in a tree with N levels.*/
template<SizeType N>
struct TreeSize
{
    const static SizeType value = cg::Power<2, N>::value - 1;
    inline SizeType operator()(SizeType num) {
        if (num == 1)
            return 1;
        else
            return 
    }
};

/**A tree that is stored in an array.
\tparam KeyType The type of key for sorting.
\tparam Predicate The object with whom the () operator should return true if
key 'a' should be higher on the tree than key 'b'.
\tparam Size The max amount of levels in the tree. Use '0' for auto resize. A
tree with 1 level has 1 node, a tree with 2 levels has 3 nodes. A tree with N
levels has [(2^N) - 1] nodes.*/
template<typename DataType, typename KeyType, 
    SizeType Size, typename Predicate = Less<KeyType>>
class ArrayHeap
{
public:
    /**The type of pair*/
    using PairType = Pair<DataType, KeyType>;
    /**The type of list*/
    using ListType = Array<PairType, TreeSize<Size>::value>;
    /**A const reverse moving iterator*/
    using ConstReverseIterator = ArrayIterator<PairType, true, true>;
    /**A forward moving const iterator*/
    using ConstIterator = ArrayIterator<PairType, true, false>;
    /**The type of self.*/
    using SelfType = ArrayHeap<DataType, KeyType, Size, Predicate>;

    ArrayHeap(SizeType init = 3);

    void Push(KeyType&& key, DataType&& o);

    void Push(PairType&& p);

    template<typename... Ts>
    void Emplace(KeyType&& key, Ts&&...ts);

    void Pop();

    DataType& Top();

    const DataType& Top() const;


    const KeyType& TopKey() const;

    template<typename OutStream>
    void ShowKeys(OutStream& out) const;

    bool Empty() const;

    bool Full() const;

    ConstIterator Begin() const;

    ConstReverseIterator RBegin() const;

    ConstIterator End() const;

    ConstReverseIterator REnd() const;

private:
    /**The type of pair*/
    using PairType = Pair<DataType, KeyType>;
    /**The type of list*/
    using ListType = Array<PairType, TreeSize<Size>::value>;
    /**The array to store the data.*/
    ListType m_data;

    void FixUp(SizeType index);

    void FixDown(SizeType index);

    const KeyType& KeyAt(SizeType index)const;

    SizeType LeftChild(SizeType index);

    SizeType RightChild(SizeType index);

    SizeType Parent(SizeType index);

    bool IsLeftChild(SizeType index, SizeType parent);

    bool IsRightChild(SizeType index, SizeType parent);

    bool IsParent(SizeType index, SizeType child);

    static const Predicate pred; 
};

template class ArrayHeap<int, int,0, Less<int>>;
template class ArrayHeap<int, int,3, Less<int>>;


}