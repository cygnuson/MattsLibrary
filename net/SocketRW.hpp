#pragma once
#include "Socket.hpp"
#include "../Filter.hpp"

namespace cg {
namespace net {
/**Do some socket Reading and Writing. The size of data sent is prepended 
to the send data. SocketRW::Read can only ready data sent with its size sent
first (as std::uint64_t).*/
class SocketRW : 
	public cg::Writer,
	public cg::Reader,
	private cg::NoCopy,
	public cg::LogAdaptor<SocketRW>
{
public:
	/**Create the socket reader writer.  This object will own its filters and
	they will be clened up on destruction.
	\param sock An address to the socket to use for writing and reading.
	\param writeFilter The filter to be applied to written data.
	\param readFilter The filter ot be applied to the read data.*/
	SocketRW(Socket* sock,
		Filter* writeFilter,
		Filter* readFilter);
	/**Destroy and clean up*/
	~SocketRW()
	{
		if (m_readFilter)
			delete m_readFilter;
		if (m_writeFilter)
			delete m_writeFilter;
	}
	/**Move*/
	SocketRW(SocketRW&& other);
	/**Move*/
	void operator=(SocketRW&& other);
	/**Create the socket reader writer.
	\param sock Aa address to the socket to use for writing and reading.*/
	SocketRW(Socket* sock);
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
	virtual std::ptrdiff_t Write(const char * data,
		std::size_t size,
		std::ptrdiff_t timeout = -1) override;
	/**Read some data from the object.  If Ready() returned true before this
	call, the timeout should never be reached sense this object should return
	immediatly.
	\param dest The place to put the data.
	\param size DOES NOT EFFECT! The size of the data is included in the send
	function.
	\param timeout time untill return if the data does not get read. If the
	implementing class does not timeout (like a file or mem write) then this
	param should have a default value and be ignored.  The timeout is in micro
	seconds.
	\return The amount of bytes read.*/
	virtual std::ptrdiff_t Read(char * dest,
		std::size_t size = 0,
		std::ptrdiff_t timeout = -1) override;
	/**Check and see if the socket has data available.
	\param timeout The amount of time to wait untill returning a false signal.
	The time units are in microseconds.  A timeout of 0 will not block at all.
	A negative timeout will block forever untill data is ready.
	\return True if at least one byte can be read without blocking.*/
	virtual bool ReadReady(std::ptrdiff_t timeout = 0) const;
	/**Check and see if the socket can send data without blocking.
	\param timeout The amount of time to wait untill returning a false signal.
	The time units are in microseconds.  A timeout of 0 will not block at all.
	A negative timeout will block forever untill data is ready.
	\return True if at least one byte can be written without blocking.*/
	virtual bool WriteReady(std::ptrdiff_t timeout = 0) const;
private:
	using cg::LogAdaptor<SocketRW>::EnableLogs;
	using cg::LogAdaptor<SocketRW>::LogNote;
	using cg::LogAdaptor<SocketRW>::LogWarn;
	using cg::LogAdaptor<SocketRW>::LogError;
	using cg::LogAdaptor<SocketRW>::Log;
	using cg::LogAdaptor<SocketRW>::ms_log;
	using cg::LogAdaptor<SocketRW>::ms_name;
	/**Check and err*/
	void CheckAndReport() const;
	/**A ref to the socket.*/
	Socket* m_socket;
	/**The reading filter filter.*/
	Filter* m_readFilter;
	/**The writing filter filter.*/
	Filter* m_writeFilter;
};

}
}


