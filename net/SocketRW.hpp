
#include "Socket.hpp"
#include "../Filter.hpp"

namespace cg {
namespace net {

class SocketRW : public cg::Writer, public cg::Reader
{
public:
	/**Create the socket reader writer.
	\param sock A reference to the socket to use for writing and reading.
	\param writeFilter The filter to be applied to written data.
	\param readFilter The filter ot be applied to the read data.*/
	SocketRW(Socket& sock,
		Filter* writeFilter,
		Filter* readFilter);
	/**Create the socket reader writer.
	\param sock A reference to the socket to use for writing and reading.*/
	SocketRW(Socket& sock);
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
		std::ptrdiff_t timeout) override;
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
	virtual std::ptrdiff_t Read(char * dest,
		std::size_t size,
		std::ptrdiff_t timeout) override;
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
	/**A ref to the socket.*/
	Socket& m_socket;
	/**The reading filter filter.*/
	Filter* m_readFilter;
	/**The writing filter filter.*/
	Filter* m_writeFilter;
};

}
}


