#pragma once

#include <list>
#include <mutex>

#include "../LogAdaptor.hpp"
#include "../MasterLock.hpp"

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
	public cg::LogAdaptor<Socket>
{
	/**************************************************************************ALIAS/TYPDEF/CONST*/
	/**The socket lock type.*/
	using LockType = cg::MasterLock<std::recursive_mutex, false, true>;
	 /** The data type for port parameters.*/
	using Port = uint16_t;
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
	/**Check and see if the socket has data available.
	\param timeout The amount of time to wait untill returning a false signal.
	The time units are in microseconds.  A timeout of 0 will not block at all.
	A negative timeout will block forever untill data is ready.
	\return True if at least one byte can be read without blocking.*/
	bool Ready(std::ptrdiff_t timeout = 0) const;

protected:/********************************************************************PROTECTED**********/
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


