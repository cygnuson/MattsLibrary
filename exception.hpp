#pragma once

#include "Logger.hpp"

namespace cg {

///A generic exception class for inhariting.
class Exception : public cg::Stringable, public std::exception
{
public:
	/**Translate the error to a message that can be read.
	
	Tradition what method for an exception.
	\@return A string that describes the issue at hand.*/
	virtual std::string What() const
	{
		return ToString();
	}
	/**Translate the error to a message that can be read.
	
	\return A string that shows the value of the object.*/
	virtual std::string ToString() const = 0;
	/**\sa std::exception::what
	\return Same as What()*/
	virtual const char* what() const
	{
		return What().c_str();
	}
protected:/********************************************************************PROTECTED**********/

};

class IndexOutOfBoundsException : public cg::Exception
{
public:

	/**Translate the error to a message that can be read.

	\return A string that shows the value of the object.*/
	virtual std::string ToString() const
	{
		return "The index is out of bounds.";
	}
};

}