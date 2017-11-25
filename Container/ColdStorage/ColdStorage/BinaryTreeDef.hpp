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

#include "BinaryTreeIterator.hpp"

namespace cg {

/**Tree exceptions*/
enum class BinaryTreeException
{
    /**Key does not exist*/
    KeyDoesNotExist,
};

/**A class for a tree type data structure.*/
template<typename DataType, typename KeyType, typename Predicate>
class BinaryTree
{
public:
    /**The node type*/
    using NodeType = BinaryTreeNode<DataType, KeyType, Predicate>;
    /**The type of pair*/
    using PairType = Pair<DataType, KeyType>;
    /**A reverse moving iterator type*/
    using ReverseIterator = BinaryTreeIterator<NodeType, false, true>;
    /**The standard forward iterator*/
    using Iterator = BinaryTreeIterator<NodeType, false, false>;
    /**A const reverse moving iterator*/
    using ConstReverseIterator = BinaryTreeIterator<NodeType, true, true>;
    /**A forward moving const iterator*/
    using ConstIterator = BinaryTreeIterator<NodeType, true, false>;
    /**The type of self.*/
    using SelfType = BinaryTree<DataType, KeyType, Predicate>;

    BinaryTree() :m_root(nullptr) {}

    void Push(KeyType&& key, DataType&& o);

    void Push(PairType&& p);

    template<typename... Ts>
    void Emplace(KeyType&& key, Ts&&...ts);

    void Pop(KeyType&& key);

    DataType& Get(KeyType&& key);

    const DataType& Get(KeyType&& key) const;

    DataType& operator[](KeyType&& key);

    const DataType& operator[](KeyType&& key) const;

    SizeType Count(KeyType&& key) const;

    template<typename OutStream>
    void ShowKeys(OutStream& out) const;

    bool Empty() const;

    ConstIterator Begin() const;

    Iterator Begin();

    ConstReverseIterator RBegin() const;

    ReverseIterator RBegin();

    ConstIterator End() const;

    Iterator End();

    ConstReverseIterator REnd() const;

    ReverseIterator REnd();
private:
    template<typename NodeType, bool Const, bool Reverse>
    friend class BinaryTreeIterator;

    static void InsertHelper(NodeType*& startNode, PairType&& p);

    static PairType* AddressHelper(NodeType* startNode, KeyType&& key);

    static  DataType& GetHelper(NodeType* startNode, KeyType&& key);

    static void RemoveHelper(NodeType*& startNode, KeyType&& key);

    static SizeType CountHelper(NodeType* startNode, KeyType&& key);

    template<typename Stream>
    static void PrintHelper(NodeType* node, Stream& out);

    /**The root node*/
    NodeType* m_root;
    /**The size of the tree in nodes.*/
    SizeType m_size;
    /**The comparator*/
    static const Predicate pred;
};




}