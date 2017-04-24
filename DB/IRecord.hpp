#pragma once

#include "../Serial.hpp"
#include "DatabaseException.hpp"

namespace cg {

/**A record interface.  Anything that imlements IRecord must always have the
same size.*/
class IRecord : public cg::Serializable
{
public:
	/**virtual dtor*/
	virtual ~IRecord() {};
	/**Push this object to the serial.
	\param s The serial object to push to.*/
	virtual void Push(cg::Serial& s) const = 0;
	/**Pull data from the serial to this object.
	\param s The serial object to pull from.*/
	virtual void Pull(cg::Serial& s) = 0;
	/**Get the ID of the record.
	\return The ID of the record.*/
	virtual std::uint64_t Id() const = 0;
	/**Compare this to another record for sorting.
	\param other The thing to compare.
	\return True if this object should come before `other`*/
	virtual bool operator<(const IRecord& other) const {
		if (typeid(*this) != typeid(other))
			throw DatabaseRecordException();
		else
			return Id() < other.Id();
	}
protected:
	/**True if the record is in uese.*/
	bool m_valid = false;
};

}