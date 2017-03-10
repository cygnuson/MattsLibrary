#pragma once

#include <future>

#include "LogAdaptor.hpp"
#include "ArrayView.hpp"

namespace cg {

/**An interface class to facilitate read of various locations.*/
class Reader : cg::LogAdaptor<Reader>
{
public:
	/**Determine if the object is in a state that would allow it to send.
	\param timeout The time in micro seconds to wait untill returning. ZERO =
	no timeout, Lessthan ZERO = inf timeout.
	\return True if the object is ready to be written to without blocking.*/
	virtual bool ReadReady(std::ptrdiff_t timeout = 0) const = 0;
	/**Read from the object.
	\param timeout time untill return if the data does not get read. If the
	implementing class does not timeout (like a file or mem write) then this
	param should have a default value and be ignored.  The timeout is in micro
	seconds.
	\param expectedSize The max size to get. Might be less
	\return an ArrayView with data.  An ArrayView with nullptr and 0 size if
	no bytes are read.*/
	virtual cg::ArrayView Read(std::size_t expectedSize,
		std::ptrdiff_t timeout = -1) = 0;
	/**Read some data. T must have a member .data()  that will return a
	pointer to a location to store data. T must also have a member .size()const
	that will return the size of the data to be stored.
	\tparam T An object that has .data()  and .size() const for storing
	data.
	\param data An object of type T that has .data()  and .size() const .
	\param timeout The time in micro seconds to wait untill returning. If the
	op does not timeout then set a default value and ignore the parameter, 
	Lessthan ZERO = inf timeout.
	\return The amount of bytes read.*/
	template<typename T>
	inline std::ptrdiff_t Read(T & data, std::ptrdiff_t timeout)
	{
		return Read(data.data(), data.size(), timeout);
	}
	/**Read data in async mode. 

	\tparam T An object that has .data()  and .size() const for reading
	its data.
	\return A future containing the amount of bytes written.*/
	template<typename...Args>
	inline std::future<std::ptrdiff_t> AsyncRead(Args&&...args)
	{
		return std::async(std::launch::async, [&]() {
			return this->Read(std::forward<Args>(args)...);
		});
	}
protected:
	using cg::LogAdaptor<Reader>::EnableLogs;
	using cg::LogAdaptor<Reader>::LogNote;
	using cg::LogAdaptor<Reader>::LogWarn;
	using cg::LogAdaptor<Reader>::LogError;
	using cg::LogAdaptor<Reader>::Log;
	using cg::LogAdaptor<Reader>::ms_log;
	using cg::LogAdaptor<Reader>::ms_name;

};

}