#pragma once

#include "Logger.hpp"

namespace cg {

///A generic exception class for inhariting.
class Exception : public cg::Stringable
{
public:
	///Translate the error to a message that can be read.
	///
	///Tradition what method for an exception.
	///@return A string that describes the issue at hand.
	virtual std::string What() const = 0;
	///Translate the error to a message that can be read.
	///
	///@return A string that shows the value of the object.
	virtual std::string ToString() const = 0;
protected:/********************************************************************PROTECTED**********/

};

}