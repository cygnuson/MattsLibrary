#pragma once

#include <string>
#include <vector>

#include "Endian.hpp"
#include "ArrayView.hpp"
#include "Writer.hpp"
#include "Reader.hpp"

namespace cg {

class Serial;

/**Interface for serializing.*/
class Serializable
{
public:
	/**Push this object to the serial.
	\param s The serial object to push to.*/
	virtual void Push(cg::Serial& s) const = 0;
	/**Pull data from the serial to this object.
	\param s The serial object to pull from.*/
	virtual void Pull(cg::Serial& s) = 0;
};
/**Helper for serializables.*/
template<typename T>
struct IsSerializable
{
	/**The dtermined value.*/
	const static bool value =
		std::is_base_of<Serializable, T>::value
		|| std::is_fundamental<T>::value;
};
/**Abstraction for size of*/
template<typename T>
std::enable_if_t<std::is_base_of<Serializable, T>::value, std::size_t>
SizeOf()
{
	return T::SizeOf;
}

/**Abstraction for size of*/
template<typename T>
std::enable_if_t<!std::is_base_of<Serializable, T>::value, std::size_t>
SizeOf()
{
	return sizeof(T);
}

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
	\param av The array view. Should include the first endian check byte.*/
	Serial(const cg::ArrayView& av);
	/**Copy data from a poitner and size. This will erase the current contents.
	Should include the first endian check byte.
	\param data The pointer to the data.
	\param size The size of the data.*/
	void Copy(const char* data, std::size_t size);
	/**Create a serial. Should include the first endian check byte.
	\param data The serial to load.
	\param size The size of the serial to load.*/
	Serial(char* data, std::size_t size);
	/**Reset the extraction postition. will be 1, to skip the endian
	indicator.*/
	void Reset();
	/**Write the serial directly to a writer.
	\param writer The writer to write to. Will work with any object that is 
	derived from cg::Writer.  The first endian check byte will be included.
	\param timeout The amount of time to wait while trying to write.  0 for
	no waiting, -1 for inf wating.
	\return The amount written. */
	std::size_t Write(cg::Writer& writer, std::ptrdiff_t timeout = 0);
	/**Read data into the serial directly.  Will clobber the current contents
	of the serial. The firt endian check byte will be expected.
	\param timeout The amount of time to wait while trying to read.  0 for
	no waiting, -1 for inf wating.
	\param size The expected size to be received. The derived class passed to
	the `reader` parameter may ignore this (eg SocketRW will ignore it).
	\param reader The reader to read from.
	\return The amount of bytes read.*/
	std::size_t Read(cg::Reader& reader,
		std::size_t size,
		std::ptrdiff_t timeout = 0);
	/**Stream operator.
	\param obj The object to stream in.
	\return A ref to this object.*/
	template<typename T>
	Serial& operator<<(const T& obj) ;
	/**Stream operator.
	\param obj The object to stream out.
	\return A ref to this object.*/
	template<typename T>
	Serial& operator>>(T& obj);
	/**Push data to the serial.
	\tparam T The type of data to push.
	\param data The data to push.*/
	template<typename T>
	std::enable_if_t<std::is_fundamental<T>::value, void>
		Push(const T& data) ;
	/**Push data to the serial.
	\tparam T The type of data to push.
	\param data The data to push.*/
	template<typename T>
	inline std::enable_if_t<std::is_base_of<Serializable,T>::value, void>
		Push(T& data) ;
	/**Push data to the serial.
	\tparam T The type of data to push.
	\param data The data to push.*/
	template<typename T>
	void Push(T* data) ;
	/**Push a string to the serial.
	\param str A string to push to the serial.*/
	void Push(const std::string& str);
	/**Get data from the stream. Will advance the pointer.
	\param out The place to put the data.
	\tparam T The type of obj to receive data.*/
	template<typename T>
	std::enable_if_t<std::is_fundamental<T>::value, void>
		Pull(T& out);
	/**Get data from the stream. Will advance the pointer.
	\param out The place to put the data.
	\tparam T The type of obj to receive data.*/
	template<typename T>
	inline std::enable_if_t<std::is_base_of<Serializable, T>::value, void>
		Pull(T& out);
	/**Get data from the stream. Will advance the pointer.
	\param out The place to put the data.
	\tparam T The type of obj to receive data.*/
	template<typename T>
	void Pull(T* out);
	/**Get string data from the serial. will advance the positon. will extract
	untill the null byte is received.
	\param out The string to receive the letters.*/
	void Pull(std::string& out);
	/**Get a char* with the serial.
	\return A char* with he data.*/
	std::pair<const char*, std::size_t> Get() const;
	/**Get an array view with the serial.
	\return An array view with the data.*/
	cg::ArrayView GetArrayView() const;
	/**Get an array view with the serial, EXCLUDING the endian byte.
	\return An array view with the data.*/
	cg::ArrayView GetRawArrayView() const;
	/**Get the size of the data currently in the serial.
	\return The amount of bytes currenty in the serial.*/
	std::size_t Size() const;
	/**Dtermine where the reading position is at.
	\return The position of the reader.*/
	std::size_t Position() const;
	/**Determine if there are any bytes left to read.
	\return The amount of bytes that can be read immediatly.*/
	std::size_t Left() const;
	/**Empty the serial vector including the endian byte.*/
	void ClearAll();
private:
	/**The data.*/
	std::vector<char> m_data;
	/**The position of the next byte to be extracted.*/
	std::size_t m_pos = 1;
	/**True for little endian system.*/
	bool m_isLittleEndian = cg::Endian::little;
};
/**A serializing interface for the filter system.*/
class SerialWriter : public cg::Writer
{
public:
	/**Create an object.
	\param serial The serial to be written to.*/
	SerialWriter(Serial& serial);
	/**Check if the object is ready to write. Will always be true for this.
	\param timeout The amount of time to wait untill the writer is ready.
	\return True always.*/
	virtual bool WriteReady(std::ptrdiff_t timeout = 0) const override;
	/**Write to the serial object.
	\param data The data to be written.
	\param size The amount of data to be written.
	\param timeout The amount of time to wait for the writer to be ready.
	\return The amount of bytes written.*/
	virtual std::ptrdiff_t Write(const char * data,
		int64_t size,
		std::ptrdiff_t timeout = 0) override;
private:
	/**A reference to the serial to be written to.*/
	cg::Serial& m_serial;
};

/**A serializing interface for the filter system.*/
class SerialReader : public cg::Reader
{
public:
	/**Create a reader object for extracting data from a serial.
	\param serial The serial to extracted from.*/
	SerialReader(Serial& serial);
	/**Determine if the serial is ready to be read from.
	\param timeout The amount of time to wait untill ready.
	\return True if there is something to be read.*/
	virtual bool ReadReady(std::ptrdiff_t timeout = 0) const override;
	/**Read data.
	\param expectedSize An option size that will be read in.
	\param expectedSize The amount of data to read.
	\param timeout The amount of time to wait for readyness.
	\return An array view with the data.*/
	virtual cg::ArrayView Read(int64_t expectedSize,
		std::ptrdiff_t timeout = 0) override;
private:
	/**A reference to the serial to access.*/
	cg::Serial& m_serial;
};

template<typename T>
inline Serial & Serial::operator<<(const T & obj)
{
	this->Push(obj);
	return *this;
}

template<typename T>
inline Serial & Serial::operator >> (T & obj)
{
	this->Pull(obj);
	return *this;
}

template<typename T>
inline std::enable_if_t<std::is_fundamental<T>::value, void>
Serial::Push(const T & data)
{
	const static std::size_t size = sizeof(data);
	for (std::size_t i = 0; i < size; ++i)
	{
		/*make a char at index i of the data*/
		char b = *(((char*)&data) + i);
		m_data.push_back(b);
	}
}
template<typename T>
inline std::enable_if_t<std::is_base_of<Serializable, T>::value, void>
Serial::Push(T & data)
{
	data.Push(*this);
}

template<typename T>
inline void Serial::Push(T * data)
{
	Push(*data);
}

template<typename T>
inline std::enable_if_t<std::is_fundamental<T>::value, void>
Serial::Pull(T & out)
{
	out = *((T*)(m_data.data() + m_pos));
	m_pos += sizeof(T);
	cg::Endian::MakeHostOrder(out, m_isLittleEndian);
}
template<typename T>
inline std::enable_if_t<std::is_base_of<Serializable, T>::value, void>
Serial::Pull(T & out)
{
	out.Pull(*this);
}

template<typename T>
inline void Serial::Pull(T * out)
{
	Pull(*out);
}

/*****************************************************************************/
template<typename T>
void Push(cg::Serial& s, const T& t)
{
	s << t;
}
template<typename T>
void Pull(cg::Serial& s, T& t)
{
	s >> t;
}

}