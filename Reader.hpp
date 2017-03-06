#pragma once

#include <future>

#include "LogAdaptor.hpp"

namespace cg {

/**An interface class to facilitate read of various locations.*/
class Reader : cg::LogAdaptor<Reader>
{
public:
	/**Determine if the object is in a state that would allow it to read.
	\return True if the object is ready to be read to without blocking.*/
	virtual bool Ready() = 0;
	/**Read some data from the object.  If Ready() returned true before this
	call, the timeout should never be reached sense this object should return
	immediatly.
	\param dest The place to put the data.
	\param size The size of the data to read.
	\param timeout time untill return if the data does not get read. If the
	implementing class does not timeout (like a file or mem write) then this
	param should have a default value and be ignored.  The timeout is in micro
	seconds.
	\return The amount of bytes read.*/
	virtual std::size_t Read(char* dest,
		std::size_t size,
		std::size_t timeout) = 0;
	/**Read data in async mode.  This will wrap a call to Read into the
	std::async function and return a future.

	\param dest The place to put the data.
	\param size The size of the data to read.
	\param timeout time untill return if the data does not get read. If the
	implementing class does not timeout (like a file or mem write) then this
	param should have a default value and be ignored.  The timeout is in micro
	seconds.
	\return A future containing the amount of bytes read.*/
	inline virtual std::future<std::size_t> AsyncRead(char* dest,
		std::size_t size,
		std::size_t timeout = 0)
	{
		return std::async(std::launch::async, [&]() {
			return this->Read(dest, size, timeout);
		});
	}
	/**Read some data. T must have a member .data()  that will return a
	pointer to a location to store data. T must also have a member .size()const
	that will return the size of the data to be stored.
	\tparam T An object that has .data()  and .size() const for storing
	data.
	\param data An object of type T that has .data()  and .size() const .
	\param timeout The time in micro seconds to wait untill returning. If the
	op does not timeout then set a default value and ignore the parameter.
	\return The amount of bytes read.*/
	template<typename T>
	inline std::size_t Read(T & data, std::size_t timeout)
	{
		return Read(data.data(), data.size(), timeout);
	}
	/**Read data in async mode.  This will wrap a call to read into the
	std::async function and return a future.T must have a member .data() 
	that will return a pointer to a location to store data. T must also have a
	member .size() const that will return the size of the data to be stored.

	\tparam T An object that has .data()  and .size() const for reading
	its data.
	\param data An object of type T that has .data()  and .size() const .
	\param timeout time untill return if the data does not get read. If the
	implementing class does not timeout (like a file or mem write) then this
	param should have a default value and be ignored.  The timeout is in micro
	seconds.
	\return A future containing the amount of bytes written.*/
	template<typename T>
	inline std::future<std::size_t> AsyncRead(T & data,
		std::size_t timeout = 0)
	{
		return std::async(std::launch::async, [&]() {
			return this->Read<T>(data, timeout);
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