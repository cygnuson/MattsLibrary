#pragma once

#include "NetLoggerMessage.hpp"
#include "../net/SocketRW.hpp"

namespace cg {
/**Ment to be used as a static.*/
class NetLog
{
public:
	/**Signal the close of the net log.*/
	static void Stop();
	/**Initialize the logger.
	\param name This clients name.
	\param port The port of the server.*/
	static void Init(const std::string& name,
		uint16_t port = NetLoggerMessage::DefaultPort);
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
};

template<typename ...Args>
inline void NetLog::LogNote(int noteLevel, Args && ...args)
{
	if (!ms_client)
	{
		cg::Logger::LogNote(noteLevel, std::forward<Args>(args)...);
		return;
	}
	NetLoggerMessage msg;
	auto lv = cg::Logger::Level::e_None;
	if (noteLevel == 1)
	{
		lv = cg::Logger::Level::e_Note1;
	}
	if (noteLevel == 2)
	{
		lv = cg::Logger::Level::e_Note2;
	}
	if (noteLevel == 3)
	{
		lv = cg::Logger::Level::e_Note3;
	}
	msg.m_text = cg::StringTogether(std::forward<Args>(args)...);
	msg.m_level = lv;
	msg.m_name = ms_name;
	msg.m_threadId = cg::ToString(std::this_thread::get_id());
	msg.m_time = cg::Logger::CurrentTime();
	WriteMessage(msg);
}

template<typename ...Args>
inline void NetLog::LogWarn(Args && ...args)
{
	if (!ms_client)
	{
		cg::Logger::LogWarn(std::forward<Args>(args)...);
		return;
	}
	NetLoggerMessage msg;
	msg.m_text = cg::StringTogether(std::forward<Args>(args)...);
	msg.m_level = cg::Logger::Level::e_Warning;
	msg.m_name = ms_name;
	msg.m_threadId = cg::ToString(std::this_thread::get_id());
	msg.m_time = cg::Logger::CurrentTime();
	WriteMessage(msg);
}

template<typename ...Args>
inline void NetLog::LogError(Args && ...args)
{
	if (!ms_client)
	{
		cg::Logger::LogError(std::forward<Args>(args)...);
		return;
	}
	NetLoggerMessage msg;
	msg.m_text = cg::StringTogether(std::forward<Args>(args)...);
	msg.m_level = cg::Logger::Level::e_Error;
	msg.m_name = ms_name;
	msg.m_threadId = cg::ToString(std::this_thread::get_id());
	msg.m_time = cg::Logger::CurrentTime();
	WriteMessage(msg);
}

}