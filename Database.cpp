#include "Database.hpp"

namespace cg {
Database::~Database()
{
}
Database::Database(const std::string& dir,
	const std::string& name)

{
	EnableLogs(true, "Database");
}


}