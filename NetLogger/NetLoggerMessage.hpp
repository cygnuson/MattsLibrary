#pragma once

#include "../Serial.hpp"
#include "../ArrayView.hpp"
#include "../Logger.hpp"

namespace cg {

class NetLoggerMessage
{
public:
	/**The default net logger port.*/
	const static uint16_t DefaultPort = 33111;
	/**Create an empty message.*/
	NetLoggerMessage() {};
	/**Create a message from an array view.
	\param av The array view.*/
	NetLoggerMessage(const cg::ArrayView& av);
	/**Serialize the message.
	\return Serialized data as an ArrayView.*/
	cg::ArrayView Serialize() const;
	/**Deserilize the message.
	\param av The arrayview to deserialize.*/
	void Deserialize(const cg::ArrayView& av);
	/**The text involved in the message.*/
	std::string m_text;
	/**A debug level code.*/
	cg::Logger::Level m_level;
	/**The thread ID of the caller.*/
	std::string m_threadId;
	/**the time of the log entry.*/
	std::string m_time;
	/**The name of the client sending the message.*/
	std::string m_name;
};

}