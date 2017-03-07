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

enum class Shutdown
{
	Read,
	Write,
	Both
};

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
	/**Unlock the socket manually.*/
	void Unlock() const;
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
	/**Receive generic data from the socket.

	Try to receive any data from the socket.  If data cannot be recieved for
	any reason or an exception is thrown, data is unaltered.
	\param data A char * to the location to store the data.
	\param size The size of the memory store.
	\param block Pass true to block the calling thread untill something can
	be read and checked. If false, the function will return immediatly whether
	or not any data was read.
	\param flags Flags to pass to the send function. usually 0.
	\return The amount of bytes received. 0 if no data was received, but the 
	socket is open still, or < 0 if the socket was closed properly.
	\throws cg::net::NetworkException A network exception is thrown if the
	underlying socket infrastructure throws an exception.*/
	std::size_t Recv(char* data,
		std::size_t size,
		bool block,
		socklen_t flags = 0);
	/**Send data to a connected socket.

	Send data to a connected socket. If the socket does not have a valid
	connection, then the function will throw cg::net::NetworkException.
	\param socket A reference to a socket to send the data to.
	\param data A pointer to the data to send.
	\param size The size of the data to send.
	\param block True to black untill all the data is sent.
	\param flags The flags to be passed to the send function.
	\return The amount of bytes sent. -1 if the socket is closed, or zero if
	nothing was sent but is still open.
	\throws cg::net::NetworkException A network exception is thrown if the
	underlying socket infrastructure throws an exception.*/
	std::size_t Send(const char* data,
		std::size_t size,
		bool block,
		socklen_t flags = 0);
	/**Shut down a socket.

	Shut down a socket.  Use the options cg::net::Shutdown enum to detemrine
	how to shut it down.
	\param how The type of shut down as a cg::net::Shutdown enum.
	\sa cg::net::Shutdown
	\throws NetworkEexception Throws the error code from the OS socket.*/
	void Shutdown(const Socket& socket,
		cg::net::Shutdown how);
	/**Close a socket gracefully.

	\throws NetworkException Throws the error code from the OS sockets.*/
	void Close();
	/**Bind the socket to a port.

	\param port The port that the socket will bind to. Must not be binded to
	by a different socket already.
	\param ip6 True to configure for ip6.
	\throws NetworkException Throws the error code set by the OS sockets.*/
	void Bind(bool ip6, Port port);
	/**Connect a socket to a remote address.

	\param addy The address to have in the struct.
	\param port The port to be in the struct.
	\return True if the socket is open.
	\throws NetworkException Throws the error code used by the OS sockets.*/
	bool Connect(const std::string& addy,
		Port port);
	/**Have a socket listen for incomming connections.

	Both the listener socket and the connections socket will lock when this is
	called. If another thread own either one, it will wait.
	\param backlog The amount of connections that can be pending before acceot
	has to be called.
	\throws NetworkException Throws the code that is set by the OS sockets.*/
	void Listen(socklen_t backlog = 100);
	/**Try to accept a connection.

	Try to accept a connection.
	\param socket The socket that will be the new connection if successfull.
	\param block True to block untill a connection is accepted.
	\return True if a socket was accepted.
	\throws NetworkException Throws the code that the OS sockets set on error.
	*/
	bool Accept(Socket& socket,
		bool block = true);
	/**Determine the status of a socket.
	
	\return True if the socket is open.*/
	bool IsOpen();
protected:/********************************************************************PROTECTED**********/
	using cg::LogAdaptor<Socket>::EnableLogs;
	using cg::LogAdaptor<Socket>::LogNote;
	using cg::LogAdaptor<Socket>::LogWarn;
	using cg::LogAdaptor<Socket>::LogError;
	using cg::LogAdaptor<Socket>::Log;
	using cg::LogAdaptor<Socket>::ms_log;
	using cg::LogAdaptor<Socket>::ms_name;
	/**Create an address info object for use with the other static functions
	here.

	NOTE: MakeAddress will allocate data use freeaddrinfo(returnedPtr); to
	clean it up.
	\param address The address to create for.
	\param port The port to be included.
	\param portOnly True to ignore the address, and only make a port.
	\return A pointer to an addrinfo object that contains the address made.
	\throws NetworkException Throws the code that the OS sockets set on error.
	*/
	addrinfo* MakeAddress(std::string&& address,
		Port port,
		bool portOnly = false);
	/**Allow this to be used in place of regular sockets.
	
	Allow this to be used in place of regular sockets.  Dont forget to lock
	it up properly when using it. The lock will be locked for a moment right
	before the socket is returned.  This ensures that only one thread can
	operate on the socket at a time.  If one thread has it locked from 
	outside, this will hang untill that thread releases it.
	\return The os-level socket for use with the os functions.*/
	operator OSSocket ()const;
	/**Rgister the socket with the operating system.

	Register the socket with the operating system so that it can be used for
	communication.
	\param stayLocked True to stay locked after created.
	\throws NetworkException Will throw with any socket system errors that may
	arise with the socket creation process.*/
	void Create(bool useIp6, bool stayLocked);
	/**Set a socket to be in a certian blocking mode.

	This is mainly for getting the linux sockets into non block mode.  Win
	sockets are not designed to be blocking but they can be if required.
	\param socket The socket to set.
	\param block True to set the socket to blocking mode. Flase to set it to
	non-blocking mode.*/
	void SetBlockMode(bool block);
	/**Allow the fast assigment of the OSSocket.
	
	\param osock The OSSocket number that will be assigned to this socket.*/
	void operator=(const OSSocket& osock);


	/** The os socket. Dont copy*/
	cg::net::OSSocket m_socket = InvalidSocket; 
	/**Lock the socket before doing anything with it.*/
	mutable LockType m_lock;
	/** The sockets ID.*/
	std::size_t m_id;
	/**True if created with ip6*/
	bool m_useIp6 = false;
	/**Keep track of IDs*/
	static std::size_t ms_idCounter; 

};

}
}


