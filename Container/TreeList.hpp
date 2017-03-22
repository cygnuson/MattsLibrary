#pragma once

#include "TreeIterator.hpp"
#include "List.hpp"

#define _DEBUGTREELIST _DEBUG && _DEBUGTREEITERATOR && 1

namespace cg {

template<typename T,
	template <typename, typename, bool, bool> typename IteratorType>
class TreeListImpl : public cg::List<T>
{
public:/**This type.*/
	using self_type = TreeListImpl<T, IteratorType>;
	/**This type.*/
	using SelfType = self_type;
	/**The iterator type*/
	using Iterator = IteratorType
		<value_type, BinaryTreeNode<value_type>, false, false>;
	/**The iterator type (STD COMPATIBILITY)*/
	using iterator = Iterator;
	/**The const iterator type*/
	using ConstIterator = IteratorType
		<value_type, BinaryTreeNode<value_type>, true, false>;
	/**The const iterator type (STD COMPATIBILITY)*/
	using const_iterator = ConstIterator;
	/**The iterator type*/
	using ReverseIterator = IteratorType
		<value_type, BinaryTreeNode<value_type>, false, true>;
	/**The iterator type (STD COMPATIBILITY)*/
	using reverse_iterator = ReverseIterator;
	/**The const iterator type*/
	using ConstReverseIterator = IteratorType
		<value_type, BinaryTreeNode<value_type>, true, true>;
	/**The const iterator type (STD COMPATIBILITY)*/
	using const_reverse_iterator = ConstReverseIterator;
	/**Create a Tree list.*/
	TreeListImpl() {
		Init();
	};
	/**Create an array list with a set of values.
	\param begin A start iterator.
	\param end An end iterator.*/
	template<typename R>
	TreeListImpl(R begin, const R& end);
	/**Push an element to the list.
	\param index The place to put the element.
	\param obj The obj to push front.*/
	void Push(const T& obj);
	/**Push an element to the the list.
	\param index The place to put the element.
	\param args Args to be sent directly to the ctor of the element.*/
	template<typename...Args>
	void Emplace(Args&&...args);
	/**Get the size of the list.
	\return The size of the list.*/
	std::size_t Size() const;
	/**Get the size of the list. (STD COMPATABILITY WRAPPER)
	\return The size of the list.*/
	std::size_t size() const {
		return Size();
	}
	/**Determine if the list is empty or not.
	\return True if the list is empty.*/
	bool Empty() const;
	/**Determine if the list is empty or not. (STD COMPATABILITY WRAPPER)
	\return True if the list is empty.*/
	bool empty() const {
		return Empty();
	}
	/**Remove a value from the list.
	\param it The iterator whos value will be removed.*/
	template<typename It>
	void Remove(const It& it);
	/**Get a begining iterator.
	\return The Begin iterator.*/
	ConstIterator Begin()const;
	/**Get a begining reverse iterator.
	\return The Begin iterator.*/
	ConstReverseIterator RBegin()const;
	/**Get a end iterator.
	\return The end iterator.*/
	ConstIterator End() const;
	/**Get a end reverse iterator.
	\return The end iterator.*/
	ConstReverseIterator REnd()const;
	/**Get a begining iterator.
	\return The Begin iterator.*/
	ConstIterator CBegin()const;
	/**Get a begining reverse iterator.
	\return The Begin iterator.*/
	ConstReverseIterator CRBegin()const;
	/**Get a end iterator.
	\return The end iterator.*/
	ConstIterator CEnd() const;
	/**Get a end reverse iterator.
	\return The end iterator.*/
	ConstReverseIterator CREnd()const;
private:
	/**Init the data array.*/
	void Init();
	/**The size of the array list*/
	std::size_t m_size = 0;
	/**The root node.*/
	cg::BinaryTreeNode<T>* m_rootNode = nullptr;
};

template<typename T>
using TreeList = TreeListImpl<T, cg::TreeIterator>;


template<typename T,
	template <typename, typename, bool, bool> typename IteratorType>
template<typename R>
inline TreeListImpl<T, IteratorType>::TreeListImpl(R begin, const R & end)
{
	Init();
	for (; begin != end; ++begin)
		Push(*begin);
}


template<typename T,
	template <typename, typename, bool, bool> typename IteratorType>
template<typename ...Args>
inline void TreeListImpl<T, IteratorType>::
Emplace(Args && ...args)
{
	auto newNode = cg::New<BinaryTreeNode<T>>(
		cg::New<value_type>(std::forward<Args>(args)...));
	if (!m_rootNode)
		m_rootNode = newNode;
	else
		BinaryTreeNode<T>::Insert(&m_rootNode, newNode);
	++m_size;
}

template<typename T,
	template <typename, typename, bool, bool> typename IteratorType>
template<typename It>
inline void TreeListImpl<T,IteratorType>::Remove(const It & it)
{
	m_rootNode = BinaryTreeNode<value_type>::Remove(&m_rootNode, it.m_node);
}

template<typename T,
	template <typename, typename, bool, bool> typename IteratorType>
inline void TreeListImpl<T, IteratorType>::Init()
{

}

template<typename T,
	template <typename, typename, bool, bool> typename IteratorType>
inline void TreeListImpl<T, IteratorType>::Push(const T & obj)
{
	auto newNode = cg::New<BinaryTreeNode<T>>(
		cg::New<value_type>(obj));
	if (!m_rootNode)
		m_rootNode = newNode;
	else
		BinaryTreeNode<T>::Insert(&m_rootNode, newNode);
	++m_size;
}
template<typename T,
	template <typename, typename, bool, bool> typename IteratorType>
inline std::size_t TreeListImpl<T, IteratorType>::Size() const
{
	return m_size;
}
template<typename T,
	template <typename, typename, bool, bool> typename IteratorType>
inline bool TreeListImpl<T, IteratorType>::Empty() const
{
	return m_size == 0;
}

template<typename T,
	template <typename, typename, bool, bool> typename IteratorType>
inline typename TreeListImpl<T, IteratorType>::ConstIterator
TreeListImpl<T, IteratorType>::Begin() const
{
	auto begin = BinaryTreeNode<T>::BeforeBegin(m_rootNode);
	ConstIterator ret(begin, &m_rootNode);
	++ret;
	return ret;
}


template<typename T,
	template <typename, typename, bool, bool> typename IteratorType>
inline typename TreeListImpl<T, IteratorType>::ConstReverseIterator
TreeListImpl<T, IteratorType>::RBegin() const
{
	auto end = BinaryTreeNode<T>::AfterEnd(m_rootNode);
	ConstReverseIterator ret(end, &m_rootNode);
	--ret;
	return ret;
}

template<typename T,
	template <typename, typename, bool, bool> typename IteratorType>
inline typename TreeListImpl<T, IteratorType>::ConstIterator
TreeListImpl<T, IteratorType>::End() const
{
	auto end = BinaryTreeNode<T>::AfterEnd(m_rootNode);
	ConstIterator ret(end, &m_rootNode);
	return ret;
}
template<typename T,
	template <typename, typename, bool, bool> typename IteratorType>
inline typename TreeListImpl<T, IteratorType>::ConstReverseIterator
TreeListImpl<T, IteratorType>::REnd() const
{
	auto begin = BinaryTreeNode<T>::BeforeBegin(m_rootNode);
	ConstReverseIterator ret(begin, &m_rootNode);
	return ret;
}

template<typename T,
	template <typename, typename, bool, bool> typename IteratorType>
inline typename TreeListImpl<T, IteratorType>::ConstIterator
TreeListImpl<T, IteratorType>::CBegin() const
{
	return Begin();
}

template<typename T,
	template <typename, typename, bool, bool> typename IteratorType>
inline typename TreeListImpl<T, IteratorType>::ConstReverseIterator
TreeListImpl<T, IteratorType>::CRBegin() const
{
	return RBegin();
}

template<typename T,
	template <typename, typename, bool, bool> typename IteratorType>
inline typename TreeListImpl<T, IteratorType>::ConstIterator
TreeListImpl<T, IteratorType>::CEnd() const
{
	return End();
}

template<typename T,
	template <typename, typename, bool, bool> typename IteratorType>
inline typename TreeListImpl<T, IteratorType>::ConstReverseIterator
TreeListImpl<T, IteratorType>::CREnd() const
{
	return REnd();
}

}

#if _DEBUGTREELIST
/**Force some instantiations*/
template class cg::TreeListImpl<int, cg::TreeIterator>;
#endif