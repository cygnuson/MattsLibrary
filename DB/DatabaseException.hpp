#pragma once

#include "../exception.hpp"

namespace cg {

/**A data base exception.*/
class DatabaseRecordException : public cg::Exception
{
	/**Get a string of the exception.
	\return The string.*/
	virtual std::string ToString() const
	{
		return "Invalid record or inconsistant size of record.";
	}
};
/**A data base exception.*/
class DatabaseRollSizeException : public cg::Exception
{
	/**Get a string of the exception.
	\return The string.*/
	virtual std::string ToString() const
	{
		return "The roll size does not match the serialized size.";
	}
};
/**A data base exception.*/
class DatabaseFileReadException : public cg::Exception
{
	/**Get a string of the exception.
	\return The string.*/
	virtual std::string ToString() const
	{
		return "Could not read from the roll file.";
	}
};

}