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

/**The node type of the linked list.*/
template<typename DataType>
class LinkedListNode
{
    template<typename ListDataType> friend class LinkedList;
    template<typename DataType, bool Const, bool Reverse>
    friend class LinkedListIterator;
    /**Default ctor*/
    LinkedListNode() {};
    /**Create with data
    \param data The data.
    \param next The next pointer.
    \param prev The previous pointer.*/
    LinkedListNode(DataType&& data, LinkedListNode<DataType>* next,
        LinkedListNode<DataType>* prev) :m_next(next), m_prev(prev),
        m_data(Forward<DataType>(data)) {};
    /**Create with data
    \param next The next pointer.
    \param prev The previous pointer.*/
    LinkedListNode(LinkedListNode<DataType>* next,
        LinkedListNode<DataType>* prev) :m_next(next), m_prev(prev) {};
    /**Create in place
    \param next The next pointer.
    \param prev The previous pointer.
    \param ts The constructor args*/
    template<typename...Ts>
    LinkedListNode(LinkedListNode<DataType>* next, 
        LinkedListNode<DataType>* prev, Ts&&...ts) 
        :m_next(next), m_prev(prev) 
    {
        new (&m_data) DataType(Forward<Ts>(ts)...);
    };
    /**Destroy*/
    ~LinkedListNode() {};
    /**Clean off this node, deleteing any children.*/
    void Clean() {
        if (m_next)
        {
            m_next->Clean();
            delete m_next;
        }
    }
    /**Determine if this node is equal to another.
    \param o The other node.*/
    bool operator==(const LinkedListNode<DataType>& o) const
    {
        return this == &o;
    }
    /**Determine if this node is equal to another.
    \param o A pointer to the other node.*/
    bool operator==(const LinkedListNode<DataType>* o) const
    {
        return this == o;
    }
    /**A pointer to the next node.*/
    LinkedListNode<DataType>* m_next;
    /**The previous node.*/
    LinkedListNode<DataType>* m_prev;
    /**The data within*/
    DataType m_data;
};

}