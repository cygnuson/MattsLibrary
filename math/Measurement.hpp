#pragma once

#include <map>

#include "Number.hpp"
#include "Rational.hpp"

#define _DEBUGMEASUREMENT _DEBUG && 1

namespace cg {
namespace math {

class UnitException : cg::Exception
{
	/**Get the message.*/
	virtual std::string ToString() const {
		return "The units associated with the opprand is invalid.";
	}
};

template<typename NumType>
class Measurement
{
public:
	/**The same type as this object.*/
	using SelfType = Measurement<NumType>;
	/**Create a number with a power.
	\param num The number to use.
	\param power The power to use.*/
	template<typename...Units>
	Measurement(const cg::math::Rational<NumType>& num = 0,
		const NumType& power = 0,
		cg::math::Prefix prefix = Prefix::None,
		Units...units);
	/**Add units to the measurement.
	\param unit The unit to add.
	\param units The other units.*/
	template<typename...Units>
	void MultiplyUnits(Unit unit, Units...units);
	/**Add units to the measurement.
	\param unit The unit to add.*/
	inline void MultiplyUnits(Unit unit);
	/**Empty function*/
	inline void MultiplyUnits() {};
	/**Remove units to the measurement.
	\param unit The unit to Remove.
	\param units The other units.*/
	template<typename...Units>
	void DivideUnits(Unit unit, Units...units);
	/**Remove units to the measurement.
	\param unit The unit to Remove.*/
	inline void DivideUnits(Unit unit);
	/**Operator + .
	\param other The other thing to operate on.
	\return A reference to this.*/
	SelfType& operator+=(SelfType other);
	/**Operator - .
	\param other The other thing to operate on.
	\return A reference to this.*/
	SelfType& operator-=(SelfType other);
	/**Operator + .
	\param other The other thing to operate on.
	\return A copy of this.*/
	SelfType operator+(const SelfType& other)const;
	/**Operator - .
	\param other The other thing to operate on.
	\return A copy of this.*/
	SelfType operator-(const SelfType& other)const;
	/**Operator * .
	\return A copy of this.*/
	SelfType& operator*=(const SelfType& other);
	/**Operator / .
	\return A copy of this.*/
	SelfType& operator/=(const SelfType& other);
	/**Operator % .
	\return A copy of this.*/
	SelfType& operator%=(SelfType other);
	/**Operator * .
	\return A copy of this.*/
	SelfType operator*(const SelfType& other)const;
	/**Operator / .
	\return A copy of this.*/
	SelfType operator/(const SelfType& other)const;
	/**Operator % .
	\return A copy of this.*/
	SelfType operator%(const SelfType& other)const;
	/**Increment the poitner by 1. (prefix)
	\return This same type.*/
	inline SelfType& operator++();
	/**Increment this by 1. (postfix)
	\param nothing Ignored pointer.
	\return a copy of this befor incrementing.*/
	inline SelfType operator++(int nothing);
	/**Decrement the poitner by 1. (prefix)
	\return This same type after incrementing.*/
	inline SelfType& operator--();
	/**Decrement this by 1. (postfix)
	\param nothing Ignored pointer.
	\return a copy of this before decrementing.*/
	inline SelfType operator--(int nothing);
	/**Comparison operator
	\param other The other thing to compare.
	\return True if the comparison is true.*/
	bool operator==(const SelfType& other)const;
	/**Comparison operator
	\param other The other thing to compare.
	\return True if the comparison is true.*/
	bool operator!=(const SelfType& other)const;
	/**Comparison operator
	\param other The other thing to compare.
	\return True if the comparison is true.*/
	bool operator<(SelfType other)const;
	/**Comparison operator
	\param other The other thing to compare.
	\return True if the comparison is true.*/
	bool operator<=(const SelfType& other)const;
	/**Comparison operator
	\param other The other thing to compare.
	\return True if the comparison is true.*/
	bool operator>(const SelfType& other)const;
	/**Comparison operator
	\param other The other thing to compare.
	\return True if the comparison is true.*/
	bool operator>=(const SelfType& other) const;
	/**Determine if this object is zero.
	\return True if the object is zero.*/
	bool IsZero() const;
	/**Determine if this object is zero.
	\return True if the object is zero.*/
	bool IsOne() const;
	/**Convert to a string.
	\return The string.*/
	std::string ToString() const;
private:
	/**Determine if an object can be added to subbed from this measurement.
	\param other The other thing.
	\return True if the units of `other` are in a state that can be added to
	this measuremnt.*/
	bool CanAdd(const SelfType& other) const;
	/**Equalize two sci notation numbers.*/
	static void Equalize(SelfType& first, SelfType& second);
	/**The number value.*/
	cg::math::Rational<NumType> m_base;
	/**The power of the number.*/
	NumType m_pow;
	/**The unit of the measurement.*/
	std::map<Unit, int32_t> m_units;
};

template<typename NumType>
template<typename...Units>
inline Measurement<NumType>::Measurement(
	const cg::math::Rational<NumType> & num,
	const NumType & power,
	cg::math::Prefix prefix,
	Units...units)
	:m_base(num), m_pow(power)
{
	MultiplyUnits(std::forward<Units>(units)...);
}

template<typename NumType>
template<typename ...Units>
inline void Measurement<NumType>::MultiplyUnits(Unit unit, Units ...units)
{
	if (m_units.count(unit) == 0)
		m_units[unit] = 0;
	++m_units[unit];
	MultiplyUnits(std::forward<Units>(units)...);
}

template<typename NumType>
template<typename ...Units>
inline void Measurement<NumType>::DivideUnits(Unit unit, Units ...units)
{
	if (m_units.count(unit) == 0)
		m_units[unit] = 0;
	--m_units[unit];
	DivideUnits(std::forward<Units>(units)...);
}

template<typename NumType>
inline void Measurement<NumType>::MultiplyUnits(Unit unit)
{
	if (m_units.count(unit) == 0)
		m_units[unit] = 0;
	++m_units[unit];
}

template<typename NumType>
inline void Measurement<NumType>::DivideUnits(Unit unit)
{
	if (m_units.count(unit) == 0)
		m_units[unit] = 0;
	--m_units[unit];
}

template<typename NumType>
inline typename Measurement<NumType>::SelfType &
Measurement<NumType>::operator+=(SelfType other)
{
	if (!CanAdd(other))
		throw UnitException();

	Equalize(*this, other);
	m_base += other.m_base;
	while ((m_base % 10).operator==(0))
	{
		m_base /= 10;
		++m_pow;
	}
	return *this;
}

template<typename NumType>
inline typename Measurement<NumType>::SelfType &
Measurement<NumType>::operator-=(SelfType other)
{
	if (!CanAdd(other))
		throw UnitException();
	Equalize(*this, other);
	m_base -= other.m_base;
	while ((m_base % 10).operator==(0))
	{
		m_base /= 10;
		++m_pow;
	}
	return *this;
}

template<typename NumType>
inline typename Measurement<NumType>::SelfType
Measurement<NumType>::operator+(const SelfType & other) const
{
	auto copy = *this;
	return copy += other;
}

template<typename NumType>
inline typename Measurement<NumType>::SelfType
Measurement<NumType>::operator-(const SelfType & other) const
{
	auto copy = *this;
	return copy -= other;
}

template<typename NumType>
inline typename Measurement<NumType>::SelfType &
Measurement<NumType>::operator*=(const SelfType & other)
{
	if (IsZero())
	{
		this->m_base = 0;
		this->m_pow = 0;
		for (auto& e : other.m_units)
		{
			m_units[e.first] += e.second;
		}
		return *this;
	}
	if (IsOne())
	{
		this->m_base = other.m_base;
		this->m_pow = other.m_pow;
		for (auto& e : other.m_units)
		{
			m_units[e.first] += e.second;
		}
		return *this;
	}
	m_base *= other.m_base;
	m_pow += other.m_pow;
	for (auto& e : other.m_units)
	{
		m_units[e.first] += e.second;
	}
	return *this;
}

template<typename NumType>
inline typename Measurement<NumType>::SelfType &
Measurement<NumType>::operator/=(const SelfType & other)
{
	if (IsZero())
	{
		this->m_base = 0;
		this->m_pow = 0;
		for (auto& e : other.m_units)
		{
			m_units[e.first] += e.second;
		}
		return *this;
	}
	m_base /= other.m_base;
	m_pow -= other.m_pow;
	for (auto& e : other.m_units)
	{
		m_units[e.first] -= e.second;
	}
	return *this;
}

template<typename NumType>
inline typename Measurement<NumType>::SelfType &
Measurement<NumType>::operator%=(SelfType other)
{
	if (IsZero())
	{
		this->m_base = 0;
		this->m_pow = 0;
		for (auto& e : other.m_units)
		{
			m_units[e.first] += e.second;
		}
		return *this;
	}
	Equalize(*this, other);
	m_base /= other.m_base;
	m_pow -= other.m_pow;
	for (auto& e : other.m_units)
	{
		m_units[e.first] -= e.second;
	}
	return *this;
}

template<typename NumType>
inline typename Measurement<NumType>::SelfType
Measurement<NumType>::operator*(const SelfType & other) const
{
	auto copy = *this;
	return copy *= other;
}

template<typename NumType>
inline typename Measurement<NumType>::SelfType
Measurement<NumType>::operator/(const SelfType & other) const
{
	auto copy = *this;
	return copy /= other;
}

template<typename NumType>
inline typename Measurement<NumType>::SelfType
Measurement<NumType>::operator%(const SelfType & other) const
{
	auto copy = *this;
	return copy %= other;
}

template<typename NumType>
inline typename Measurement<NumType>::SelfType &
Measurement<NumType>::operator++()
{
	++m_base;
	return *this;
}

template<typename NumType>
inline typename Measurement<NumType>::SelfType
Measurement<NumType>::operator++(int nothing)
{
	auto copy = *this;
	++(*this);
	return copy;
}

template<typename NumType>
inline typename Measurement<NumType>::SelfType &
Measurement<NumType>::operator--()
{
	--m_base;
	return *this;
}

template<typename NumType>
inline typename Measurement<NumType>::SelfType
Measurement<NumType>::operator--(int nothing)
{
	auto copy = *this;
	--(*this);
	return copy;
}

template<typename NumType>
inline bool Measurement<NumType>::operator==(const SelfType & other) const
{
	if (!CanAdd(other))
		return false;
	return (m_base == other.m_base) 
		&& (m_pow == other.m_pow);
}

template<typename NumType>
inline bool Measurement<NumType>::operator!=(const SelfType & other) const
{
	return !(*this == other);
}

template<typename NumType>
inline bool Measurement<NumType>::operator<(SelfType other) const
{
	if (!CanAdd(other))
		throw UnitException();
	auto copy = *this;
	Equalize(copy, other);
	return copy.m_base < other.m_base;
}

template<typename NumType>
inline bool Measurement<NumType>::operator<=(const SelfType & other) const
{
	return (*this < other) || (*this == other) ;
}

template<typename NumType>
inline bool Measurement<NumType>::operator>(const SelfType & other) const
{
	return !(*this <= other);
}

template<typename NumType>
inline bool Measurement<NumType>::operator>=(const SelfType & other) const
{
	return !(*this < other);
}

template<typename NumType>
inline bool Measurement<NumType>::IsZero() const
{
	return m_base == Rational<NumType>(0);
}

template<typename NumType>
inline bool Measurement<NumType>::IsOne() const
{
	return m_base == Rational<NumType>(1) || m_pow == Rational<NumType>(0);
}

template<typename NumType>
inline std::string Measurement<NumType>::ToString() const
{
	return m_base.ToString() + "^(" + cg::ToString(m_pow) + ")";
}


template<typename NumType>
inline bool Measurement<NumType>::CanAdd(const SelfType & other) const
{
	if(m_units.size() != other.m_units.size())
		return false;
	return std::equal(m_units.begin(), m_units.end(), 
		other.m_units.begin(), other.m_units.end());
}

template<typename NumType>
inline void Measurement<NumType>::Equalize(SelfType & first,
	SelfType & second)
{
	auto& larger = first.m_pow > second.m_pow ? first : second;
	auto& lesser = first.m_pow > second.m_pow ? second : first;
	auto diff = larger.m_pow - lesser.m_pow;
	larger.m_base *= cg::RTPower<NumType>(10, diff);
	larger.m_pow -= diff;
}

}
}

#if _DEBUGMEASUREMENT
template class cg::math::Measurement<int>;
#endif