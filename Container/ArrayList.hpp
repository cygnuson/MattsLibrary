#pragma once

#include "List.hpp"
#include "RandomAccessIterator.hpp"

#define _DEBUGARRAYLIST _DEBUG && _DEBUGARRAYNODE &&1

namespace cg {

template<typename T, 
	template <typename,typename,bool, bool> typename IteratorType>
class ArrayListImpl : public cg::List<T>
{
public:
	/**This type.*/
	using self_type = ArrayListImpl<T, IteratorType>;
	/**This type.*/
	using SelfType = self_type;
	/**The iterator type*/
	using Iterator = IteratorType
		<value_type, ArrayNode<value_type>, false, false>;
	/**The iterator type (STD COMPATIBILITY)*/
	using iterator = Iterator;
	/**The const iterator type*/
	using ConstIterator= IteratorType
		<value_type, ArrayNode<value_type>, true, false>;
	/**The const iterator type (STD COMPATIBILITY)*/
	using const_iterator = ConstIterator;
	/**The iterator type*/
	using ReverseIterator= IteratorType
		<value_type, ArrayNode<value_type>, false, true>; 
	/**The iterator type (STD COMPATIBILITY)*/
	using reverse_iterator = ReverseIterator;
	/**The const iterator type*/
	using ConstReverseIterator= IteratorType
		<value_type, ArrayNode<value_type>, true, true>;
	/**The const iterator type (STD COMPATIBILITY)*/
	using const_reverse_iterator = ConstReverseIterator;

	/**clean up*/
	~ArrayListImpl();
	/**Create an empty list. with default reservation.*/
	ArrayListImpl();
	/**Create an array list with a set of values.
	\param begin A start iterator.
	\param end An end iterator.*/
	template<typename R>
	ArrayListImpl(R begin, const R& end);
	/**Push an element to the back of the list.
	\param obj The obj to push back.*/
	void PushBack(const T& obj);
	/**Push an element to the back of the list.
	\param args Args to be sent directly to the ctor of the element.*/
	template<typename...Args>
	void EmplaceBack(Args&&...args);
	/**Push an element to the front of the list.
	\param obj The obj to push front.*/
	void PushFront(const T& obj);
	/**Push an element to the front of the list.
	\param args Args to be sent directly to the ctor of the element.*/
	template<typename...Args>
	void EmplaceFront(Args&&...args);
	/**Push an element to the list.
	\param index The place to put the element.
	\param obj The obj to push front.*/
	void Push(std::size_t index, const T& obj);
	/**Push an element to the the list.
	\param index The place to put the element.
	\param args Args to be sent directly to the ctor of the element.*/
	template<typename...Args>
	void Emplace(std::size_t index, Args&&...args);
	/**Increase the capacity of the list.
	\param amt The amount of cap to increase by.*/
	void Reserve(std::size_t amt);
	/**Get the size of the list.
	\return The size of the list.*/
	std::size_t Size() const;
	/**Get the size of the list. (STD COMPATABILITY WRAPPER)
	\return The size of the list.*/
	std::size_t size() const;
	/**Determine if the list is empty or not.
	\return True if the list is empty.*/
	bool Empty() const;
	/**Determine if the list is empty or not. (STD COMPATABILITY WRAPPER)
	\return True if the list is empty.*/
	bool empty() const;
	/**Get the data pointer tot he start of the data.
	\return A T* to the pointer containing the data.*/
	const T* Data() const;
	/**Get the data pointer tot he start of the data.
	(STD COMPATABILITY WRAPPER)
	\return A T* to the pointer containing the data.*/
	const T* data() const;
	/**Get a begining iterator.
	\return The Begin iterator.*/
	Iterator Begin();
	/**Get a begining iterator.
	\return The Begin iterator.*/
	ConstIterator Begin()const;
	/**Get a begining reverse iterator.
	\return The Begin iterator.*/
	ReverseIterator RBegin();
	/**Get a begining reverse iterator.
	\return The Begin iterator.*/
	ConstReverseIterator RBegin()const;
	/**Get a end iterator.
	\return The end iterator.*/
	Iterator End();
	/**Get a end iterator.
	\return The end iterator.*/
	ConstIterator End() const;
	/**Get a end reverse iterator.
	\return The end iterator.*/
	ReverseIterator REnd();
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
	void Init(std::size_t cap = 0);
	/**The capacity of the array list.*/
	std::size_t m_cap = 8;
	/**The size of the array list*/
	std::size_t m_size = 0;
	/**the pointer to the array.*/
	Pointer m_data = nullptr;
};

template<typename T, 
	template <typename,typename,bool, bool> typename IteratorType>
inline ArrayListImpl<T,IteratorType>::~ArrayListImpl()
{
	cg::DeleteA(m_data);
}

template<typename T,
	template <typename,typename,bool, bool> typename IteratorType>
inline ArrayListImpl<T,IteratorType>::ArrayListImpl()
{
	Init();
}

template<typename T, 
	template <typename,typename,bool, bool> typename IteratorType>
inline void ArrayListImpl<T,IteratorType>::PushBack(const T & obj)
{
	Push(m_size, obj);
}

template<typename T, 
	template <typename,typename,bool, bool> typename IteratorType>
inline void ArrayListImpl<T,IteratorType>::PushFront(const T & obj)
{
	Push(0, obj);
}

template<typename T, 
	template <typename,typename,bool, bool> typename IteratorType>
inline void ArrayListImpl<T,IteratorType>::Push(std::size_t index, const T & obj)
{
	if (m_size + 1 > m_cap)
		Reserve(8);
	if (index > m_size)
		throw IndexOutOfBoundsException();
	if (index == m_size)
	{
		cg::PNew<T>(&m_data[m_size++], obj);
		return;
	}
	auto sizeToMove = m_size++ - index;
	std::memmove(m_data + index + 1, m_data + index, sizeToMove * sizeof(T));
	cg::PNew<T>(&m_data[index], obj);
}

template<typename T,
	template <typename,typename,bool, bool> typename IteratorType>
inline void ArrayListImpl<T,IteratorType>::Reserve(std::size_t amt)
{
	m_cap += amt;
	auto newData = cg::NewA<T>(m_cap);
	std::memcpy(newData, m_data, m_size * sizeof(T));
	auto oldData = m_data;
	m_data = newData;
	cg::DeleteA(oldData);
}

template<typename T,
	template <typename,typename,bool, bool> typename IteratorType>
inline std::size_t ArrayListImpl<T,IteratorType>::Size() const
{
	return m_size;
}

template<typename T,
	template <typename,typename,bool, bool> typename IteratorType>
inline std::size_t ArrayListImpl<T,IteratorType>::size() const
{
	return Size();
}

template<typename T, 
	template <typename,typename,bool, bool> typename IteratorType>
inline bool ArrayListImpl<T,IteratorType>::Empty() const
{
	return m_size == 0;
}

template<typename T,
	template <typename,typename,bool, bool> typename IteratorType>
inline bool ArrayListImpl<T,IteratorType>::empty() const
{
	return Empty();
}

template<typename T, 
	template <typename,typename,bool, bool> typename IteratorType>
inline const T * ArrayListImpl<T,IteratorType>::Data() const
{
	return m_data;
}

template<typename T,
	template <typename,typename,bool, bool> typename IteratorType>
inline const T * ArrayListImpl<T,IteratorType>::data() const
{
	return Data();
}

template<typename T,
	template <typename,typename,bool, bool> typename IteratorType>
inline typename ArrayListImpl<T,IteratorType>::Iterator
ArrayListImpl<T,IteratorType>::Begin()
{
	auto ret = Iterator(m_data);
#if _DEBUGARRAYLIST
	ret.D_DebugBounds(m_data - 1, m_data + m_size);
#endif
	return ret;
}

template<typename T,
	template <typename,typename,bool, bool> typename IteratorType>
inline typename ArrayListImpl<T,IteratorType>::ConstIterator
ArrayListImpl<T,IteratorType>::Begin() const
{
	auto ret = ConstIterator(m_data);
#if _DEBUGARRAYLIST
	ret.D_DebugBounds(m_data - 1, m_data + m_size);
#endif
	return ret;
}

template<typename T,
	template <typename,typename,bool, bool> typename IteratorType>
inline typename ArrayListImpl<T,IteratorType>::ReverseIterator
ArrayListImpl<T,IteratorType>::RBegin()
{
	auto ret = ReverseIterator(m_data+m_size-1);
#if _DEBUGARRAYLIST
	ret.D_DebugBounds(m_data - 1, m_data + m_size);
#endif
	return ret;
}

template<typename T,
	template <typename,typename,bool, bool> typename IteratorType>
inline typename ArrayListImpl<T,IteratorType>::ConstReverseIterator
ArrayListImpl<T,IteratorType>::RBegin() const
{
	auto ret = ConstReverseIterator(m_data + m_size-1 );
#if _DEBUGARRAYLIST
	ret.D_DebugBounds(m_data - 1, m_data + m_size);
#endif
	return ret;
}

template<typename T,
	template <typename,typename,bool, bool> typename IteratorType>
inline typename ArrayListImpl<T,IteratorType>::Iterator
ArrayListImpl<T,IteratorType>::End()
{
	auto ret = Iterator(m_data + m_size);
#if _DEBUGARRAYLIST
	ret.D_DebugBounds(m_data - 1, m_data + m_size);
#endif
	return ret;
}

template<typename T,
	template <typename,typename,bool, bool> typename IteratorType>
inline typename ArrayListImpl<T,IteratorType>::ConstIterator
ArrayListImpl<T,IteratorType>::End() const
{
	auto ret = ConstIterator(m_data + m_size);
#if _DEBUGARRAYLIST
	ret.D_DebugBounds(m_data - 1, m_data + m_size);
#endif
	return ret;
}

template<typename T,
	template <typename,typename,bool, bool> typename IteratorType>
inline typename ArrayListImpl<T,IteratorType>::ReverseIterator
ArrayListImpl<T,IteratorType>::REnd()
{
	auto ret = ReverseIterator(m_data-1);
#if _DEBUGARRAYLIST
	ret.D_DebugBounds(m_data - 1, m_data + m_size);
#endif
	return ret;
}

template<typename T,
	template <typename,typename,bool, bool> typename IteratorType>
inline typename ArrayListImpl<T,IteratorType>::ConstReverseIterator
ArrayListImpl<T,IteratorType>::REnd() const
{
	auto ret = ConstReverseIterator(m_data-1);
#if _DEBUGARRAYLIST
	ret.D_DebugBounds(m_data - 1, m_data + m_size);
#endif
	return ret;
}

template<typename T, 
	template <typename,typename,bool, bool> typename IteratorType>
inline typename ArrayListImpl<T,IteratorType>::ConstIterator
ArrayListImpl<T,IteratorType>::CBegin() const
{
	auto ret = ConstIterator(m_data);
#if _DEBUGARRAYLIST
	ret.D_DebugBounds(m_data - 1, m_data + m_size);
#endif
	return ret;
}

template<typename T,
	template <typename,typename,bool, bool> typename IteratorType>
inline typename ArrayListImpl<T,IteratorType>::ConstReverseIterator
ArrayListImpl<T,IteratorType>::CRBegin() const
{
	auto ret = ConstReverseIterator(m_data + m_size - 1);
#if _DEBUGARRAYLIST
	ret.D_DebugBounds(m_data - 1, m_data + m_size);
#endif
	return ret;
}

template<typename T, 
	template <typename,typename,bool, bool> typename IteratorType>
inline typename ArrayListImpl<T,IteratorType>::ConstIterator
ArrayListImpl<T,IteratorType>::CEnd() const
{
	auto ret = ConstIterator(m_data + m_size);
#if _DEBUGARRAYLIST
	ret.D_DebugBounds(m_data - 1, m_data + m_size);
#endif
	return ret;
}

template<typename T,
	template <typename,typename,bool, bool> typename IteratorType>
inline typename ArrayListImpl<T,IteratorType>::ConstReverseIterator
ArrayListImpl<T,IteratorType>::CREnd() const
{
	ConstReverseIterator ret = ConstReverseIterator(m_data-1);
#if _DEBUGARRAYLIST
	ret.D_DebugBounds(m_data - 1, m_data + m_size);
#endif
	return ret;
}

template<typename T,
	template <typename,typename,bool, bool> typename IteratorType>
inline void ArrayListImpl<T,IteratorType>::Init(std::size_t cap)
{
	if (cap)
		m_cap = cap;
	if (!m_data)
		m_data = cg::NewA<T>(m_cap);
}

template<typename T,
	template <typename,typename,bool, bool> typename IteratorType>
template<typename R>
inline ArrayListImpl<T,IteratorType>::ArrayListImpl(R begin, const R & end)
{
	Init();
	while (begin++ != end)
		PushBack(*begin);
}

template<typename T, 
	template <typename,typename,bool, bool> typename IteratorType>
template<typename ...Args>
inline void ArrayListImpl<T,IteratorType>::EmplaceBack(Args && ...args)
{
	Emplace(m_size, std::forward<Args>(args)...);
}

template<typename T,
	template <typename,typename,bool, bool> typename IteratorType>
template<typename ...Args>
inline void ArrayListImpl<T,IteratorType>::EmplaceFront(Args && ...args)
{
	Emplace(0, std::forward<Args>(args)...);
}

template<typename T,
	template <typename,typename,bool, bool> typename IteratorType>
template<typename ...Args>
inline void ArrayListImpl<T,IteratorType>::Emplace(std::size_t index, Args && ...args)
{
	if (m_size + 1 > m_cap)
		Reserve(8);
	if (index > m_size)
		throw IndexOutOfBoundsException();
	if (index == m_size)
	{
		cg::PNew<T>(&m_data[m_size++], std::forward<Args>(args)...);
		return;
	}
	auto sizeToMove = m_size++ - index;
	std::memmove(m_data + index + 1, m_data + index, sizeToMove * sizeof(T));
	cg::PNew<T>(&m_data[index], std::forward<Args>(args)...);
}

template<typename T>
using ArrayList = ArrayListImpl<T, cg::RandomAccessIterator>;

#if _DEBUGARRAYLIST
/**Force some instantiations*/
template class ArrayListImpl<int,cg::RandomAccessIterator>;
#endif
}