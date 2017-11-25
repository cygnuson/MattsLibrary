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
    TreePtr tree) :m_tree(tree), m_active(nullptr), m_stackCount(0)
{

}
/**Create a node based on a tree \p tree.
\pre \p startNode must be reachable through \p tree.
\p tree The tree to iterate.
\p startNode The node for which to start iterating.*/
template<typename NodeType, bool Const, bool Reverse>
inline BinaryTreeIterator<NodeType, Const, Reverse>::BinaryTreeIterator(
    TreePtr tree, NodeType * startNode)
    :m_tree(tree), m_active(startNode), m_justDidLeft(false),
    m_stackCount(0)
{
}
/**Check and throw errors.*/
template<typename NodeType, bool Const, bool Reverse>
inline void BinaryTreeIterator<NodeType, Const, Reverse>::CheckAndThrow() const
{
    if (!m_tree || !m_active)
        throw BinaryTreeIteratorExceptions::NotDereferenceable;
}
/**Generate the node stack.*/
template<typename NodeType, bool Const, bool Reverse>
inline void BinaryTreeIterator<NodeType, Const, Reverse>::MakeStack()
{
    DeleteStack();
    if (!m_tree)
        m_active = nullptr;
    m_active = new NodeType*[m_tree->m_size];
    AddStack(m_tree->m_root);
}
/**Add a node to the stack.
\param n The node to add.*/
template<typename NodeType, bool Const, bool Reverse>
inline void BinaryTreeIterator<NodeType, Const, Reverse>
::AddStack(NodeType * n)
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
    if (m_stackIndex >= m_stackCount)
    {
        DeleteStack();
        m_tree = nullptr;
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
        || m_tree != other.m_tree;
}
/**Dereference this iterator to access the data within.
\return A reference to the data.*/
template<typename NodeType, bool Const, bool Reverse>
inline typename BinaryTreeIterator<NodeType, Const, Reverse>::Ref
BinaryTreeIterator<NodeType, Const, Reverse>::operator*()
{
    if (!m_active)
        MakeStack();
    CheckAndThrow();
    return m_active[m_stackCount - m_stackIndex - 1]->m_data;
}

}