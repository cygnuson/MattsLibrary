#include "NetLoggerMessage.hpp"

namespace cg {




NetLoggerMessage::NetLoggerMessage(const cg::ArrayView & av)
{
	Deserialize(av);
}

cg::ArrayView NetLoggerMessage::Serialize() const
{
	std::size_t predictedSize = m_text.size() + 1;
	predictedSize += sizeof(m_level);
	cg::ArrayView ret(predictedSize);
	cg::Serial serial;
	serial.Push(m_text);
	serial.Push(m_level);
	serial.Push(m_threadId);
	serial.Push(m_time);
	serial.Push(m_name);
	return serial.GetArrayView();
}

void NetLoggerMessage::Deserialize(const cg::ArrayView & av)
{
	cg::Serial serial(av);
	serial.Pull(m_text);
	serial.Pull(m_level);
	serial.Pull(m_threadId);
	serial.Pull(m_time);
	serial.Pull(m_name);
}

}



