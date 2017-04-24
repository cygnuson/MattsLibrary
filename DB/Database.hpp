#pragma once

#include <string>
#include <set>
#include <fstream>

#include "../Memory.hpp"
#include "../LogAdaptor.hpp"
#include "../Serial.hpp"
#include "../FileSystem.hpp"
#include "IRecord.hpp"

namespace cg {

/**A wrapper for the record.*/
template<typename RecordType>
class RecordAccess
{
public:
	/**Make sure RecordType is serializable.*/
	static_assert(std::is_base_of<cg::IRecord, RecordType>::value,
		"The record type is not a record.");
	/**Create a record access object.
	\param index The index of the record.
	\param rollsize The roll size of the database.
	\param The path to the database root directory.*/
	RecordAccess(uint64_t index, uint64_t rollsize, cg::Dir path);
	/**Move a RecordAccess
	\param other The other to move.*/
	RecordAccess(RecordAccess&& other);
	/**When destroyed, write the changes.*/
	~RecordAccess();
	/**Write back the data.*/
	void Write();
	/**Load the data off the disk.*/
	void Load();
	/**Get the record.
	\return A reference to the record.*/
	RecordType& Record();
	/**Get the record.
	\param rec The record to set at this position.
	\return A reference to the record.*/
	RecordType& Record(const RecordType& rec);
private:
	/**Calculate the write location of the record.*/
	std::pair<std::string, uint64_t> GetWriteLocation() const;
	/**The index number of the record to access.*/
	uint64_t m_index;
	/**The roll size of the data base.*/
	uint64_t m_rollSize;
	/**The path to the db root folder.*/
	cg::Dir m_path;
	/**The internal record that will be written back to the db when required.*/
	RecordType m_record;
	/**True to skip writing when destroyed.*/
	bool m_skipWrite = false;
};

/**Create a data base for effectivly storing information on the disk. Each
table will have its own file and each table and file pair will be indexed in a
seperate index file.*/
template<typename RecordType>
class Database : public cg::LogAdaptor<Database<RecordType>>
{
public:
	/**Make sure RecordType is serializable.*/
	static_assert(std::is_base_of<cg::IRecord, RecordType>::value,
		"The record type is not a record.");
	/**Destroy and save.*/
	~Database();
	/**Create a database object, which will not actually create the DB on the
	disk. The place the db will exist in in dir/name.db
	\param dir The place for the data base files. Multiple files are needed and
	any files that exist without the complete set will be cobbered.
	\param rollsize The amount of records per each file (roll) of records.*/
	Database(const std::string& dir,
		uint32_t rollsize);
	/**Create a roll of records.
	\param index Create enough empty rolls that this index can be accessed.*/
	bool CreateRolls(uint64_t index);
	/**Create a new empty roll on the db.
	\return True if the roll was created.*/
	bool CreateNewRoll();
	/**Get a record accessor.
	\param index The index to get.
	\return A record accessor that will write back changes on destruction or
	on demand.*/
	RecordAccess<RecordType> Get(uint64_t index);
private:
	using cg::LogAdaptor<Database<RecordType>>::EnableLogs;
	using cg::LogAdaptor<Database<RecordType>>::LogNote;
	using cg::LogAdaptor<Database<RecordType>>::LogWarn;
	using cg::LogAdaptor<Database<RecordType>>::LogError;
	using cg::LogAdaptor<Database<RecordType>>::Log;
	using cg::LogAdaptor<Database<RecordType>>::ms_log;
	using cg::LogAdaptor<Database<RecordType>>::ms_name;
	/**The amount of rolls for each record.*/
	uint32_t m_rollSize;
	/**The root directory of the database.*/
	cg::Dir m_dir;
};

template<typename RecrodType>
inline Database<RecrodType>::~Database()
{

}

template<typename RecrodType>
inline Database<RecrodType>::Database(const std::string & dir,
	uint32_t rollsize) : m_dir(dir), m_rollSize(rollsize)
{
	if (!m_dir.Exists())
		m_dir.Touch();
	EnableLogs(true, "Database");
}

template<typename RecordType>
inline bool Database<RecordType>::CreateRolls(uint64_t index)
{
	std::string filename = cg::ToString(index / m_rollSize);
	cg::File file(m_dir, filename + ".dbroll");
	while (!file.Exists())
	{
		if (!CreateNewRoll())
			return false;
	}
	return true;
}

template<typename RecordType>
inline bool Database<RecordType>::CreateNewRoll()
{
	uint64_t currentRoll = 0;
	cg::File file(m_dir, cg::ToString(currentRoll) + ".dbroll");
	while (file.Exists())
	{
		currentRoll += 1;
		file.SetPath(m_dir, cg::ToString(currentRoll) + ".dbroll");
	}
	file.Touch();
	char ch = 0;
	if (!file.Write(ch, RecordType::SizeOf*m_rollSize,0))
		return false;
	return true;
}

template<typename RecordType>
inline RecordAccess<RecordType> Database<RecordType>::Get(uint64_t index)
{
	RecordAccess<RecordType> ra(index, m_rollSize, m_dir);
	ra.Load();
	return ra;
}



/*********************************************************************************/


template<typename RecordType>
inline RecordAccess<RecordType>::RecordAccess(uint64_t index,
	uint64_t rollsize, cg::Dir path)
	:m_index(index), m_rollSize(rollsize), m_path(path)
{

}

template<typename RecordType>
inline RecordAccess<RecordType>::RecordAccess(RecordAccess && other)
{
	m_index = other.m_index;
	m_path = other.m_path;
	m_record = other.m_record;
	m_rollSize = other.m_rollSize;
	other.m_skipWrite = true;
}

template<typename RecordType>
inline RecordAccess<RecordType>::~RecordAccess()
{
	if (!m_skipWrite)
		Write();
}

template<typename RecordType>
inline void RecordAccess<RecordType>::Write()
{
	auto writeInfo = GetWriteLocation();
	cg::File file(m_path, writeInfo.first);
	cg::Serial s;
	s << m_record;
	auto av = s.GetRawArrayView();
	file.Write(av, writeInfo.second);
	file.Flush();
}

template<typename RecordType>
inline void RecordAccess<RecordType>::Load()
{
	auto writeInfo = GetWriteLocation();
	cg::File file(m_path, writeInfo.first);
	cg::ArrayView av(RecordType::SizeOf);
	if (file.Read(av, writeInfo.second))
	{
		cg::Serial s;
		cg::SerialWriter sw(s);
		sw.Write(av.data(), av.size());
		s >> m_record;
	}
	else
		throw DatabaseFileReadException();
}

template<typename RecordType>
inline RecordType & RecordAccess<RecordType>::Record()
{
	return m_record;
}

template<typename RecordType>
inline RecordType & RecordAccess<RecordType>::Record(const RecordType & rec)
{
	m_record = rec;
	return m_record;
}

template<typename RecordType>
inline std::pair<std::string, uint64_t>
RecordAccess<RecordType>::GetWriteLocation() const
{
	/*the file (roll) to write to.*/
	std::string filename = cg::ToString(m_index / m_rollSize) + ".dbroll";
	/*The place to write inside the roll file.*/
	uint64_t writeLocation
		= ((m_index % m_rollSize) * RecordType::SizeOf);
	return{ filename, writeLocation };
}

}