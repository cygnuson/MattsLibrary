#pragma once

#include "Iterator.hpp"
#include "ArrayNode.hpp"

#define _DEBUGRANDOMACCESSITERATOR _DEBUG && 1

namespace cg {

template<typename T, typename NodeTypeT, bool Const = false,
	bool Reverse = false>
	class RandomAccessIterator : public Iterator<T, Const>
{
public:
	/**The type of node to use.*/
	using NodeType = NodeTypeT;
	/**The same type as this object.*/
	using self_type = RandomAccessIterator<T, NodeTypeT, Const, Reverse>;
	/**The same type as this object.*/
	using SelfType = self_type;
	/**An iterator tag to make this work with std stuff.*/
	using iterator_category = std::random_access_iterator_tag;
	/**An iterator tag to make this work with std stuff.*/
	using IteratorCategory = iterator_category;
	/**default ctor*/
	RandomAccessIterator() :m_node(nullptr) {};
	/**Create the iterator with an address.
	\param ptr The pointer to create with.*/
	RandomAccessIterator(pointer ptr);
	/**Copy asignment contructor.
	\param other The other thing to copy.*/
	RandomAccessIterator(const self_type& other);
	/**Move asignment contructor.
	\param other The other thing to move.*/
	RandomAccessIterator(self_type&& other);
	/**Copy assignment operator.
	\param other The other iterator.*/
	inline void operator=(const self_type& other);
	/**Move assignment operator.
	\param other The other iterator.*/
	inline void operator=(self_type&& other);
	/**Increment the poitner by 1. (prefix)
	\return This same type.*/
	inline self_type& operator++();
	/**Increment this by 1. (postfix)
	\param nothing Ignored pointer.
	\return a copy of this befor incrementing.*/
	inline self_type operator++(int nothing);
	/**Decrement the poitner by 1. (prefix)
	\return This same type after incrementing.*/
	inline self_type& operator--();
	/**Decrement this by 1. (postfix)
	\param nothing Ignored pointer.
	\return a copy of this before decrementing.*/
	inline self_type operator--(int nothing);
	/**Add to this node.
	\param amt The amount to increment.
	\return A reference to this.*/
	inline self_type& operator+=(std::size_t amt);
	/**Sub from this node.
	\param amt The amount to decrement.
	\return A reference to this.*/
	inline self_type& operator-=(std::size_t amt);
	/**The plus operator.
	\param amt The amount to add to the position.*/
	inline self_type operator+(std::size_t amt) const {
		auto copy = *this;
		return copy += amt;
	}
	/**The sub operator.
	\param amt The amount to sub from the position.*/
	inline SelfType operator-(std::size_t amt) const {
		return SelfType(NodeType::Difference(m_node, amt));
	}
	/**The sub operator.
	\param ptr The ptr to sub from this.*/
	inline DifferenceType operator-(pointer ptr) const {
		return NodeType::Difference(m_node, ptr);
	}
	/**The sub operator.
	\param other The object to sub from the position.
	\return The space between the this iterator and another.*/
	inline DifferenceType operator-(const self_type& other) const;
	/**Dereference operator.
	\return A reference to the data contained.*/
	inline RefReturnType operator*() {
		return *(this->operator->());
	}
	/**Poitner member access.
	\return A pointer to this (-> is appolied to the pointer too).*/
	inline PointerReturnType operator->();
	/**Compare for equality.
	\return True if the iterators are the same.*/
	inline bool operator==(const self_type& rhs) const {
		return NodeType::Equal(m_node, rhs.m_node);
	}
	/**Compare for equality.
	\return False if the iterators are the same.*/
	inline bool operator!=(const self_type& rhs) const {
		return !(*this == rhs);
	}
	/**Compare less than.
	\param other The other iterator to compare.
	\return True if other is before this in memory.*/
	inline bool operator<(const self_type& other) const {
		return NodeType::LessThan(m_node, other.m_node);
	}
	/**Greater than comparison.
	\param other The other thing to compare.
	\return True if the other thing is after this in memory.*/
	inline bool operator>(const self_type& other) const {
		return !((*this) <= other);
	}
	/**Compare less than or equals.
	\param other The other iterator to compare.
	\return True if other is before this in memory or at the same place.*/
	inline bool operator<=(const self_type& other) const {
		return (*this < other) || (*this == other);
	}
	/**Greater than or equals comparison.
	\param other The other thing to compare.
	\return True if the other thing is after this in memory or at the same
	place.*/
	inline bool operator>=(const self_type& other) const {
		return !(*this < other);
	}
#if _DEBUGRANDOMACCESSITERATOR
	pointer md_beforeFirst = nullptr;
	pointer md_afterLast = nullptr;
	/**Throw an error if the pointer is in an invalid location.*/
	void D_CheckBounds() const;
	/**Throw an error if the poitner is not dereferencable.*/
	void D_CheckValid() const;
	/**Set the debug members for the bounds.*/
	inline void D_DebugBounds(pointer beforeFirst, pointer afterLast);
#endif
private:
	/**Global plus operator for STL compatibility.*/
	template<typename T, typename NodeTypeT, bool Const, bool Reverse>
	friend std::ptrdiff_t operator+(std::size_t,
		const RandomAccessIterator<T, NodeTypeT, Const, Reverse>&);

	/**A pointer to the data.*/
	NodeType* m_node;
};

template<typename T, typename NodeTypeT, bool Const, bool Reverse>
typename RandomAccessIterator<T, NodeTypeT, Const, Reverse>::difference_type
operator+(std::size_t lhs,
	const RandomAccessIterator<T, NodeTypeT, Const, Reverse>& rhs)
{
	if (Reverse)
		return rhs.m_node->m_ptr - lhs;
	return rhs.m_node->m_ptr + lhs;
}
template<typename T, typename NodeTypeT, bool Const, bool Reverse>
inline RandomAccessIterator<T, NodeTypeT, Const, Reverse>
::RandomAccessIterator(pointer ptr)
	:m_node(cg::New<NodeType>(__FUNCSTR__,ptr))
{
#if _DEBUGRANDOMACCESSITERATOR
	m_node->D_DebugBounds(md_beforeFirst, md_afterLast);
#endif
}


template<typename T, typename NodeTypeT, bool Const, bool Reverse>
inline RandomAccessIterator<T, NodeTypeT, Const, Reverse>
::RandomAccessIterator(const self_type & other)
	:m_node(other.m_node)
{
#if _DEBUGRANDOMACCESSITERATOR

	md_beforeFirst = other.md_beforeFirst;
	md_afterLast = other.md_afterLast;
#endif
}

template<typename T, typename NodeTypeT, bool Const, bool Reverse>
inline RandomAccessIterator<T, NodeTypeT, Const, Reverse>
::RandomAccessIterator(self_type && other)
	:m_node(std::move(other.m_node))
{
#if _DEBUGRANDOMACCESSITERATOR

	md_beforeFirst = other.md_beforeFirst;
	md_afterLast = other.md_afterLast;
#endif
}

template<typename T, typename NodeTypeT, bool Const, bool Reverse>
inline void RandomAccessIterator<T, NodeTypeT, Const, Reverse>::operator=(
	const self_type & other)
{
	m_node = other.m_node;
#if _DEBUGRANDOMACCESSITERATOR
	md_beforeFirst = other.md_beforeFirst;
	md_afterLast = other.md_afterLast;
#endif
}

template<typename T, typename NodeTypeT, bool Const, bool Reverse>
inline void RandomAccessIterator<T, NodeTypeT, Const, Reverse>::operator=(
	self_type && other)
{
	m_node = std::move(other.m_node);
#if _DEBUGRANDOMACCESSITERATOR
	md_beforeFirst = std::move(other.md_beforeFirst);
	md_afterLast = std::move(other.md_afterLast);
#endif
}

template<typename T, typename NodeTypeT, bool Const, bool Reverse>
inline typename RandomAccessIterator<T, NodeTypeT, Const, Reverse>::self_type &
RandomAccessIterator<T, NodeTypeT, Const, Reverse>::operator++()
{
	if (!Reverse)
		NodeType::AdvanceNext(&m_node);
	else
		NodeType::AdvancePrev(&m_node);
#if _DEBUGRANDOMACCESSITERATOR
	D_CheckBounds();
#endif
	return *this;
}

template<typename T, typename NodeTypeT, bool Const, bool Reverse>
inline typename RandomAccessIterator<T, NodeTypeT, Const, Reverse>::self_type
RandomAccessIterator<T, NodeTypeT, Const, Reverse>::operator++(int nothing)
{
	auto copy = *this;
	++(*this);
	return copy;
}

template<typename T, typename NodeTypeT, bool Const, bool Reverse>
inline typename RandomAccessIterator<T, NodeTypeT, Const, Reverse>::self_type &
RandomAccessIterator<T, NodeTypeT, Const, Reverse>::operator--()
{
	if (Reverse)
		NodeType::AdvanceNext(&m_node);
	else
		NodeType::AdvancePrev(&m_node);
#if _DEBUGRANDOMACCESSITERATOR
	D_CheckBounds();
#endif
	return *this;
}

template<typename T, typename NodeTypeT, bool Const, bool Reverse>
inline typename RandomAccessIterator<T, NodeTypeT, Const, Reverse>::self_type
RandomAccessIterator<T, NodeTypeT, Const, Reverse>::operator--(int nothing)
{
	auto copy = *this;
	--(*this);
	return copy;
}

template<typename T, typename NodeTypeT, bool Const, bool Reverse>
inline typename RandomAccessIterator<T, NodeTypeT, Const, Reverse>::self_type &
RandomAccessIterator<T, NodeTypeT, Const, Reverse>::operator+=(std::size_t amt)
{
	if (!Reverse)
		NodeType::AdvanceNext(&m_node, amt);
	else
		NodeType::AdvancePrev(&m_node, amt);
#if _DEBUGRANDOMACCESSITERATOR
	D_CheckBounds();
#endif
	return *this;
}

template<typename T, typename NodeTypeT, bool Const, bool Reverse>
inline typename RandomAccessIterator<T, NodeTypeT, Const, Reverse>::self_type &
RandomAccessIterator<T, NodeTypeT, Const, Reverse>::operator-=(std::size_t amt)
{
	if (Reverse)
		NodeType::AdvanceNext(&m_node, amt);
	else
		NodeType::AdvancePrev(&m_node, amt);
#if _DEBUGRANDOMACCESSITERATOR
	D_CheckBounds();
#endif
	return *this;
}

template<typename T, typename NodeTypeT, bool Const, bool Reverse>
inline typename
RandomAccessIterator<T, NodeTypeT, Const, Reverse>::difference_type
RandomAccessIterator<T, NodeTypeT, Const, Reverse>::operator-(
	const self_type & other) const
{
#if _DEBUGRANDOMACCESSITERATOR
	D_CheckValid();
	other.D_CheckValid();
#endif 
	return NodeType::Difference(m_node, other.m_node);
}

template<typename T, typename NodeTypeT, bool Const, bool Reverse>
inline typename
RandomAccessIterator<T, NodeTypeT, Const, Reverse>::PointerReturnType
RandomAccessIterator<T, NodeTypeT, Const, Reverse>::operator->()
{
#if _DEBUGRANDOMACCESSITERATOR
	D_CheckValid();
#endif
	return NodeType::Get(m_node);
}


#if _DEBUGRANDOMACCESSITERATOR
template<typename T, typename NodeTypeT, bool Const, bool Reverse>
inline void RandomAccessIterator<T, NodeTypeT, Const, Reverse>::D_DebugBounds(
	pointer beforeFirst,
	pointer afterLast)
{
	md_beforeFirst = beforeFirst;
	md_afterLast = afterLast;
	m_node->D_DebugBounds(beforeFirst, afterLast);
}
template<typename T, typename NodeTypeT, bool Const, bool Reverse>
inline void 
RandomAccessIterator<T, NodeTypeT, Const, Reverse>::D_CheckBounds() const
{
	m_node->D_CheckBounds();
}
template<typename T, typename NodeTypeT, bool Const, bool Reverse>
inline void 
RandomAccessIterator<T, NodeTypeT, Const, Reverse>::D_CheckValid() const

{
	m_node->D_CheckValid();
}
#endif

/****************************************************************************/

#if _DEBUGRANDOMACCESSITERATOR
template class RandomAccessIterator<int, cg::ArrayNode<int>, false>;
template class RandomAccessIterator<int, cg::ArrayNode<int>, true>;
#endif

}
