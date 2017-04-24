#pragma once

#include <map>

#include "Object.hpp"

namespace cg {
namespace math {

template<typename DataType>
class Vars
{
public:
	static DataType GetValue(const std::string& str) {
		return m_vals[str];
	}
	static void SetValue(const std::string& str, const DataType& val) {
		m_vals[str] = val;
	}
	static bool IsSet(const std::string& str) {
		return m_vals.count(str) > 0;
	}
private:
	static std::map<std::string, DataType> m_vals;
};

template<typename DataType>
std::map<std::string, DataType> Vars<DataType>::m_vals;

template<typename DataType>
class Variable : public cg::math::Object
{
public:
	/**Create the variable
	\param The text that shall reprisent the variable.*/
	Variable(const std::string& text);
	/**Create a variable
	\param pow The power.*/
	Variable(Object* pow);
	/**Get a string of this number.
	\return The string.*/
	virtual std::string ToString() const override;
	/**Determine if the object is equivalent to one.
	\return True if the object is one.*/
	virtual bool IsOne() const override;
	/**Determine if the object is zero.
	\return True if the object is zero.*/
	virtual bool IsZero() const override;
	/**Determine if the variable is set to a value.
	\return True if the variable has a value set for it.*/
	bool IsSet() const;
	/**Get the type id of the object.
	\return The enum that is the type of the object.*/
	inline virtual Object::Type TypeOf() const override {
		return Object::Type::Variable;
	}
protected:
	/**The text reprisentation of the variable.*/
	std::string m_text;
	/**The power of the variable*/
	std::shared_ptr<Object> m_power;
};

template<typename DataType>
inline Variable<DataType>::Variable(const std::string & text)
	:m_text(text) {}

template<typename DataType>
inline Variable<DataType>::Variable(Object* pow)
{
	m_power = std::shared_ptr<Object>(pow);
}


template<typename DataType>
inline std::string Variable<DataType>::ToString() const
{
	std::string outp;
	if (IsSet())
		outp = cg::ToString("(") + m_text + "{" 
		+ cg::ToString(Vars<DataType>::GetValue(m_text)) + "})";
	else
		outp = cg::ToString("(") + m_text + ")";
	if (m_power)
		return outp + "^[" + m_power->ToString() + "]";
	else
		return outp;
}

template<typename DataType>
inline bool Variable<DataType>::IsOne() const
{
	if (Vars<DataType>::IsSet(m_text))
		return Vars<DataType>::GetValue(m_text) == 1;
	else
		return false;
}

template<typename DataType>
inline bool Variable<DataType>::IsZero() const
{
	if (Vars<DataType>::IsSet(m_text))
		return Vars<DataType>::GetValue(m_text) == 0;
	else
		return false;
}

template<typename DataType>
inline bool Variable<DataType>::IsSet() const
{
	return Vars<DataType>::IsSet(m_text);
}

}
}