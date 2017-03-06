#pragma once

#include <future>

#include "LogAdaptor.hpp"

namespace cg {

/**An interface class to facilitate writing to various locations.*/
class Writer : cg::LogAdaptor<Writer>
{
public:
	/**Determine if the object is in a state that would allow it to send.
	\return True if the object is ready to be written to without blocking.*/
	virtual bool Ready() = 0;
	/**Write some data to the object.  If Ready() returned true before this
	call, the timeout should never be reached sense this object should return
	immediatly.
	\param data A pointer to the data to write.
	\param size The size of the data to write.
	\param timeout time untill return if the data does not get sent. If the
	implementing class does not timeout (like a file or mem write) then this
	param should have a default value and be ignored.  The timeout is in micro
	seconds.
	\return The amount of bytes written.*/
	virtual std::size_t Write(const char* data,
		std::size_t size,
		std::size_t timeout) = 0;
	/**Write data in async mode.  This will wrap a call to Write into the
	std::async function and return a future.

	\param data A pointer to the data to write.
	\param size The size of the data to write.
	\param timeout time untill return if the data does not get sent. If the
	implementing class does not timeout (like a file or mem write) then this
	param should have a default value and be ignored.  The timeout is in micro
	seconds.
	\return A future containing the amount of bytes written.*/
	inline std::future<std::size_t> AsyncWrite(const char* data,
		std::size_t size,
		std::size_t timeout = 0)
	{
		return std::async(std::launch::async, [&]() {
			return this->Write(data,size, timeout);
		});
	}
	/**Write some data. T must have a member .data() const that will return a 
	pointer to a location to read from. T must also have a member .size() const 
	that will return the size of the data to be read.
	\tparam T An object that has .data() const and .size() const for reading 
	its data.
	\param data An object of type T that has .data() const and .size() const .
	\param timeout The time in micro seconds to wait untill returning. If the 
	op does not timeout then set a default value and ignore the parameter.
	\return The amount of bytes written.*/
	template<typename T>
	inline std::size_t Write(const T & data, std::size_t timeout)
	{
		return Write(data.data(), data.size(), timeout);
	}
	/**Write data in async mode.  This will wrap a call to Write into the
	std::async function and return a future.T must have a member .data() const 
	that will return a pointer to a location to read from. T must also have a 
	member .size() const that will return the size of the data to be read.

	\tparam T An object that has .data() const and .size() const for reading
	its data.
	\param data An object of type T that has .data() const and .size() const .
	\param timeout time untill return if the data does not get sent. If the
	implementing class does not timeout (like a file or mem write) then this
	param should have a default value and be ignored.  The timeout is in micro
	seconds.
	\return A future containing the amount of bytes written.*/
	template<typename T>
	inline std::future<std::size_t> AsyncWrite(const T & data,
		std::size_t timeout = 0)
	{
		return std::async(std::launch::async, [&]() {
			return this->Write<T>(data, timeout);
		});
	}
protected:
	using cg::LogAdaptor<Writer>::EnableLogs;
	using cg::LogAdaptor<Writer>::LogNote;
	using cg::LogAdaptor<Writer>::LogWarn;
	using cg::LogAdaptor<Writer>::LogError;
	using cg::LogAdaptor<Writer>::Log;
	using cg::LogAdaptor<Writer>::ms_log;
	using cg::LogAdaptor<Writer>::ms_name;
};


}