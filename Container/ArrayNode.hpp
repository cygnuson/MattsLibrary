#pragma once

#include "Node.hpp"

#define _DEBUGARRAYNODE _DEBUG && 1

namespace cg {


/**A Node type that will exist under the hood.*/
template<typename T>
struct ArrayNode : public Node<T> {
	/**This type.*/
	using self_type = ArrayNode<T>;
	/**This type.*/
	using SelfType = self_type;
	/**Create the node with the pointer.
	\param ptr The pointer for the node.*/
	ArrayNode(Pointer ptr) : m_ptr(ptr) {};
	/**Delete the data Pointers. If before_begin node is deleted,
	than the whole chain gets deleted.*/
	~ArrayNode();
	/**Advance the node to the next node in the chain.
	\param node The node to advance.*/
	static void AdvanceNext(SelfType** node);
	/**Advance the node to the next node in the chain.
	\param amt The amount to advance.
	\param node The node to advance.*/
	static void AdvanceNext(SelfType** node, std::size_t amt);
	/**Advance the node to the previous node in the chain.
	\param node The node to advance.*/
	static void AdvancePrev(SelfType** node);
	/**Advance the node to the previous node in the chain.
	\param amt The amount to advance.
	\param node The node to advance.*/
	static void AdvancePrev(SelfType** node, std::size_t amt);
	/**Get the difference between two array node (lhs - rhs).
	\param lhs The left side of the op.
	\param rhs The right side of the op.
	\return The difference between two arbitrary nodes.*/
	static DifferenceType Difference(SelfType* lhs, SelfType* rhs);
	/**Get the difference between two array node (lhs - rhs).
	\param lhs The left side of the op.
	\param rhs The right side of the op.
	\return The difference between two arbitrary nodes.*/
	static DifferenceType Difference(SelfType* lhs, Pointer rhs);
	/**Get the difference between two array node (lhs - rhs).
	\param lhs The left side of the op.
	\param rhs The right side of the op.
	\return The difference between two arbitrary nodes.*/
	static Pointer Difference(SelfType* lhs, std::size_t rhs);
	/**Determine if one node is less than another.
	\param lhs The first node in the op.
	\param rhs The second node in the op.
	\return true if LHS comes before RHS.*/
	static bool LessThan(SelfType* lhs, SelfType* rhs) {
		return lhs->m_ptr < rhs->m_ptr;
	}
	/**Determine if one node is equal to another.
	\param lhs The first node in the op.
	\param rhs The second node in the op.
	\return true if LHS and RHS are the same Pointer.*/
	static bool Equal(SelfType* lhs, SelfType* rhs) {
		return lhs->m_ptr == rhs->m_ptr;
	}
	/**Dereference a node.
	\return A reference to the nodes data.*/
	static Pointer Dereference(SelfType* node) {
		return node->m_ptr;
	}
	/**A Pointer to the data.*/
	Pointer m_ptr;
#if _DEBUGARRAYNODE
	/**Debug only member to see the address of this object.*/
	const SelfType* md_self = this;
	/**Debug member for knowing if this is the before first.*/
	Pointer md_beforeFirst = nullptr;
	/**Debug member for knowing if this is the after last.*/
	Pointer md_afterLast = nullptr;
	/**Throw an error if the Pointer is in an invalid location.*/
	void D_CheckBounds() const;
	/**Throw an error if the poitner is not dereferencable.*/
	void D_CheckValid() const;
	/**Set the debug members for the bounds.*/
	inline void D_DebugBounds(Pointer beforeFirst, Pointer afterLast);
#endif
};

#if _DEBUGARRAYNODE
template<typename T>
inline void ArrayNode<T>::D_CheckBounds() const
{
	if (md_beforeFirst && md_afterLast)
		if (m_ptr < md_beforeFirst || m_ptr > md_afterLast)
		{
			cg::Logger::LogError(__FUNCSTR__,
				"The iterator is out of bounds.");
			throw IteratorOutOfBoundsException();
		}

	if (!m_ptr)
	{
		cg::Logger::LogError(__FUNCSTR__,
			"The iterator does not container a valid data Pointer.");
		throw NullPtrException();

	}
}

template<typename T>
inline void ArrayNode<T>::D_CheckValid() const
{

	if (md_beforeFirst && md_afterLast)
		if (m_ptr <= md_beforeFirst
			|| m_ptr >= md_afterLast)
		{
			cg::Logger::LogError(__FUNCSTR__,
				"The iterator is out of bounds.");
			throw IteratorOutOfBoundsException();
		}

	if (!m_ptr)
	{
		cg::Logger::LogError(__FUNCSTR__,
			"The iterator does not container a valid data Pointer.");
		throw NullPtrException();

	}
}

template<typename T>
inline void ArrayNode<T>::D_DebugBounds(Pointer beforeFirst,
	Pointer afterLast)
{
	this->md_beforeFirst = beforeFirst;
	this->md_afterLast = afterLast;
}
#endif
template<typename T>
inline ArrayNode<T>::~ArrayNode()
{
	/*dont delete here.*/
}

template<typename T>
inline void ArrayNode<T>::AdvanceNext(SelfType ** node)
{
	++(*node);
}

template<typename T>
inline void ArrayNode<T>::AdvanceNext(SelfType ** node, std::size_t amt)
{
	(*node) += amt;
}

template<typename T>
inline void ArrayNode<T>::AdvancePrev(SelfType ** node)
{
	--(*node)->m_ptr;
}

template<typename T>
inline void ArrayNode<T>::AdvancePrev(SelfType ** node,
	std::size_t amt)
{
	(*node)->m_ptr -= amt;
}

template<typename T>
inline typename ArrayNode<T>::DifferenceType
ArrayNode<T>::Difference(SelfType * lhs,
	SelfType * rhs)
{
	return lhs->m_ptr - rhs->m_ptr;
}

template<typename T>
inline typename ArrayNode<T>::DifferenceType
ArrayNode<T>::Difference(SelfType * lhs, Pointer rhs)
{
	return lhs->m_ptr - rhs;
}

template<typename T>
inline typename ArrayNode<T>::Pointer
ArrayNode<T>::Difference(SelfType * lhs,
	std::size_t rhs)
{
	return lhs->m_ptr - rhs;
}

#if _DEBUGARRAYNODE
/**Force some instantiations*/
template struct ArrayNode<int>;
#endif

}