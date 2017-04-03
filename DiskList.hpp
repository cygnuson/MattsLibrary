#pragma once

#include "FileSystem.hpp"
#include "Serial.hpp"
#include "exception.hpp"

#define _DEBUGDISKLIST _DEBUG && 1

namespace cg {

/**A list with data that exists on the disk instead of ram.
\tparam T A fundamental non pointer type or a object that implements the
Serializable class.*/
template<typename T>
class DiskList
{
public:
	using SizeType = uint64_t;
	static_assert(cg::IsSerializable<T>::value,
		"The type must be fundamental or implement cg::Serializable.");
	/**Create a disk list.
	\param vectorLike True if the list should act like a vector. If false, The
	file will expand on inserts and shifts, acting like a file instead of an
	array list.
	\param filePath The path to the file without an extension. If it exists, 
	and is valid, it will be loaded. If it does not exist, it will be created.
	*/
	DiskList(const std::string& filePath,
		bool arrayLike = true);
	/**Flush and close.*/
	~DiskList();
	/**Insert data to the list. If the list is not considered 'arraylike' then
	the file will expand to allow insertions at larger indexes. Otherwise the
	list will throw an out of bounds exception if the index is past the end of
	the list.
	\param data The data unit to insert. The data must impliment Serializable.
	\param index The index to place the data.*/
	void Insert(std::size_t index, const T& data);
	/**Push data to the back of the list.
	\param data The data to push.*/
	void PushBack(const T& data);
	/**Get the size of the list.
	\return The size of the list.*/
	std::size_t Size() const;
	/**Make sure the disk reflects the changes to the list.*/
	void Flush();
	/**Close the file of the disk.*/
	void Close();
private:
	/**True to act like a vector.*/
	bool m_arrayLike;
	/**The path of the list without an extension.*/
	std::string m_path;
	/**The file for which the data will exist.*/
	cg::File m_file;
	/**Keep track of the address of each index.*/
	std::vector<SizeType> m_indexAddress;
	/**The next place that data can be put.*/
	uint64_t m_nextAddress = 1;
	/**The endian status of the file.*/
	bool m_isLittleEndian = cg::Endian::little;
};

template<typename T>
inline DiskList<T>::DiskList(const std::string& filePath, bool arrayLike)
	:m_arrayLike(arrayLike),
	m_file(filePath + ".disk_list"),
	m_path(filePath)
{
	bool exists = cg::FileSystem::FileExists(filePath) == 1;
	if (exists)
	{

	}
	else
	{
		m_file.Touch();
	}
}

template<typename T>
inline DiskList<T>::~DiskList()
{
	Close();
}

template<typename T>
inline void DiskList<T>::Insert(std::size_t index,
	const T & data)
{
	if (index == Size())
	{
		cg::Serial s;
		s << data;
		auto av = s.GetRawArrayView();
		m_file.Write(av, m_nextAddress);
		m_indexAddress.push_back(m_nextAddress);
		m_nextAddress += cg::SizeOf(data);
	}
}

template<typename T>
inline void DiskList<T>::PushBack(const T & data)
{
	Insert(Size(), data);
}

template<typename T>
inline std::size_t DiskList<T>::Size() const
{
	return m_indexAddress.size();
}

template<typename T>
inline void DiskList<T>::Flush()
{
	cg::File indexFile(m_path + ".disk_list_index");
	indexFile.Remove();
	indexFile.Touch();
	indexFile.Write((char*) m_indexAddress.data(),
		m_indexAddress.size() * sizeof(SizeType));
	indexFile.Close();
	m_file.Flush();
}

template<typename T>
inline void DiskList<T>::Close()
{
	Flush();
	m_file.Close();
}

}

#if _DEBUGDISKLIST
template class cg::DiskList<int>;
#endif