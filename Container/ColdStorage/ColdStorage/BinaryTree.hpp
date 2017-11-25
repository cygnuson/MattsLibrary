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

#include "BinaryTreeDef.hpp"

namespace cg {

template<typename DataType, typename KeyType, typename Predicate>
const Predicate BinaryTree<DataType, KeyType, Predicate>::pred;

/**Insert an element into the tree.
\param key The key of the thing to insert.
\param o The object to insert.*/
template<typename DataType, typename KeyType, typename Predicate>
inline void BinaryTree<DataType, KeyType, Predicate>::Push(KeyType && key,
    DataType && o)
{
    InsertHelper(m_root,
        MakePair(Forward<DataType>(o), Forward<KeyType>(key)));
    ++m_size;
}
/**Insert an element into the tree.
\param p The pair of key and object to insert..*/
template<typename DataType, typename KeyType, typename Predicate>
inline void BinaryTree<DataType, KeyType, Predicate>::Push(PairType && p)
{
    InsertHelper(m_root, Forward<PairType>(p));
    ++m_size;
}
/**Remove the object at \p key.
\param key The key to search for.
\throw BinaryTreeException::KeyDoesNotExist if the key does not exist.*/
template<typename DataType, typename KeyType, typename Predicate>
inline void BinaryTree<DataType, KeyType, Predicate>::Pop(KeyType && key)
{
    RemoveHelper(m_root, Forward<KeyType>(key));
    --m_size;
}
/**Get the object at \p key.
\param key The key to search for.
\return A reference to the object.
\throw BinaryTreeException::KeyDoesNotExist if the key does not exist.*/
template<typename DataType, typename KeyType, typename Predicate>
inline DataType & BinaryTree<DataType, KeyType, Predicate>::Get(KeyType && key)
{
    return GetHelper(m_root, Forward<KeyType>(key));
}
/**Get the object at \p key.
\param key The key to search for.
\return A reference to the object.
\throw BinaryTreeException::KeyDoesNotExist if the key does not exist.*/
template<typename DataType, typename KeyType, typename Predicate>
inline const DataType &
BinaryTree<DataType, KeyType, Predicate>::Get(KeyType && key) const
{
    return GetHelper(m_root, Forward<KeyType>(key));
}
/**Get the object at \p key.
\param key The key to search for.
\return A reference to the object.
\throw BinaryTreeException::KeyDoesNotExist if the key does not exist.*/
template<typename DataType, typename KeyType, typename Predicate>
inline DataType &
BinaryTree<DataType, KeyType, Predicate>::operator[](KeyType && key)
{
    return Get(Forward<KeyType>(key));
}
/**Get the object at \p key.
\param key The key to search for.
\return A reference to the object.
\throw BinaryTreeException::KeyDoesNotExist if the key does not exist.*/
template<typename DataType, typename KeyType, typename Predicate>
inline const DataType &
BinaryTree<DataType, KeyType, Predicate>::operator[](KeyType && key) const
{
    return Get(Forward<KeyType>(key));
}
template<typename DataType, typename KeyType, typename Predicate>
inline SizeType BinaryTree<DataType, KeyType, Predicate>::Count(
    KeyType && key) const
{
    return CountHelper(m_root, Forward<KeyType>(key));
}
/**Determine if the tree is empty.
\return True if the tree is empty.*/
template<typename DataType, typename KeyType, typename Predicate>
inline bool BinaryTree<DataType, KeyType, Predicate>::Empty() const
{
    return m_root == nullptr;
}
/**Get the begin iterator.
\return The begin iterator to the first inorder element.*/
template<typename DataType, typename KeyType, typename Predicate>
inline typename  BinaryTree<DataType, KeyType, Predicate>::ConstIterator
BinaryTree<DataType, KeyType, Predicate>::Begin() const
{
    return ConstIterator(this);
}
/**Get the begin iterator.
\return The begin iterator to the first inorder element.*/
template<typename DataType, typename KeyType, typename Predicate>
inline typename  BinaryTree<DataType, KeyType, Predicate>::Iterator
BinaryTree<DataType, KeyType, Predicate>::Begin()
{
    return Iterator(this);
}
/**Get the reverse begin iterator.
\return The reverse begin iterator to the first inorder element.*/
template<typename DataType, typename KeyType, typename Predicate>
inline typename  BinaryTree<DataType, KeyType, Predicate>::ConstReverseIterator
BinaryTree<DataType, KeyType, Predicate>::RBegin() const
{
    return ConstReverseIterator(this);
}
/**Get the reverse begin iterator.
\return The reverse begin iterator to the first inorder element.*/
template<typename DataType, typename KeyType, typename Predicate>
inline typename  BinaryTree<DataType, KeyType, Predicate>::ReverseIterator
BinaryTree<DataType, KeyType, Predicate>::RBegin()
{
    return ReverseIterator(this);
}
/**Get a one past the end iterator.
\return A iterator to one element past the end.*/
template<typename DataType, typename KeyType, typename Predicate>
inline typename  BinaryTree<DataType, KeyType, Predicate>::ConstIterator
BinaryTree<DataType, KeyType, Predicate>::End() const
{
    return ConstIterator();
}
/**Get a one past the end iterator.
\return A iterator to one element past the end.*/
template<typename DataType, typename KeyType, typename Predicate>
inline typename  BinaryTree<DataType, KeyType, Predicate>::Iterator
BinaryTree<DataType, KeyType, Predicate>::End()
{
    return Iterator();
}
/**Get a one past the end reverse iterator.
\return A reverse iterator to one element past the end.*/
template<typename DataType, typename KeyType, typename Predicate>
inline typename  BinaryTree<DataType, KeyType, Predicate>::ReverseIterator
BinaryTree<DataType, KeyType, Predicate>::REnd()
{
    return ReverseIterator();
}
/**Get a one past the end reverse iterator.
\return A reverse iterator to one element past the end.*/
template<typename DataType, typename KeyType, typename Predicate>
inline typename BinaryTree<DataType, KeyType, Predicate>::ConstReverseIterator
BinaryTree<DataType, KeyType, Predicate>::REnd() const
{
    return ConstReverseIterator();
}
/**Emplace an object onto the tree.
\param key The key to put the new object.
\param ts The arguments to forward to the constructor of the data.*/
template<typename DataType, typename KeyType, typename Predicate>
template<typename ...Ts>
inline void BinaryTree<DataType, KeyType, Predicate>::Emplace(
    KeyType && key, Ts && ...ts)
{
    PairType p;
    p.m_b = Forward<KeyType>(key);
    new (&p.m_a) DataType(Forward<Ts>(ts)...);
    InsertHelper(m_root, Move(p));
    ++m_size;
}
/**Print the keys from left to right
\param out The stream to print to.*/
template<typename DataType, typename KeyType, typename Predicate>
template<typename OutStream>
inline void BinaryTree<DataType, KeyType, Predicate>::ShowKeys(
    OutStream & out) const
{
    PrintHelper(m_root, out);
}
/**Print the keys from left to right
\param out The stream to print to.*/
template<typename DataType, typename KeyType, typename Predicate>
template<typename Stream>
inline void BinaryTree<DataType, KeyType, Predicate>::PrintHelper(
    NodeType * node, Stream & out)
{
    if (!node)
        return;
    PrintHelper(node->m_left, out);
    out << node->m_data.m_b << ',';
    PrintHelper(node->m_right, out);
}
/**Recursive helper for inserting.
\param startNode The node to start looking with.
\param p The pair to insert.*/
template<typename DataType, typename KeyType, typename Predicate>
inline void BinaryTree<DataType, KeyType, Predicate>::InsertHelper(
    NodeType *& startNode, PairType && p)
{
    if (!startNode)
    {
        startNode = new BinaryTreeNode<DataType, KeyType, Predicate>(
            Forward<PairType>(p), nullptr, nullptr);
        return;
    }
    bool goLeft = pred(p.m_b, startNode->m_data.m_b);
    bool goRight = pred(startNode->m_data.m_b, p.m_b);
    if (goLeft)
        InsertHelper(startNode->m_left, Forward<PairType>(p));
    else if (goRight)
        InsertHelper(startNode->m_right, Forward<PairType>(p));
    else
        startNode->m_data = Forward<PairType>(p);
}
/**Will return the address of the pair with key \p key
\param startNode The node tostart looking at.
\param key The key to search for.
\return a pointer to the pair with key \p key.*/
template<typename DataType, typename KeyType, typename Predicate>
inline typename BinaryTree<DataType, KeyType, Predicate>::PairType *
BinaryTree<DataType, KeyType, Predicate>::AddressHelper(
    NodeType * startNode, KeyType && key)
{
    if (!startNode)
        return nullptr;
    bool goLeft = pred(key, startNode->m_data.m_b);
    bool goRight = pred(startNode->m_data.m_b, key);
    if (goLeft)
        return AddressHelper(startNode->m_left, Forward<KeyType>(key));
    else if (goRight)
        return AddressHelper(startNode->m_right, Forward<KeyType>(key));
    else
        return nullptr;
}
/**Helper to get data.
\param startNode The node to start with.
\param key The key to look for.
\return A referene to the data.*/
template<typename DataType, typename KeyType, typename Predicate>
inline DataType & BinaryTree<DataType, KeyType, Predicate>::GetHelper(
    NodeType * startNode, KeyType && key)
{
    if (!startNode)
        throw BinaryTreeException::KeyDoesNotExist;
    bool goLeft = pred(key, startNode->m_data.m_b);
    bool goRight = pred(startNode->m_data.m_b, key);
    if (goLeft)
        return GetHelper(startNode->m_left, Forward<KeyType>(key));
    else if (goRight)
        return GetHelper(startNode->m_right, Forward<KeyType>(key));
    else
        return startNode->m_data.m_a;
}
/**Helper to remove data.
\param node The node to start with.
\param key The key to look for.*/
template<typename DataType, typename KeyType, typename Predicate>
inline void BinaryTree<DataType, KeyType, Predicate>::RemoveHelper(
    NodeType *& node, KeyType && key)
{
    if (!node)
        return;
    bool goLeft = pred(key, node->m_data.m_b);
    bool goRight = pred(node->m_data.m_b, key);
    if (goLeft)
        return RemoveHelper(node->m_left, Forward<KeyType>(key));
    else if (goRight)
        return RemoveHelper(node->m_right, Forward<KeyType>(key));
    else //key must be equal here
    {
        if (node->m_left && node->m_right)
        {
            //both nodes
            NodeType* rightmost = node->m_left;
            while (rightmost->m_right)
                rightmost = rightmost->m_right;
            node->m_data = rightmost->m_data;
            RemoveHelper(node->m_left, Forward<KeyType>(node->m_data.m_b));
        }
        else if (!node->m_left && node->m_right)
        {
            NodeType* old = node;
            node = node->m_right;
            delete old;
        }
        else if (node->m_left && !node->m_right)
        {
            NodeType* old = node;
            node = node->m_left;
            delete old;
        }
        else
        {
            delete node;
            node = NULL;
        }
    }
}
/**Helper to count data.
\param node The node to start with.
\param key The key to look for.*/
template<typename DataType, typename KeyType, typename Predicate>
inline SizeType BinaryTree<DataType, KeyType, Predicate>::CountHelper(
    NodeType * startNode, KeyType&& key)
{
    SizeType ct = 0;
    if (!startNode)
        return 0;
    if (startNode->m_data.m_b == key)
        ct = 1;
    return ct + CountHelper(startNode->m_left, Forward<KeyType>(key))
        + CountHelper(startNode->m_right, Forward<KeyType>(key));
}

template class BinaryTree<int, int, Less<int>>;

}