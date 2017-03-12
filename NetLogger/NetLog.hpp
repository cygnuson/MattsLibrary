#pragma once

#include "NetLoggerMessage.hpp"
#include "../net/SocketRW.hpp"

namespace cg {
/**Ment to be used as a static.*/
class NetLog
{
public:
	/**Initialize the logger.
	\param name This clients name.
	\param port The port of the server.*/
	static void Init(const std::string& name,
		uint16_t port);
	/**Send a log message to the server.
	\param notelevel The level of note to log.
	\param args any params that will be sent straight to the logger.*/
	template<typename...Args>
	static void LogNote(int notelevel, Args&&...args);
	/**Send a log message to the server.
	\param args any params that will be sent straight to the logger.*/
	template<typename...Args>
	static void LogWarn(Args&&...args);
	/**Send a log message to the server.
	\param args any params that will be sent straight to the logger.*/
	template<typename...Args>
	static void LogError(Args&&...args);
	/**Write a message to the server.
	\param msg The message to write.*/
	static void WriteMessage(const cg::NetLoggerMessage& msg);
private:
	/**The client socket that will be used to connect.*/
	static cg::net::Socket* ms_client;
	/**True if init has been done.*/
	static bool ms_isInit;
	/**The port to use for the logger.*/
	static uint16_t ms_port;
	/**The name of this machine.*/
	static std::string ms_name;
	/**The temporary stream for captureing logs.*/
	static std::stringstream ms_logStream;
};

template<typename ...Args>
inline void NetLog::LogNote(int noteLevel, Args && ...args)
{
	auto oldLog = cg::Logger::ActiveLogName();
	cg::Logger::ChangeLog("NETLOGGERTEMP");
	ms_logStream << "[" << ms_name << "]";
	cg::Logger::LogNote(noteLevel, std::forward<Args>(args)...);
	cg::Logger::ChangeLog(oldLog);
	NetLoggerMessage msg;
	if (noteLevel == 2)
		msg.m_level = cg::Logger::Level::e_Note2;
	else if (noteLevel == 3)
		msg.m_level = cg::Logger::Level::e_Note3;
	else
		msg.m_level = cg::Logger::Level::e_Note1;
	msg.m_text = ms_logStream.str();
	ms_logStream.str("");
	WriteMessage(msg);
}

template<typename ...Args>
inline void NetLog::LogWarn(Args && ...args)
{
	auto oldLog = cg::Logger::ActiveLogName();
	cg::Logger::ChangeLog("NETLOGGERTEMP");
	ms_logStream << "[" << ms_name << "]";
	cg::Logger::LogWarn(std::forward<Args>(args)...);
	cg::Logger::ChangeLog(oldLog);
	NetLoggerMessage msg;
	msg.m_level = cg::Logger::Level::e_Warning;
	msg.m_text = ms_logStream.str();
	ms_logStream.str("");
	WriteMessage(msg);
}

template<typename ...Args>
inline void NetLog::LogError(Args && ...args)
{
	auto oldLog = cg::Logger::ActiveLogName();
	cg::Logger::ChangeLog("NETLOGGERTEMP");
	ms_logStream << "[" << ms_name << "]";
	cg::Logger::LogError(std::forward<Args>(args)...);
	cg::Logger::ChangeLog(oldLog);
	NetLoggerMessage msg;
	msg.m_level = cg::Logger::Level::e_Error;
	msg.m_text = ms_logStream.str();
	ms_logStream.str("");
	WriteMessage(msg);
}

}