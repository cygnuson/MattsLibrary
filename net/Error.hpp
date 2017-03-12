#pragma once

#include "../OSInclude.hpp"

#include <string>

namespace cg {
namespace net {

#ifdef _WIN32
using OSSocket = SOCKET;
#else
#define INVALID_SOCKET -1
using OSSocket = int;
#endif // _WIN32

enum Error
{
#ifdef _WIN32
	Unknown,
	/**Likly the issue is that the server is not listening or on the wrong
	port.*/
	ConnectionRefused = WSAECONNREFUSED,
	ConnectionReset = WSAECONNRESET,
	TimedOut = WSAETIMEDOUT,
	ConnectionAborted = WSAECONNABORTED,
	AddressInUse = WSAEADDRINUSE,
	/**Likly that the issue is that the socket was not created.*/
	NotASocket = WSAENOTSOCK,
	Denied = WSAEACCES,
	AddressFamNotSupported = WSAEAFNOSUPPORT,
	/**Might be a bad address string.*/
	Invalid = WSAEINVAL,
	MaxFilesOpen = WSAEMFILE,
	NoBuffer = WSAENOBUFS,
	NoMemory = ENOMEM,
	ProtNotSupported = WSAEPROTONOSUPPORT,
	BadAddressPointer = WSAEFAULT,
	BadSocket = WSAEBADF,
	WouldBlock = WSAEWOULDBLOCK,
	Again = EAGAIN,
	WSANotInit = WSANOTINITIALISED,
	NetSystemDown = WSAENETDOWN,
	OperationInProgress = WSAEINPROGRESS,
	NotConnected = WSAENOTCONN,
#else 
	Unknown,
	/**Likly the issue is that the server is not listening or on the wrong
	port.*/
	ConnectionRefused = ECONNREFUSED,
	ConnectionReset = ECONNRESET,
	TimedOut = ETIMEDOUT,
	ConnectionAborted = ECONNABORTED,
	AddressInUse = EADDRINUSE,
	/**Likly that the issue is that the socket was not created.*/
	NotASocket = ENOTSOCK,
	Denied = EACCES,
	AddressFamNotSupported = EAFNOSUPPORT,
	/**Might be a bad address string.*/
	Invalid = EINVAL,
	MaxFilesOpen = EMFILE,
	NoBuffer = ENOBUFS,
	NoMemory = ENOMEM,
	ProtNotSupported = EPROTONOSUPPORT,
	BadAddressPointer = EFAULT,
	BadSocket = EBADF,
	WouldBlock = EWOULDBLOCK,
	Again = EAGAIN,
	NetSystemDown = ENETDOWN,
	OperationInProgress = EINPROGRESS,
	NotConnected = ENOTCONN,
#endif
};


/**Translate a network error code to it Error::* name.
\param code THe code to translate.*/
inline std::string ErrorName(Error code)
{
	switch (code)
	{
	case cg::net::ConnectionRefused:
		return "Error::ConnectionRefused";
	case cg::net::ConnectionReset:
		return "Error::ConnectionReset";
	case cg::net::TimedOut:
		return "Error::TimedOut";
	case cg::net::ConnectionAborted:
		return "Error::ConnectionAborted";
	case cg::net::AddressInUse:
		return "Error::AddressInUse";
	case cg::net::NotASocket:
		return "Error::NotASocket";
	case cg::net::Denied:
		return "Error::Denied";
	case cg::net::AddressFamNotSupported:
		return "Error::AddressFamNotSupported";
	case cg::net::Invalid:
		return "Error::Invalid";
	case cg::net::MaxFilesOpen:
		return "Error::MaxFilesOpen";
	case cg::net::NoBuffer:
		return "Error::NoBuffer";
	case cg::net::NoMemory:
		return "Error::NoMemory";
	case cg::net::ProtNotSupported:
		return "Error::ProtNotSupported";
	case cg::net::BadAddressPointer:
		return "Error::BadAddressPointer";
	case cg::net::BadSocket:
		return "Error::BadSocket";
	case cg::net::WouldBlock:
		return "Error::WouldBlock";
	case cg::net::Again:
		return "Error::Again";
	case cg::net::WSANotInit:
		return "Error::WSANotInit";
	case cg::net::NetSystemDown:
		return "Error::NetSystemDown";
	case cg::net::OperationInProgress:
		return "Error::OperationInProgress";
	case cg::net::NotConnected:
		return "Error::NotConnected";
	default:
		return "Error::Unknown";
	}
}

}
}