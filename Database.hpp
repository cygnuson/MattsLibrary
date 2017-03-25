#pragma once

#include "Memory.hpp"
#include "LogAdaptor.hpp"
#include "ArrayView.hpp"

#include <string>
#include <set>
#include <fstream>

namespace cg {


/**A record interface.*/
class IRecord 
{
public:
	/**Get the serial data of the record.
	\return An array view with the serial data.*/
	virtual cg::ArrayView Data() const = 0;
	/**Load the data from an array view into the record.
	\param data The data to load as ArrayView.*/
	virtual void Load(const cg::ArrayView& data) = 0;
private:

};

/**Create a data base for effectivly storing information on the disk. Each
table will have its own file and each table and file pair will be indexed in a
seperate index file.*/
class Database : public cg::LogAdaptor<Database>
{
public:
	/**Destroy and save.*/
	~Database();
	/**Create a database object, which will not actually create the DB on the
	disk. The place the db will exist in in dir/name.db
	\param dir The place for the data base files. Multiple files are needed and
	any files that exist without the complete set will be cobbered.
	\param name The name of the database.*/
	Database(const std::string& dir,
		const std::string& name);
	
private:
	using cg::LogAdaptor<Database>::EnableLogs;
	using cg::LogAdaptor<Database>::LogNote;
	using cg::LogAdaptor<Database>::LogWarn;
	using cg::LogAdaptor<Database>::LogError;
	using cg::LogAdaptor<Database>::Log;
	using cg::LogAdaptor<Database>::ms_log;
	using cg::LogAdaptor<Database>::ms_name;
};

}