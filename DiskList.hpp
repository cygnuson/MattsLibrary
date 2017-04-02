#pragma once

#include "FileSystem.hpp"
#include "exception.hpp"

namespace cg {

/**A list with data that exists on the disk instead of ram.
\tparam T A fundamental non pointer type or a object that implements the
Serializable class.*/
template<typename T>
class DiskList
{
public:
	/**Create a disk list.
	\param vectorLike True if the list should act like a vector. If false, The
	file will expand on inserts and shifts, acting like a file instead of an 
	array list.
	*/
	DiskList(bool arrayLike = true)
		:m_arrayLike(arrayLike) {};
	/**Insert data to the list. If the list is not considered 'arraylike' then
	the file will expand to allow insertions at larger indexes. Otherwise the 
	list will throw an out of bounds exception if the index is past the end of
	the list.
	\param data The data unit to insert. The data must impliment Serializable.
	\param index The index to place the data.*/
	void Insert(std::size_t index, const T& data);
	/**Get the size of the list.
	\return The size of the list.*/
	std::size_t Size() const;
private:
	/**True to act like a vector.*/
	bool m_arrayLike;
	/**The file for which the data will exist.*/
	cg::File m_file;

};

template<typename T>
inline void DiskList<T>::Insert(std::size_t index, const T & data)
{

}

template<typename T>
inline std::size_t DiskList<T>::Size() const
{
	return m_file.Size();
}

}