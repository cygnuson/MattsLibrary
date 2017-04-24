#pragma once

#include "Term.hpp"

namespace cg {
namespace math {

/**A List of terms and a power.*/
template<typename DataType>
class Expression : public cg::math::Object
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
	/**Create a term with no objects in it.*/
	Expression();
	/**Create a term with a bunch of objects.
	\param objs The objects to add to the term.  The last object should be
	the power.*/
	template<typename...Ts>
	Expression(Ts...ts);
	/**Add an object to the term.
	\param obj The object to add.  The term will take ownership of the param
	object pointer.*/
	void AddObject(Object* obj);
	/**Set the power of the term.
	\param pow The power to set.  The term will take ownership of the param
	object pointer.*/
	void SetPower(Object* power);
private:
	/**The parts of the term.*/
	std::vector<std::shared_ptr<cg::math::Term<DataType>>> m_parts;
	/**The power object.*/
	std::shared_ptr<Object> m_power;
	/**Add objects to the term.
	\param objs The objects to add.  The term will take ownership of the param
	object pointers.  The parameter type Ts must be Object* and will have its
	ownership taken by this term.*/
	template<typename...Ts>
	void ObjectInitHelper(Object* o, Ts...objs);
	/**Add an object to the term.
	\param obj The object to add.  The term will take ownership of the param
	object pointer.*/
	void ObjectInitHelper(Object* obj);
};
template<typename DataType>
template<typename ...Ts>
inline Expression<DataType>::Expression(Ts ...ts)
{
	ObjectInitHelper(std::forward<Ts>(ts)...);
}

template<typename DataType>
template<typename ...Ts>
inline void
Expression<DataType>::ObjectInitHelper(Object* o, Ts ...obj)
{
	AddObject(o);
	ObjectInitHelper(std::forward<Ts>(obj)...);
}

template<typename DataType>
Expression<DataType>::Expression()
{

}

template<typename DataType>
void Expression<DataType>::AddObject(Object* obj)
{
	if (obj->TypeOf() == Object::Term)
	{
		auto t = dynamic_cast<cg::math::Term<DataType>*>(obj);
		m_parts.push_back(std::shared_ptr<cg::math::Term<DataType>>(t));
	}
	else
	{
		cg::math::Term<DataType>* term
			= new cg::math::Term<DataType>;
		term->AddObject(obj);
		AddObject(term);
	}
}

template<typename DataType>
void Expression<DataType>::SetPower(Object* power)
{
	if (power->TypeOf() == Object::Term)
	{
		auto t = dynamic_cast<cg::math::Term<DataType>*>(power);
		m_power = std::shared_ptr<cg::math::Term<DataType>>(t);
	}
	else
	{
		cg::math::Term<DataType>* term
			= new cg::math::Term<DataType>;
		term->AddObject(power);
		SetPower(term);
	}
}

template<typename DataType>
void Expression<DataType>::ObjectInitHelper(Object* obj)
{
	SetPower(obj);
}

template<typename DataType>
std::string Expression<DataType>::ToString() const
{
	std::stringstream ss;
	ss << "[";
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
bool Expression<DataType>::IsOne() const
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
bool Expression<DataType>::IsZero() const
{
	auto it = m_parts.begin();
	auto end = m_parts.end();
	for (; it != end; ++it)
		if ((*it)->IsZero())
			return true;
	return false;
}
/*********************************************************************************************/
template<typename DataType>
std::shared_ptr<Object> AddObjects(cg::math::Object* lhs,
	cg::math::Object* rhs)
{
	auto X = new Expression<DataType>;
	X->AddObject(lhs);
	X->AddObject(rhs);
	return std::shared_ptr<Object>(X);
}

}
}