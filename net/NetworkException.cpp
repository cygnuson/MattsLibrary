#include "NetworkException.hpp"

namespace cg {
namespace net {

NetworkException::NetworkException(Error errorCode)
	:m_error(errorCode)
{

}
NetworkException::NetworkException(const NetworkException & other)
	:m_error(other.m_error)
{

}
std::string NetworkException::What() const
{
	return ToString(m_error);
}
Error NetworkException::GetErrno()
{
#ifdef _WIN32
	return (Error) WSAGetLastError();
#else
	return (Error) errno;
#endif
}

std::string NetworkException::ToString(Error error)
{
	/*portability for windows fiasco*/
#ifdef _WIN32
	LPTSTR eStr = nullptr;
	auto formatResult = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		(errno_t)error,
		0,
		(LPTSTR)&eStr,
		0,
		NULL);
	if (formatResult == 0)
	{
		eStr = "Failed to get the error.";
	}
	auto str = std::string(eStr);
	LocalFree(eStr);
	str += " CODE: ";
	str += std::to_string((int)error);
	str += "(" + ErrorName(error);
	str += ")";
	return str;
#else
	/*posix from here on*/
	char err[128] = { 0 };
	strerror(error);
	auto str = std::string(err);
	str += " CODE: ";
	str += std::to_string((int)error);
	str += "(" + GetErrorName(error);
	str += ")";
	return str;
#endif
}

std::string NetworkException::ToString() const
{
	return ToString(m_error);
}

}
}

