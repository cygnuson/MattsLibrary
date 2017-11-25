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
#include "cgdef.hpp"
namespace cg {
/**A class for a search tree.*/
template<typename T, typename K, typename P>
class BinaryTreeNode {
public:
    using DataType = T;
    using KeyType = K;
    using Predicate = P;
    /**The type of pair*/
    using PairType = Pair<DataType, KeyType>;
    /**Create the node.
    \param p The data to set.
    \param l The left node.
    \param r the right node.*/
    BinaryTreeNode(PairType&& p, BinaryTreeNode<T, K, P>* l,
        BinaryTreeNode<T, K, P>* r) :m_data(Forward<PairType>(p)),
        m_left(l), m_right(r) {}
    /**Create the node.
    \param l The left node.
    \param r the right node.*/
    BinaryTreeNode(BinaryTreeNode<T, K, P>* l, BinaryTreeNode<T, K, P>* r)
        : m_left(l), m_right(r) {}
    /**The data held*/
    PairType m_data;
    /**A pointer to the left child.*/
    BinaryTreeNode<T, K, P>* m_left;
    /**A pointer to the right child.*/
    BinaryTreeNode<T, K, P>* m_right;

};
}