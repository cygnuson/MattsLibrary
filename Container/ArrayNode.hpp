#pragma once

#include "Node.hpp"

namespace cg {

/**
The array node is a node for holding data in single large chunk of memory.
\tparam T The data type.
*/
template<typename T>
struct ArrayNode : public cg::Node<T>
{
	using self_type = ArrayNode<T>;
	/**default ctor*/
	ArrayNode(): m_ptr(nullptr) {};
	/**Construct the node from an already created poitner.
	\param ptr The pointer.*/
	ArrayNode(T* ptr);
	/**Construct the node with new memory.
	\param size The amount to allocate.*/
	ArrayNode(std::size_t size);
	/**Incrememnt this pointer node.
	\param nothing dummy op.
	\return a copy of the node incremented.*/
	self_type operator++(int nothing);
	/**Incrememnt this pointer node.
	\return this node incremented.*/
	self_type operator++();
	/**The pointer to the current position.*/
	T* m_ptr;
};


}