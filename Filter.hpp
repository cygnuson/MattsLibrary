#pragma once


#include <thread>

#include "ArrayView.hpp"

namespace cg {

class Filter
{
public:
	/**Covnert data that will be converted *inplace* meaning that the pointer
	provided will both be the input and the output.
	\param data The place to read, and store the data.
	\param size The size of the data to process.*/
	inline void operator()(char* data, std::size_t size)
	{
		(*this)(data, data, size,size);
	}
	/**Covnert data that will be converted *inplace* meaning that the pointer
	provided will both be the input and the output.
	\tparam T An object with .data() and .size().
	\param data The place to read, and store the data.*/
	template<typename T>
	inline void operator()(T& data)
	{
		(*this)(data.data(), data.size());
	}


	/**Covnert data from one place and store to another.  Dest and source may
	be the same, but may not overlap if they are different pointers.
	\param dest The place to store the data.
	\param src The place to read the data from.
	\param dSize The size of the data source.
	\param sSize The size of the data destination.  If the destination size is
	smaller than the source size the result is undefined behavior.*/
	virtual void operator()(char* dest, const char* src, 
		std::size_t dSize, std::size_t sSize) = 0;
	/**Covnert data that will be converted *inplace* meaning that the pointer
	provided will both be the input and the output.
	\tparam T An object with .data() and .size().
	\tparam Y An object with .data() and .size().
	\param dest The place to store the data.
	\param src The place to read the data from.*/
	template<typename T,typename Y>
	inline void operator()(T& dest, const Y& src)
	{
		(*this)(dest.data(),src.data(), dest.size(), src.size());
	}
};

}