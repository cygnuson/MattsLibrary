
#include "Serial.hpp"

namespace cg {

Serial::Serial()
{
	if (m_isLittleEndian)
		m_data.push_back(LittleEndian);
	else
		m_data.push_back(BigEndian);
}

Serial::Serial(const cg::ArrayView & av)
{
	Copy(av.data(), av.size());
}

void Serial::Copy(const char * data, std::size_t size)
{
	m_data.insert(m_data.begin(), data, data + size);
	m_isLittleEndian = (*m_data.begin()) == LittleEndian;
}

Serial::Serial(char * data, std::size_t size)
{
	Copy(data, size);
}

void Serial::Reset()
{
	m_pos = 1;
}

void Serial::Push(const std::string & str)
{
	Reset();
	m_data.insert(m_data.end(), str.begin(), str.end());
	/*make sure to add the zero*/
	m_data.push_back(0);
}

void Serial::Pull(std::string & out)
{
	out = std::string(m_data.data() + m_pos);
	m_pos += out.size() + 1;
}

std::pair<const char*, std::size_t> Serial::Get() const
{
	return{ m_data.data(), m_data.size() };
}

cg::ArrayView Serial::GetArrayView() const
{
	return cg::ArrayView::Copy(m_data.data(), m_data.size());
}

}