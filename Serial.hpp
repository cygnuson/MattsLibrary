#pragma once

#include <string>
#include <vector>

#include "Endian.hpp"
#include "ArrayView.hpp"

namespace cg {

/**A class that will accumulate any data for sending.*/
class Serial
{
public:
	/**The code for little endian.*/
	const static char LittleEndian = 1;
	/**The code for big endian*/
	const static char BigEndian = 0;
	/**Create a serial.*/
	Serial();
	/**Create a serial from an arrayview.
	\param av The array view.*/
	Serial(const cg::ArrayView& av);
	/**Copy data from a poitner and size. This will erase the current contents.
	\param data The pointer to the data.
	\param size The size of the data.*/
	void Copy(const char* data, std::size_t size);
	/**Create a serial.
	\param data The serial to load.
	\param size The size of the serial to load.*/
	Serial(char* data, std::size_t size);
	/**Reset the extraction postition. will be 1, to skip the endian
	indicator.*/
	void Reset();
	/**Push data to the serial.
	\tparam T The type of data to push.
	\param data The data to push.*/
	template<typename T>
	void Push(const T& data);
	/**Push data to the serial.
	\tparam T The type of data to push.
	\param data The data to push.*/
	template<typename T>
	void Push(T* data);
	/**Push a string to the serial.
	\param str A string to push to the serial.*/
	void Push(const std::string& str);
	/**Get data from the stream. Will advance the pointer.
	\param out The place to put the data.
	\tparam T The type of obj to receive data.*/
	template<typename T>
	void Pull(T& out);
	/**Get data from the stream. Will advance the pointer.
	\param out The place to put the data.
	\tparam T The type of obj to receive data.*/
	template<typename T>
	void Pull(T* out);
	/**Get string data from the serial. will advance the positon. will extract
	untill the null byte is received.
	\param str The string to receive the letters.*/
	void Pull(std::string& out);
	/**Get a char* with the serial.
	\return A char* with he data.*/
	std::pair<const char*, std::size_t> Get() const;
	/**Get a char* with the serial.
	\return An array view with the data.*/
	cg::ArrayView GetArrayView() const;
private:
	/**The data.*/
	std::vector<char> m_data;
	/**The position of the next byte to be extracted.*/
	std::size_t m_pos = 1;
	/**True for little endian system.*/
	bool m_isLittleEndian = cg::Endian::little;
};

template<typename T>
inline void Serial::Push(const T & data)
{
	Reset();
	const static std::size_t size = sizeof(data);
	for (std::size_t i = 0; i < size; ++i)
	{
		/*make a char at index i of the data*/
		char b = *(((char*)&data) + i);
		m_data.push_back(b);
	}
}

template<typename T>
inline void Serial::Push(T * data)
{
	Push(*data);
}

template<typename T>
inline void Serial::Pull(T & out)
{
	out = *((T*)(m_data.data() + m_pos));
	m_pos += sizeof(T);
	cg::Endian::MakeHostOrder(out, m_isLittleEndian);
}

template<typename T>
inline void Serial::Pull(T * out)
{
	Pull(*out);
}

}