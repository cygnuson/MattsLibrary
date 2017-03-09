#pragma once


#include <thread>

#include "ArrayView.hpp"

namespace cg {

class Filter
{
public:
	/**Virtual destructor*/
	virtual ~Filter() {};
	/**Covnert data from one place and store to another.  Dest and source may
	be the same, but may not overlap if they are different pointers.
	\param src The place to read the data from.
	\param size The size of the data destination.
	\return An array view with the converted data.*/
	virtual ArrayView TransformCopy(const char* src, std::size_t size)
	{
		auto av = ArrayView::Copy(src, size);
		Filter::Transform(av);
		return av;
	}
	/**Transform data in place (no copies).
	\param av The array view to transform.*/
	virtual void Transform(ArrayView& av)
	{
		Transform(av.data(), av.size());
	}
	/**Transform data in place (no copies).
	\param data The data place.
	\param size The data size.*/
	virtual void Transform(char* data, std::size_t size) = 0;
	/**Transform and array view in place.
	\param av The array view to transform.
	\return Another array view.*/
	virtual ArrayView TransformCopy(ArrayView& av)
	{
		return TransformCopy(av.data(), av.size());
	}
	/**Transform with any set of aprameters as log as there is an overload for
	them.

	\return A future with the result.*/
	template<typename...Args>
	inline void AsyncTransformCopy(Args...args)
	{
		return std::async(std::launch::async, [&]() {
			this->TransformCopy(std::forward<Args>(args)...);
		});
	}
	/**Transform with any set of aprameters as log as there is an overload for
	them.

	\return A future with the result.*/
	template<typename...Args>
	inline void AsyncTransform(Args...args)
	{
		return std::async(std::launch::async, [&]() {
			this->TransformInPlace(std::forward<Args>(args)...);
		});
	}
};

}