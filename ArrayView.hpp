#pragma once

#include <cstddef>

namespace cg {

template<typename T>
struct ArrayView
{
	/**Create the array view.
	\param data A pointer to the data.
	\param size The size of the data in elements.*/
	ArrayView(T* data = nullptr, std::size_t size = 0)
		:m_data(data), m_size(size) {};
	/**Create the array view.
	\param size The size of the data in elements.*/
	ArrayView(std::size_t size)
		:m_data(new T[size]), m_size(size)
	{
		m_destroy = true;
	};
	/**Move ctor
	\param other The thing to move.*/
	ArrayView(ArrayView&& other)
	{
		m_data = other.m_data;
		m_destroy = other.m_destroy;
		m_size = other.m_size;
		other.m_data = nullptr;
		other.m_size = 0;
		other.m_destroy = false;
	}
	/**Copy ctor
	\param other The thing to copy.  Data will be deep copied.*/
	ArrayView(const ArrayView& other)
	{
		m_data = new char[other.m_size];
		m_size = other.m_size;
		std::memcpy(m_data, other.m_data, m_size);
		m_destroy = true;
	}
	/**Move assign
	\param other The thing to move.*/
	void operator=(ArrayView&& other)
	{
		m_data = other.m_data;
		m_destroy = other.m_destroy;
		m_size = other.m_size;
		other.m_data = nullptr;
		other.m_size = 0;
		other.m_destroy = false;
	}
	/**Copy assign
	\param other The thing  to copy.  Data will not be copied, only the
	pointer.  This object will not be able to delete the pointer.*/
	void operator=(const ArrayView& other)
	{
		m_data = other.m_data;
		m_size = other.m_size
	}
	/**Destruct and if needed, destroy the data.*/
	~ArrayView()
	{
		if (m_destroy)
			Delete();
	}
	/**Get a pointer to the data.
	\return A pointer to the data.*/
	inline T* data()
	{
		return m_data;
	}
	/**Get a const pointer to the data.
	\return A pointer to the data.*/
	inline const T* data() const
	{
		return m_data;
	}
	/**Get the size of the data region.
	\return The amount of elements for the data.*/
	inline const std::size_t size() const
	{
		return m_size;
	}
	/**Call delete on the array. Should only be used when the ArrayView is
	constructed with just a size.  Has no effect if the pointer was not
	allocated during construction.*/
	inline void Delete()
	{
		delete[] m_data;
		m_data = nullptr;
		m_size = 0;
	}
private:
	/**A pointer to the data.*/
	T* m_data;
	/**The size of the data.*/
	std::size_t m_size;
	/**True if the data should be deleted when finished.*/
	bool m_destroy = false;

};

}