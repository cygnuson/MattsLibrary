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
	inline void Transform(char* data, std::size_t size)
	{
		this->Transform(data, data, size,size);
	}
	/**Covnert data that will be converted *inplace* meaning that the pointer
	provided will both be the input and the output.
	\tparam T An object with .data() and .size().
	\param data The place to read, and store the data.*/
	template<typename T>
	inline void Transform(T& data)
	{
		this->Transform(data.data(), data.size());
	}


	/**Covnert data from one place and store to another.  Dest and source may
	be the same, but may not overlap if they are different pointers.
	\param dest The place to store the data.
	\param src The place to read the data from.
	\param dSize The size of the data source.
	\param sSize The size of the data destination.  If the destination size is
	smaller than the source size the result is undefined behavior.*/
	virtual void Transform(char* dest, const char* src,
		std::size_t dSize, std::size_t sSize) = 0;
	/**Covnert data that will be converted *inplace* meaning that the pointer
	provided will both be the input and the output.
	\tparam T An object with .data() and .size().
	\tparam Y An object with .data() and .size().
	\param dest The place to store the data.
	\param src The place to read the data from.*/
	template<typename T,typename Y>
	inline std::enable_if_t<!std::is_fundamental<Y>::value,void>
		Transform(T& dest, const Y& src)
	{
		this->Transform(dest.data(),src.data(), dest.size(), src.size());
	}
	/**Transform with any set of aprameters as log as there is an overload for
	them.
	
	\return A future with the result.*/
	template<typename...Args>
	inline void AsyncTransform(Args...args)
	{
		return std::async(std::launch::async, [&]() {
			this->Transform(std::forward<Args>(args)...);
		});
	}
	/**Do a transformation, but return an ArrayView<char> instead of using
	parameters.

	\param dSize The size of the detination memory.
	\tparam Y Something with a .data() const and .size() const to read from.
	\tparam T Something that can be constructed with a char* and size_t.
	\return An object of type T with the results.*/
	template<typename T, typename Y>
	inline T TransformView(Y& src, std::size_t dSize)
	{
		char* data = new char[dSize];
		Transform(data, src.data(), dSize, src.size());
		return T(data, dSize);
	}
	/**Do a transformation, but return an ArrayView<char> instead of using
	parameters.

	\param dSize The size of the detination memory.
	\param src The source of data.
	\param sSize The size of the source.
	\tparam T Something that can be constructed with a char* and size_t.
	\return An object of type T with the results.*/
	template<typename T>
	inline T TransformView(const char* src, 
		std::size_t sSize, 
		std::size_t dSize)
	{
		char* data = new char[dSize];
		Transform(data, src, dSize, sSize);
		return T(data, dSize);
	}
	/**Transform with any set of aprameters as log as there is an overload for
	them.

	\tparam T The type of container for returning.
	\return A future with the result.*/
	template<typename T, typename...Args>
	inline std::future<T> AsyncTransformView(Args...args)
	{
		return std::async(std::launch::async, [&]() {
			return this->TransformView<T>(std::forward<Args>(args)...);
		});
	}
};

}