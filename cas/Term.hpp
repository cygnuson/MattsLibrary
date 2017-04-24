#pragma once

#include <vector>
#include <memory>

#include "Number.hpp"

#define _TERMDEBUG _DEBUG && 1

namespace cg {
namespace math {

/**A term is an object that holds other objects and has a power.*/
template<typename DataType>
class Term : public cg::math::Object
{
public:
	/**Get a string of this number.
	\return The string.*/
	virtual std::string ToString() const override;
	/**Determine if the object is equivalent to one.
	\return True if the object is one.*/
	virtual bool IsOne() const override;
	/**Determine if the object is zero.
	\return True if the object is zero.*/
	virtual bool IsZero() const override;
	/**Get the type id of the object.
	\return The enum that is the type of the object.*/
	inline virtual Object::Type TypeOf() const override {
		return Object::Type::Term;
	}
	/**Create a term with no objects in it.
	\param neg The negative status of the term.*/
	Term(bool neg = false);
	/**Create a term with a bunch of objects.
	\param objs The objects to add to the term.  The last object should be
	the power.*/
	template<typename...Ts>
	Term(Ts...ts);
	/**Add an object to the term.
	\param obj The object to add.  The term will take ownership of the param
	object pointer.*/
	void AddObject(Object* obj);
	/**Set the power of the term.
	\param pow The power to set.  The term will take ownership of the param
	object pointer.*/
	void SetPower(Object* power);
	/**Set the negative status of the term.
	\param neg True to make the term negative.*/
	void Negative(bool neg);
	/**Get the negative status of the term.
	\return True if the term is negative.*/
	bool Negative();
protected:
	/**The parts of the term.*/
	std::vector<std::shared_ptr<Object>> m_parts;
	/**The power object.*/
	std::shared_ptr<Object> m_power;
	/**The sign of the term.*/
	bool m_neg = false;
	/**Add objects to the term.
	\param objs The objects to add.  The term will take ownership of the param
	object pointers.  The parameter type Ts must be Object* and will have its
	ownership taken by this term.*/
	template<typename...Ts>
	void ObjectInitHelper(cg::math::Object* o, Ts...objs);
	/**Add an object to the term.
	\param obj The object to add.  The term will take ownership of the param
	object pointer.*/
	void ObjectInitHelper(Object* obj);
};
template<typename DataType>
template<typename ...Ts>
inline Term<DataType>::Term(Ts ...ts)
{
	ObjectInitHelper(std::forward<Ts>(ts)...);
}

template<typename DataType>
template<typename ...Ts>
inline void Term<DataType>::ObjectInitHelper(cg::math::Object* o, Ts ...obj)
{
	AddObject(o);
	ObjectInitHelper(std::forward<Ts>(obj)...);
}

template<typename DataType>
Term<DataType>::Term(bool neg)
{
	m_neg = neg;
}

template<typename DataType>
void Term<DataType>::AddObject(Object * obj)
{
	m_parts.push_back(std::shared_ptr<Object>(obj));
}

template<typename DataType>
void Term<DataType>::SetPower(Object * power)
{
	m_power = std::shared_ptr<Object>(power);
}

template<typename DataType>
inline void Term<DataType>::Negative(bool neg)
{
	m_neg = neg;
}

template<typename DataType>
inline bool Term<DataType>::Negative()
{
	return m_neg;
}

template<typename DataType>
void Term<DataType>::ObjectInitHelper(Object * obj)
{
	SetPower(obj);
}

template<typename DataType>
std::string Term<DataType>::ToString() const
{
	std::stringstream ss;
	ss << (m_neg ? "-" : "+") << "[";
	auto it = m_parts.begin();
	auto end = m_parts.end();
	for (; it != end; ++it)
		ss << (*it)->ToString();
	if (m_power)
		return ss.str() + "]^[" + m_power->ToString() + "]";
	else
		return ss.str() + "]";
}

template<typename DataType>
bool Term<DataType>::IsOne() const
{
	if (m_power->IsZero())
		return true;
	auto it = m_parts.begin();
	auto end = m_parts.end();
	for (; it != end; ++it)
		if (!(*it)->IsOne())
			return false;
	return true;
}

template<typename DataType>
bool Term<DataType>::IsZero() const
{
	auto it = m_parts.begin();
	auto end = m_parts.end();
	for (; it != end; ++it)
		if ((*it)->IsZero())
			return true;
	return false;
}


}
}
