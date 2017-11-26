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

#include "BinaryTreeIteratorDef.hpp"

namespace cg {

/**Create a node based on a tree \p tree.
\p tree The tree to iterate.*/
template<typename NodeType, bool Const, bool Reverse>
inline BinaryTreeIterator<NodeType, Const, Reverse>::BinaryTreeIterator(
    NodePtr root) :m_root(root), m_active(nullptr), m_stackCount(0),
    m_stackIndex(0)
{

}
template<typename NodeType, bool Const, bool Reverse>
inline BinaryTreeIterator<NodeType, Const, Reverse>::BinaryTreeIterator(
    const SelfType & other)
    : m_root(other.m_root), m_active(other.m_active),
    m_stackCount(other.m_stackCount), m_stackIndex(other.m_stackIndex)
{
}
template<typename NodeType, bool Const, bool Reverse>
inline typename BinaryTreeIterator<NodeType, Const, Reverse>::SelfType &
BinaryTreeIterator<NodeType, Const, Reverse>::operator=(
    const SelfType & other)
{
    m_root = other.m_root;
    m_active = other.m_active;
    m_stackCount = other.m_stackCount;
    m_stackIndex = other.m_stackIndex;
    return *this;
}
template<typename NodeType, bool Const, bool Reverse>
inline BinaryTreeIterator<NodeType, Const, Reverse>::BinaryTreeIterator(
    SelfType && other)
    : m_root(Move(other.m_root)),
    m_active(Move(other.m_active)),
    m_stackCount(Move(other.m_stackCount)),
    m_stackIndex(Move(other.m_stackIndex))
{
}
template<typename NodeType, bool Const, bool Reverse>
inline typename BinaryTreeIterator<NodeType, Const, Reverse>::SelfType &
BinaryTreeIterator<NodeType, Const, Reverse>::operator=(
    SelfType && other)
{
    m_root = Move(other.m_root);
    m_active = Move(other.m_active);
    m_stackCount = Move(other.m_stackCount);
    m_stackIndex = Move(other.m_stackIndex);
    return *this;
}
template<typename NodeType, bool Const, bool Reverse>
inline BinaryTreeIterator<NodeType, Const, Reverse>::operator bool() const
{
    return m_active != nullptr && m_root != nullptr;
}
/**Check and throw errors.*/
template<typename NodeType, bool Const, bool Reverse>
inline void BinaryTreeIterator<NodeType, Const, Reverse>::CheckAndThrow()
const
{
    if (!m_root || !m_active)
        throw BinaryTreeIteratorExceptions::NotDereferenceable;
}
/**Generate the node stack.*/
template<typename NodeType, bool Const, bool Reverse>
inline void BinaryTreeIterator<NodeType, Const, Reverse>::MakeStack()
{
    DeleteStack();
    if (!m_root)
        m_active = nullptr;
    m_active = new NodePtr[SizeHelper(m_root)];
    AddStack(m_root);
}
/**Add a node to the stack.
\param n The node to add.*/
template<typename NodeType, bool Const, bool Reverse>
inline void BinaryTreeIterator<NodeType, Const, Reverse>
::AddStack(NodePtr n)
{
    if (Reverse)
    {
        if (n->m_left)
            AddStack(n->m_left);
        m_active[m_stackCount++] = n;
        if (n->m_right)
            AddStack(n->m_right);
    }
    else
    {
        if (n->m_right)
            AddStack(n->m_right);
        m_active[m_stackCount++] = n;
        if (n->m_left)
            AddStack(n->m_left);
    }

}
/**Clear the stack and make this equal to a default constructed iterator*/
template<typename NodeType, bool Const, bool Reverse>
inline void BinaryTreeIterator<NodeType, Const, Reverse>::DeleteStack()
{
    if (m_active)
    {
        delete[] m_active;
        m_active = nullptr;
        m_stackCount = 0;
        m_stackIndex = 0;
    }
}
/**Move this to the next in order successor of the current active node.
\return Areference to this.*/
template<typename NodeType, bool Const, bool Reverse>
inline typename BinaryTreeIterator<NodeType, Const, Reverse>::SelfType &
BinaryTreeIterator<NodeType, Const, Reverse>::operator++()
{
    ++m_stackIndex;
    if (m_stackIndex >= (DiffType) m_stackCount)
    {
        DeleteStack();
        m_root = nullptr;
    }
    return *this;
}
template<typename NodeType, bool Const, bool Reverse>
inline typename BinaryTreeIterator<NodeType, Const, Reverse>::SelfType
BinaryTreeIterator<NodeType, Const, Reverse>::operator--(int)
{
    auto copy = *this;
    --(*this);
    return copy;
}
template<typename NodeType, bool Const, bool Reverse>
inline typename BinaryTreeIterator<NodeType, Const, Reverse>::SelfType &
BinaryTreeIterator<NodeType, Const, Reverse>::operator--()
{
    --m_stackIndex;
    if (m_stackIndex < 0)
    {
        DeleteStack();
        m_root = nullptr;
    }
    return *this;
}
/**Determine if this is equal to another iterator.
\return True if dereferenceing this will not produce the same reference as
\p other.*/
template<typename NodeType, bool Const, bool Reverse>
inline bool BinaryTreeIterator<NodeType, Const, Reverse>
::operator!=(const SelfType & other) const
{
    return m_active != other.m_active
        || m_root != other.m_root;
}
template<typename NodeType, bool Const, bool Reverse>
inline bool BinaryTreeIterator<NodeType, Const, Reverse>::operator==(
    const SelfType & other) const
{
    return !(*this != other);
}
template<typename NodeType, bool Const, bool Reverse>
inline bool BinaryTreeIterator<NodeType, Const, Reverse>::operator<(
    const SelfType & other) const
{
    return Addr()->m_b < other.Addr()->m_b;
}
template<typename NodeType, bool Const, bool Reverse>
inline bool BinaryTreeIterator<NodeType, Const, Reverse>::operator<=(
    const SelfType & other) const
{
    return (*this < other) || (*this == other);
}
template<typename NodeType, bool Const, bool Reverse>
inline bool BinaryTreeIterator<NodeType, Const, Reverse>::operator>(
    const SelfType & other) const
{
    return !(*this <= other);
}
template<typename NodeType, bool Const, bool Reverse>
inline bool BinaryTreeIterator<NodeType, Const, Reverse>::operator>=(
    const SelfType & other) const
{
    return !(*this < other);
}
template<typename NodeType, bool Const, bool Reverse>
inline typename BinaryTreeIterator<NodeType, Const, Reverse>::SelfType
BinaryTreeIterator<NodeType, Const, Reverse>::operator+(const SizeType & amt) const
{
    auto copy = *this;
    copy += amt;
    return copy;
}
template<typename NodeType, bool Const, bool Reverse>
inline typename BinaryTreeIterator<NodeType, Const, Reverse>::SelfType &
BinaryTreeIterator<NodeType, Const, Reverse>::operator+=(const SizeType & amt)
{
    for (SizeType i = 0; i < amt; ++i)
        ++(*this);
    return *this;
}
template<typename NodeType, bool Const, bool Reverse>
inline typename BinaryTreeIterator<NodeType, Const, Reverse>::SelfType
BinaryTreeIterator<NodeType, Const, Reverse>::operator-(
    const SizeType & amt) const
{
    auto copy = *this;
    copy -= amt;
    return copy;
}
template<typename NodeType, bool Const, bool Reverse>
inline typename BinaryTreeIterator<NodeType, Const, Reverse>::SelfType &
BinaryTreeIterator<NodeType, Const, Reverse>::operator-=(const SizeType & amt)
{
    for (SizeType i = 0; i < amt; ++i)
        --(*this);
    return *this;
}
template<typename NodeType, bool Const, bool Reverse>
inline typename BinaryTreeIterator<NodeType, Const, Reverse>::Ptr
BinaryTreeIterator<NodeType, Const, Reverse>::Addr()
{
    if (!m_active)
        MakeStack();
    CheckAndThrow();
    return &m_active[m_stackCount - m_stackIndex - 1]->m_data;
}
template<typename NodeType, bool Const, bool Reverse>
inline const typename BinaryTreeIterator<NodeType, Const, Reverse>::Ptr
BinaryTreeIterator<NodeType, Const, Reverse>::Addr() const
{
    CheckAndThrow();
    return &m_active[m_stackCount - m_stackIndex - 1]->m_data;
}
/**Dereference this iterator to access the data within.
\return A reference to the data.*/
template<typename NodeType, bool Const, bool Reverse>
inline typename BinaryTreeIterator<NodeType, Const, Reverse>::Ref
BinaryTreeIterator<NodeType, Const, Reverse>::operator*()
{
    return *Addr();
}

template<typename NodeType, bool Const, bool Reverse>
inline const typename BinaryTreeIterator<NodeType, Const, Reverse>::Ref
BinaryTreeIterator<NodeType, Const, Reverse>::operator*() const
{
    return *Addr();
}

template<typename NodeType, bool Const, bool Reverse>
inline typename BinaryTreeIterator<NodeType, Const, Reverse>::Ptr
BinaryTreeIterator<NodeType, Const, Reverse>::operator->()
{
    return Addr();
}

template<typename NodeType, bool Const, bool Reverse>
inline const typename BinaryTreeIterator<NodeType, Const, Reverse>::Ptr
BinaryTreeIterator<NodeType, Const, Reverse>::operator->() const
{
    return Addr();
}

template<typename NodeType, bool Const, bool Reverse>
inline typename BinaryTreeIterator<NodeType, Const, Reverse>::SelfType
BinaryTreeIterator<NodeType, Const, Reverse>::operator++(int)
{
    auto copy = *this;
    ++(*this);
    return copy;
}
/**Helper to count data.
\param node The node to start with.
\return The size of the tree starting at \p startNode*/
template<typename NodeType, bool Const, bool Reverse>
inline SizeType BinaryTreeIterator<NodeType, Const, Reverse>::SizeHelper(
    NodePtr startNode)
{
    if (!startNode)
        return 0;
    return 1 + SizeHelper(startNode->m_left)
        + SizeHelper(startNode->m_right);
}

}