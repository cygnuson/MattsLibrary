#pragma once

#include "Memory.hpp"

#include <string>

namespace cg {

/**Create a data base for effectivly storing information on the disk.*/
class Database
{
public:
	/**Create a database object, which will not actually create the DB on the
	disk.
	\param dir The place for the data base files. Multiple files are needed and
	any files that exist without the complete set will be cobbered.*/
	Database(std::string dir = "");
private:
	/**The name of the file being tracked by the data base.*/
	std::string m_dir;
};

}