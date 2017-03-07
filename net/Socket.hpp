#pragma once

#include <list>
#include <mutex>

#include "../LogAdaptor.hpp"
#include "../MasterLock.hpp"
#include "../Writer.hpp"
#include "../Reader.hpp"

#include "NetworkObject.hpp"
#include "NetworkException.hpp"

namespace cg {
namespace net {


/**A data set for the sockets. It is used for holding the OS specific info.  
If a reference of the socket exists anyware it should be locked.  When the
socket is locked, any lists it is inside of should also be locked.  This 
will ensure that while the reference to the socket is alive, it will stay
alive as long as intented.*/
struct Socket : cg::net::NetworkObject,
	public cg::LogAdaptor<Socket>, public cg::Writer, public cg::Reader
{
	/**************************************************************************ALIAS/TYPDEF/CONST*/
	/**The socket lock type.*/
	using LockType = cg::MasterLock<std::recursive_mutex, false, true>;
	 /** The data type for port parameters.*/
	using Port = uint16_t;
	/**Convient address*/
	using Address = std::pair<std::string, Port>;
	/** An invalid port for omparisons.*/
	const static Port InvalidPort = 0; 
	/** A bad socket ex.*/
	const static OSSocket InvalidSocket = INVALID_SOCKET; 
	/**Bad id const.*/
	const static std::size_t InvalidId = 0; 
	/**************************************************************************FUNCTIONS**********/
	/**Create the socket.
	
	Create the socket in memory only. It will not be created in the 
	OS socket subsystem.*/
	Socket();
	/**Destruct the socket. Will warn if not closed.*/
	~Socket();
	/**Create the socket and lock it immediatly.
	\param locked True to create a socket locked.*/
	Socket(bool locked);
	/**Move constructor so that sockets can be moved into lists (cant be copy)
	
	Move a socket to a new location. The old location will be invalid.
	Both sockets must be lockable by the calling thread in order to make the
	move.  Remember that when a socket is moved, it gets a new lock and its old
	lock is no longer valid.
	\param other The other socket to move.*/
	Socket(Socket&& other);
	/**Get the sockets ID.
	
	Get the ID of the socket. Ids are used for identifying the socket so
	that it can be passed around without maintaining the lock.  This function
	may be called without locking the socket.
	\return The ID of the socket.*/
	std::size_t Id()const;
	/**Compare this socket to an OSSocket.

	Compare this socket to an OSSocket. This function may be called without 
	locking the the socket.
	\param other The other OSSocket to compare to.
	\return True if the sockets are the same.*/
	bool operator==(const OSSocket& other)const;
	/**Compare this socket to another Socket.

	Because sockets are unique and non-copyable this function will only return
	true for a socket compared to itself.  Therefore the function will only 
	return true if both this and &other are the same address.
	\param other The other Socket to compare to.
	\return True if the sockets are the same.*/
	bool operator==(const Socket& other) const;
	/**Lock the socket manually.*/
	void Lock() const;
	/**UnLock the socket manually.*/
	void UnLock() const;
	/**Determine if the object is in a state that would allow it to send.
	\return True if the object is ready to be written to without blocking.*/
	virtual bool Ready() override;
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
	virtual std::size_t Write(const char * data,
		std::size_t size,
		std::size_t timeout) override;
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
	virtual std::size_t Read(char * dest,
		std::size_t size,
		std::size_t timeout) override;
	/**Check and see if the socket has data available.
	\param timeout The amount of time to wait untill returning a false signal.
	The time units are in microseconds.  A timeout of 0 will not block at all.
	A negative timeout will block forever untill data is ready.
	\return True if at least one byte can be read without blocking.*/
	bool Ready(std::ptrdiff_t timeout = 0) const;
	/**Get the name of a connected socket. Usually as an address.

	\param socket The socket for which to get the name.
	\param addr A refernce to a sockaddr_storage object that will be the
	location of the data once it has been received.  It must be accessed
	after the fact casted as such (sockaddr_in*) or (sockaddr_in6*).
	\throws cg::net::NetworkException A network exception is thrown if the
	underlying socket infrastructure throws an exception.
	\sa cg::net::NetworkException*/
	void GetPeerName(sockaddr_storage & addr) const;
	/**Get the address of a connected socket.

	Get the address of a socket that is connected. If the socket has any
	problems an exception wil be thrown.
	\return An Address object, which is just a std::pair object with first
	set to be the address and the second set to be the port. If for any
	reson the functions fails or an exception is thrown, the Port will be
	const Socket::InvalidPort and the address will be an empty string.
	\throws cg::net::NetworkException A network exception is thrown if the
	underlying socket infrastructure throws an exception.
	\sa cg::net::NetworkException*/
	Address GetAddress();
	/**Get an address from a pre-filled out sockaddr_storage.

	\param info The sockaddr_storage object that was filled out by some other
	function.
	\return The Address object with the relevent info.*/
	static Address GetAddress(sockaddr_storage info);
protected:/********************************************************************PROTECTED**********/
	using cg::LogAdaptor<Socket>::EnableLogs;
	using cg::LogAdaptor<Socket>::LogNote;
	using cg::LogAdaptor<Socket>::LogWarn;
	using cg::LogAdaptor<Socket>::LogError;
	using cg::LogAdaptor<Socket>::Log;
	using cg::LogAdaptor<Socket>::ms_log;
	using cg::LogAdaptor<Socket>::ms_name;
	/**Allow this to be used in place of regular sockets.
	
	Allow this to be used in place of regular sockets.  Dont forget to lock
	it up properly when using it. The lock will be locked for a moment right
	before the socket is returned.  This ensures that only one thread can
	operate on the socket at a time.  If one thread has it locked from 
	outside, this will hang untill that thread releases it.
	\return The os-level socket for use with the os functions.*/
	operator OSSocket ()const;
	/**Allow the fast assigment of the OSSocket.
	
	\param osock The OSSocket number that will be assigned to this socket.*/
	void operator=(const OSSocket& osock);


	/** The os socket. Dont copy*/
	cg::net::OSSocket m_socket = InvalidSocket; 
	/**Lock the socket before doing anything with it.*/
	mutable LockType m_lock;
	/** The sockets ID.*/
	std::size_t m_id;
	/**Keep track of IDs*/
	static std::size_t ms_idCounter; 

};

/**Type safe socket for listening and accepting (and anything is Socket too).
\sa cg::net::Socket*/
struct ServerSocket : public Socket 
{
	/**Default ctor jus calls Sockets ctor.*/
	ServerSocket();
	/**Create the socket and lock it immediatly.
	\param locked True to create a socket locked.*/
	ServerSocket(bool locked);
	/**move ctor jus calls Sockets move ctor.
	\param other The other ServerSocket.*/
	ServerSocket(ServerSocket&& other);
	/**Destruct the socket. Will warn if not closed.*/
	~ServerSocket();
};


}
}


