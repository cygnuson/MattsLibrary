
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

std::size_t Serial::Write(cg::Writer & writer, std::ptrdiff_t timeout)
{
	if (writer.WriteReady(timeout))
		return (std::size_t) writer.Write(m_data);
	else
		return 0;
}

std::size_t Serial::Read(cg::Reader & reader,
	std::size_t size,
	std::ptrdiff_t timeout)
{
	if (reader.ReadReady(timeout))
	{
		auto av = reader.Read(size);
		auto avSize = av.size();
		this->m_data.clear();
		this->m_data.insert(m_data.begin(), av.data(), av.data() + avSize);
		return avSize;
	}
	return 0;
}

void Serial::Push(const std::string & str)
{
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
cg::ArrayView Serial::GetRawArrayView() const
{
	return cg::ArrayView::Copy(m_data.data()+1, m_data.size()-1);
}
std::size_t Serial::Size() const
{
	return m_data.size()-1;
}
std::size_t Serial::Position() const
{
	return m_pos-1;
}
std::size_t Serial::Left() const
{
	return Size() - Position();
}
void Serial::ClearAll()
{
	m_data.swap(std::vector<char>());
}
/*****************************************************************************/


SerialWriter::SerialWriter(Serial & serial)
	:m_serial(serial)
{

}
bool SerialWriter::WriteReady(std::ptrdiff_t timeout) const
{
	return true;
}

std::ptrdiff_t SerialWriter::Write(const char * data, 
	int64_t size, 
	std::ptrdiff_t timeout)
{
	for (int64_t i = 0; i < size; ++i)
	{
		m_serial << *data;
		++data;
	}
	return size;
}

SerialReader::SerialReader(Serial & serial)
	:m_serial(serial)
{

}

bool SerialReader::ReadReady(std::ptrdiff_t timeout) const
{
	return m_serial.Left() > 0;
}

cg::ArrayView SerialReader::Read(int64_t expectedSize,
	std::ptrdiff_t timeout)
{
	if (!ReadReady(timeout))
		return cg::ArrayView();
	cg::ArrayView av(expectedSize);
	auto p = av.data();
	for (int64_t i = 0; i < expectedSize; ++i)
	{
		m_serial.Pull(*p);
		++p;
	}
	return av;
}

}