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

#include "ArrayHeapDef.hpp"

namespace cg {
template<typename DataType, typename KeyType, SizeType Size,
    typename Predicate>
    const Predicate ArrayHeap<DataType, KeyType, Size, Predicate>::pred;

/**Create with an initial amount of levels. If the template parameter \p Size
is non-zero, \p init will be meaningless, as the amount of levels are set at
compile time.
\param init The amount of levels to be immediatly reserved.*/
template<typename DataType, typename KeyType, SizeType Size,
    typename Predicate>
    inline ArrayHeap<DataType, KeyType, Size, Predicate>::ArrayHeap(
        SizeType init) :m_data(RTPower(2, init) - 1)
{

}
/**Emplace an object on the tree. Will automatically rearrange the tree to be
balanced and complete.
\param ts The arguments to forward to the constructor.*/
template<typename DataType, typename KeyType, SizeType Size,
    typename Predicate>
    template<typename ...Ts>
inline void ArrayHeap<DataType, KeyType, Size, Predicate>::Emplace(
    KeyType&& key, Ts && ...ts)
{
    if (Full())
        throw ArrayheapException::FullArray;
    m_data.PushBack(PairType());
    new (&m_data.Back().m_a) DataType(Forward<Ts>(ts)...);
    m_data.Back().m_b = Forward<KeyType>(key);
    FixUp(m_data.Size() - 1);
}
/**Push an object to the tree. Will automatically rearrange the tree to be
balanced and complete.
\param o The object to push.*/
template<typename DataType, typename KeyType, SizeType Size,
    typename Predicate>
    inline void
    ArrayHeap<DataType, KeyType, Size, Predicate>::Push(
        KeyType&& key, DataType && o)
{
    if (Full())
        throw ArrayheapException::FullArray;
    m_data.PushBack(MakePair(Forward<DataType>(o), Forward<KeyType>(key)));
    FixUp(m_data.Size() - 1);
}
/**insert a pair into the tree.
\param p The pair to insert.*/
template<typename DataType, typename KeyType, SizeType Size,
    typename Predicate>
    inline void ArrayHeap<DataType, KeyType, Size, Predicate>
    ::Push(PairType && p)
{
    if (Full())
        throw ArrayheapException::FullArray;
    m_data.PushBack(Forward<PairType>(p));
    FixUp(m_data.Size() - 1);
}
/**Pop the top item off the heap.*/
template<typename DataType, typename KeyType, SizeType Size, typename Predicate>
inline void ArrayHeap<DataType, KeyType, Size, Predicate>::Pop()
{
    if (Empty())
        throw ArrayheapException::EmptyArray;
    Swap(m_data.Front(), m_data.Back());
    m_data.PopBack();
    if (!Empty())
        FixDown(0);
}
/**Get the data of the top element.
\return A reference to the data on top of the heap.*/
template<typename DataType, typename KeyType, SizeType Size,
    typename Predicate>
    inline DataType & ArrayHeap<DataType, KeyType, Size, Predicate>::Top()
{
    return m_data[0].m_a;
}
/**Get the data of the top element.
\return A reference to the data on top of the heap.*/
template<typename DataType, typename KeyType, SizeType Size,
    typename Predicate>
    inline const DataType & ArrayHeap<DataType, KeyType, Size, Predicate>
    ::Top() const
{
    return m_data[0].m_a;
}
/**Get the key of the top element.
\return A const reference to the key on top of the heap.*/
template<typename DataType, typename KeyType, SizeType Size,
    typename Predicate>
    inline const KeyType & ArrayHeap<DataType, KeyType, Size, Predicate>
    ::TopKey() const
{
    return m_data[0].m_b;
}
/**Determine if the heap is empty or not.
\return True if the heap is empty.*/
template<typename DataType, typename KeyType, SizeType Size,
    typename Predicate>
    inline bool ArrayHeap<DataType, KeyType, Size, Predicate>::Empty() const
{
    return m_data.Size() == 0;
}
/**Determine if the heap is full or not.
\return True if the heap is full.*/
template<typename DataType, typename KeyType, SizeType Size,
    typename Predicate>
    inline bool ArrayHeap<DataType, KeyType, Size, Predicate>::Full() const
{
    return m_data.Size() == Size && Size != 0;
}
/**Show the keys of the items in the heap.
\param out A stream to take output.*/
template<typename DataType, typename KeyType, SizeType Size,
    typename Predicate>
    template<typename OutStream>
inline void ArrayHeap<DataType, KeyType, Size, Predicate>::ShowKeys(
    OutStream & out) const
{
    SizeType printed = 0;
    SizeType pow = 0;
    while (printed < m_data.Size())
    {
        int amt = 1 << pow;
        for (SizeType i = 0; i < amt && printed < m_data.Size(); ++i)
        {
            out << KeyAt(printed++) << " ";
        }
        out << "\n";
        ++pow;
    }
}
/**Fix the tree into its proper binary search relationship after inserting an
item.
\param index The index for which to start.*/
template<typename DataType, typename KeyType, SizeType Size,
    typename Predicate>
    inline void ArrayHeap<DataType, KeyType, Size, Predicate>::FixUp(
        SizeType index)
{
    if (index == 0)
        return;
    KeyType n = KeyAt(index);
    SizeType pIndex = Parent(index);
    KeyType p = KeyAt(pIndex);
    //swap if the node should occure before its parent.
    bool swapIt = pred(n, p);
    if (swapIt)
    {
        Swap(m_data[index], m_data[pIndex]);
        FixUp(pIndex);
    }
}
/**Fix the tree into its proper binary search relationship after popping an
item.
\param index The index for which to start.*/
template<typename DataType, typename KeyType, SizeType Size,
    typename Predicate>
    inline void ArrayHeap<DataType, KeyType, Size, Predicate>::FixDown(
        SizeType index)
{
    SizeType l = LeftChild(index);
    SizeType r = RightChild(index);
    KeyType lk = KeyAt(l);
    KeyType rk = KeyAt(r);
    SizeType swapIndex = 0;
    if (l && r)
        swapIndex = pred(lk, rk) ? l : r;
    else if (r)
        swapIndex = r;
    else if (l)
        swapIndex = l;
    else
        return;
    KeyType k = KeyAt(index);
    KeyType sk = KeyAt(swapIndex);
    bool doIt = pred(sk, k);
    if (doIt)
    {
        Swap(m_data[index], m_data[swapIndex]);
        FixDown(swapIndex);
    }
}
/**Quick reference to the key at an index.
\param index The index for which to get the key.*/
template<typename DataType, typename KeyType, SizeType Size,
    typename Predicate> inline const KeyType &
    ArrayHeap<DataType, KeyType, Size, Predicate>::KeyAt(SizeType index)const
{
    return m_data[index].m_b;
}
/**Determine the index of the left child of the object at \p index
@param index The index for which to get the child.
@return The index of the child of the object at \p index, or ZERO if there is
no child.*/
template<typename DataType, typename KeyType, SizeType Size,
    typename Predicate>
    inline SizeType
    ArrayHeap<DataType, KeyType, Size, Predicate>::LeftChild(SizeType index)
{
    SizeType newIndex = (2 * index) + 1;
    return newIndex < m_data.Size() ? newIndex : 0;
}
/**Determine the index of the right child of the object at \p index
@param index The index for which to get the child.
@return The index of the child of the object at \p index, or ZERO if there is
no child.*/
template<typename DataType, typename KeyType, SizeType Size,
    typename Predicate>
    inline SizeType
    ArrayHeap<DataType, KeyType, Size, Predicate>::RightChild(SizeType index)
{
    SizeType newIndex = (2 * index) + 2;
    return newIndex < m_data.Size() ? newIndex : 0;
}
/**Determine the index of the parent of the object at \p index
\param index The index for which to get the parent.
\return The index of the parent of the object at \p index, or ZERO if there is
no parent (happens when index is root index (0) ).*/
template<typename DataType, typename KeyType, SizeType Size,
    typename Predicate>
    inline SizeType
    ArrayHeap<DataType, KeyType, Size, Predicate>::Parent(SizeType index)
{
    return (index - 1) / 2;
}
/**Determine if an index is the left child of another index.
\param index The index of the child in question.
\param parant The parent index to test.
\return True if \p index is the left child of \p paretn.*/
template<typename DataType, typename KeyType, SizeType Size,
    typename Predicate>
    inline bool ArrayHeap<DataType, KeyType, Size, Predicate>
    ::IsLeftChild(SizeType index, SizeType parent)
{
    return SizeType();
}
/**Determine if an index is the right child of another index.
\param index The index of the child in question.
\param parant The parent index to test.
\return True if \p index is the right child of \p paretn.*/
template<typename DataType, typename KeyType, SizeType Size,
    typename Predicate>
    inline bool ArrayHeap<DataType, KeyType, Size, Predicate>
    ::IsRightChild(SizeType index, SizeType parent)
{
    return SizeType();
}
/**Determine if an index is the parent of another index.
\param index The parent in question.
\param child The child index is question.*/
template<typename DataType, typename KeyType, SizeType Size,
    typename Predicate>
    inline bool ArrayHeap<DataType, KeyType, Size, Predicate>::IsParent(
        SizeType index, SizeType child)
{
    return IsLeftChild(child, index) || IsRightChild(child, index);
}

/**Get iterator from the data array.
\return An iterator for the first element.*/
template<typename DataType, typename KeyType, SizeType Size,
    typename Predicate>
    inline typename ArrayHeap<DataType, KeyType, Size, Predicate>
    ::ConstIterator
    ArrayHeap<DataType, KeyType, Size, Predicate>::Begin() const
{
    return ConstIterator((&m_data[0]));
}

/**Get iterator from the data array.
\return An iterator for the first element.*/
template<typename DataType, typename KeyType, SizeType Size,
    typename Predicate>
    inline typename ArrayHeap<DataType, KeyType, Size, Predicate>
    ::ConstReverseIterator
    ArrayHeap<DataType, KeyType, Size, Predicate>::RBegin() const
{
    return ConstReverseIterator((&m_data[0]) + m_data.Size() - 1);
}

/**Get iterator from the data array.
\return A one past the end iterator for the array of the heap.*/
template<typename DataType, typename KeyType, SizeType Size,
    typename Predicate>
    inline typename ArrayHeap<DataType, KeyType, Size, Predicate>
    ::ConstIterator
    ArrayHeap<DataType, KeyType, Size, Predicate>::End() const
{
    return ConstIterator((&m_data[0]) + m_data.Size());
}
/**Get iterator from the data array.
\return A one past the end iterator for the array of the heap.*/
template<typename DataType, typename KeyType, SizeType Size,
    typename Predicate>
    inline typename ArrayHeap<DataType, KeyType, Size, Predicate>
    ::ConstReverseIterator
    ArrayHeap<DataType, KeyType, Size, Predicate>::REnd() const
{
    return ConstReverseIterator((&m_data[0]) - 1);
}

}