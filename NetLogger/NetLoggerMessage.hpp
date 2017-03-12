#pragma once

#include "../Serial.hpp"
#include "../ArrayView.hpp"
#include "../Logger.hpp"

namespace cg {

class NetLoggerMessage
{
public:
	/**Create an empty message.*/
	NetLoggerMessage() {};
	/**Create a message from an array view.
	\param av The array view.*/
	NetLoggerMessage(const cg::ArrayView& av);
	/**Serialize the message.
	\return Serialized data as an ArrayView.*/
	cg::ArrayView Serialize() const;
	/**Deserilize the message.
	\param data The arrayview to deserialize.*/
	void Deserialize(const cg::ArrayView& av);
	/**The text involved in the message.*/
	std::string m_text;
	/**A debug level code.*/
	cg::Logger::Level m_level;
};

}