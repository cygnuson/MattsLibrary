#pragma once

#include "../Logger.hpp"
#include "../exception.hpp"
#include "../Power.hpp"

namespace cg {
namespace math {

class PowerException : public cg::Exception
{
	/**Get the message.
	\return A string with the message.*/
	inline virtual std::string ToString() const {
		return "The power is invalid for this operation.";
	}
};

/**An object to wrap a number into scientific notation.
\tparam A type of number. Must have all math operators available.*/
template<typename NumType>
class SciNoWraper
{
public:
	/**The same type as this object.*/
	using SelfType = SciNoWraper<NumType>;
	/**The type for the power.*/
	using PowerType = int32_t;
	/**Create a number with a power.
	\param num The number to use.
	\param power The power to use.*/
	SciNoWraper(const NumType& num = 0, PowerType power = 1);
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
	SelfType operator+(const SelfType& other);
	/**Operator - .
	\param other The other thing to operate on.
	\return A copy of this.*/
	SelfType operator-(const SelfType& other);
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
	SelfType operator*(const SelfType& other);
	/**Operator / .
	\return A copy of this.*/
	SelfType operator/(const SelfType& other);
	/**Operator % .
	\return A copy of this.*/
	SelfType operator%(const SelfType& other);
	/**Determine if this object is zero.
	\return True if the object is zero.*/
	bool IsZero() const;
	/**Determine if this object is zero.
	\return True if the object is zero.*/
	bool IsOne() const;
private:
	/**Equalize two sci notation numbers.*/
	static void Equalize(SelfType& first, SelfType& second);
	/**The number value.*/
	NumType m_num;
	/**The power of the number.*/
	int32_t m_pow;
};

template<typename NumType>
inline SciNoWraper<NumType>::SciNoWraper(const NumType & num, PowerType power)
	:m_num(num), m_pow(power) {}

template<typename NumType>
inline  typename SciNoWraper<NumType>::SelfType&
SciNoWraper<NumType>::operator+=(SelfType other)
{
	Equalize(*this, other);
	m_num += other.m_num;
	while (m_num % 10 == 0)
	{
		m_num /= 10;
		++m_pow;
	}
	return *this;
}

template<typename NumType>
inline typename SciNoWraper<NumType>::SelfType&
SciNoWraper<NumType>::operator-=(SelfType other)
{
	Equalize(*this, other);
	m_num -= other.m_num;
	while (m_num % 10 == 0)
	{
		m_num /= 10;
		++m_pow;
	}
	return *this;
}

template<typename NumType>
inline  typename SciNoWraper<NumType>::SelfType
SciNoWraper<NumType>::operator+(const SelfType & other)
{
	auto copy = *this;
	return copy += other;
}

template<typename NumType>
inline  typename SciNoWraper<NumType>::SelfType
SciNoWraper<NumType>::operator-(const SelfType & other)
{
	auto copy = *this;
	return copy -= other;
}

template<typename NumType>
inline typename SciNoWraper<NumType>::SelfType &
SciNoWraper<NumType>::operator*=(const SelfType & other)
{
	if (IsZero())
		return SelfType();
	if (IsOne())
		*this = other;
	m_num *= other.m_num;
	m_pow += other.m_pow;
	return *this;
}

template<typename NumType>
inline typename SciNoWraper<NumType>::SelfType &
SciNoWraper<NumType>::operator/=(const SelfType & other)
{
	if (IsZero())
		return SelfType();
	m_num /= other.m_num;
	m_pow -= other.m_pow;
	return *this;
}

template<typename NumType>
inline typename SciNoWraper<NumType>::SelfType &
SciNoWraper<NumType>::operator%=(SelfType other)
{
	if (IsZero())
		return SelfType();
	Equalize(*this, other);
	m_num %= other.m_num;
	return *this;
}

template<typename NumType>
inline typename SciNoWraper<NumType>::SelfType
SciNoWraper<NumType>::operator*(const SelfType & other)
{
	auto copy = *this;
	return copy *= other;
}

template<typename NumType>
inline typename SciNoWraper<NumType>::SelfType
SciNoWraper<NumType>::operator/(const SelfType & other)
{
	auto copy = *this;
	return copy /= other;
}

template<typename NumType>
inline typename SciNoWraper<NumType>::SelfType
SciNoWraper<NumType>::operator%(const SelfType & other)
{
	auto copy = *this;
	return copy %= other;
}

template<typename NumType>
inline bool SciNoWraper<NumType>::IsZero() const
{
	return m_num == 0;
}

template<typename NumType>
inline bool SciNoWraper<NumType>::IsOne() const
{
	return m_num == 1 && m_pow == 0;
}


template<typename NumType>
inline void SciNoWraper<NumType>::Equalize(SelfType & first,
	SelfType & second)
{
	auto& larger = first.m_pow > second.m_pow ? first : second;
	auto& lesser = first.m_pow > second.m_pow ? second : first;
	auto diff = larger.m_pow - lesser.m_pow;
	larger.m_num *= cg::RTPower(10,diff);
	larger.m_pow -= diff;
}

}
}