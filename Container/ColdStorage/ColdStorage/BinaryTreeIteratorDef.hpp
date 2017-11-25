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

#include "BinaryTreeNode.hpp"

namespace cg {

template<typename DataType, typename KeyType, typename Predicate>
class BinaryTree;

enum class BinaryTreeIteratorExceptions
{

    /**The iterator was not dereferencable.*/
    NotDereferenceable,
};

template<typename NodeType, bool Const, bool Reverse>
class BinaryTreeIterator 
{
public:
    using DataType = typename NodeType::DataType;
    using KeyType = typename NodeType::KeyType;
    using Predicate = typename NodeType::Predicate;
    using TreeType = BinaryTree<DataType, KeyType, Predicate>;
    using PairType = typename TreeType::PairType;
    /**The type of data reference to return.*/
    using Ref = ConditionalType<Const, const PairType&, PairType&>;
    /**The type of data reference to return.*/
    using Ptr = ConditionalType<Const, const PairType*, PairType*>;
    /**The type of data reference to return.*/
    using TreePtr = ConditionalType<Const, const TreeType*, TreeType*>;
    /**The type of this object.*/
    using SelfType = BinaryTreeIterator<NodeType, Const, Reverse>;

    BinaryTreeIterator() : m_active(nullptr), m_tree(nullptr) {}

    ~BinaryTreeIterator() {
        DeleteStack();
    }

    BinaryTreeIterator(TreePtr tree);

    BinaryTreeIterator(TreePtr tree, NodeType* startNode);

    BinaryTreeIterator(NodeType* p);

    BinaryTreeIterator(const SelfType& other);

    SelfType& operator=(const SelfType& other);

    BinaryTreeIterator(SelfType&& other);

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

    Ptr Addr();

    const Ptr Addr() const;

private:
    /**Check and throw and exceptions needed*/
    void CheckAndThrow() const;
    /**A reference to the tree*/
    TreePtr m_tree;
    /**the active node.*/
    NodeType** m_active;
    /**The stack counter*/
    SizeType m_stackCount;
    /**The stack index.*/
    SizeType m_stackIndex;
    
    void MakeStack();

    void AddStack(NodeType* n);

    void DeleteStack();

};

}