#pragma once

#include "../Logger.hpp"
#include "../exception.hpp"
#include "../Power.hpp"
#include "Prefix.hpp"

#define _DEBUGNUMBER _DEBUG && 1

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
class Number : public cg::Stringable
{
public:
	/**The same type as this object.*/
	using SelfType = Number<NumType>;
	/**Create a number with a power.
	\param num The number to use.
	\param power The power to use.*/
	Number(const NumType& num =(NumType) 0, 
		const NumType& power = (NumType) 0);
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
	/**Equalize two sci notation numbers.*/
	static void Equalize(SelfType& first, SelfType& second);
	/**The number value.*/
	NumType m_base;
	/**The power of the number.*/
	NumType m_pow;
};

template<typename NumType>
inline Number<NumType>::Number(const NumType & num, const NumType & power)
	:m_base(num), m_pow(power) 
{
	int i = 0;
}

template<typename NumType>
inline  typename Number<NumType>::SelfType&
Number<NumType>::operator+=(SelfType other)
{
	Equalize(*this, other);
	m_base += other.m_base;
	while (m_base % 10 == 0)
	{
		m_base /= 10;
		++m_pow;
	}
	return *this;
}

template<typename NumType>
inline typename Number<NumType>::SelfType&
Number<NumType>::operator-=(SelfType other)
{
	Equalize(*this, other);
	m_base -= other.m_base;
	while (m_base % 10 == 0)
	{
		m_base /= 10;
		++m_pow;
	}
	return *this;
}

template<typename NumType>
inline  typename Number<NumType>::SelfType
Number<NumType>::operator+(const SelfType & other)const
{
	auto copy = *this;
	return copy += other;
}

template<typename NumType>
inline  typename Number<NumType>::SelfType
Number<NumType>::operator-(const SelfType & other)const
{
	auto copy = *this;
	return copy -= other;
}

template<typename NumType>
inline typename Number<NumType>::SelfType &
Number<NumType>::operator*=(const SelfType & other)
{
	if (IsZero())
	{
		*this = SelfType();
		return *this;
	}
	if (IsOne())
	{
		*this = other;
		return *this;
	}
	m_base *= other.m_base;
	m_pow += other.m_pow;
	return *this;
}

template<typename NumType>
inline typename Number<NumType>::SelfType &
Number<NumType>::operator/=(const SelfType & other)
{
	if (IsZero())
	{
		*this = SelfType();
		return *this;
	}
	m_base /= other.m_base;
	m_pow -= other.m_pow;
	return *this;
}

template<typename NumType>
inline typename Number<NumType>::SelfType &
Number<NumType>::operator%=(SelfType other)
{
	if (IsZero())
	{
		*this = SelfType();
		return *this;
	}
	Equalize(*this, other);
	m_base %= other.m_base;
	return *this;
}

template<typename NumType>
inline typename Number<NumType>::SelfType
Number<NumType>::operator*(const SelfType & other)const
{
	auto copy = *this;
	return copy *= other;
}

template<typename NumType>
inline typename Number<NumType>::SelfType
Number<NumType>::operator/(const SelfType & other)const
{
	auto copy = *this;
	return copy /= other;
}

template<typename NumType>
inline typename Number<NumType>::SelfType
Number<NumType>::operator%(const SelfType & other)const
{
	auto copy = *this;
	return copy %= other;
}

template<typename NumType>
inline typename Number<NumType>::SelfType &
Number<NumType>::operator++()
{
	++m_base;
	return *this;
}

template<typename NumType>
inline typename Number<NumType>::SelfType
Number<NumType>::operator++(int nothing)
{
	auto copy= *this;
	++(*this);
	return copy;
}

template<typename NumType>
inline typename Number<NumType>::SelfType &
Number<NumType>::operator--()
{
	--m_base;
	return *this;
}

template<typename NumType>
inline typename Number<NumType>::SelfType
Number<NumType>::operator--(int nothing)
{
	auto copy= *this;
	--(*this);
	return copy;
}

template<typename NumType>
inline bool Number<NumType>::operator==(const SelfType & other)const
{
	return m_base == other.m_base && m_pow == other.m_pow;
}

template<typename NumType>
inline bool Number<NumType>::operator!=(const SelfType & other)const
{
	return !(*this == other);
}

template<typename NumType>
inline bool Number<NumType>::operator<(SelfType other)const
{
	auto copy = *this;
	Equalize(copy, other);
	return copy.m_base < other.m_base;
}

template<typename NumType>
inline bool Number<NumType>::operator<=(const SelfType & other)const
{
	return (*this < other) || (*this == other);
}

template<typename NumType>
inline bool Number<NumType>::operator>(const SelfType & other)const
{
	return !(*this <= other);
}

template<typename NumType>
inline bool Number<NumType>::operator>=(const SelfType & other)const
{
	return !(*this < other);
}

template<typename NumType>
inline bool Number<NumType>::IsZero() const
{
	return m_base == 0;
}

template<typename NumType>
inline bool Number<NumType>::IsOne() const
{
	return m_base == 1 && m_pow == 0;
}

template<typename NumType>
inline std::string Number<NumType>::ToString() const
{
	return std::string("(") + cg::ToString(m_base) + ")^"
		+ "(" + cg::ToString(m_pow) + ")";
}


template<typename NumType>
inline void Number<NumType>::Equalize(SelfType & first,
	SelfType & second)
{
	auto& larger = first.m_pow > second.m_pow ? first : second;
	auto& lesser = first.m_pow > second.m_pow ? second : first;
	auto diff = larger.m_pow - lesser.m_pow;
	larger.m_base *= cg::RTPower(10,diff);
	larger.m_pow -= diff;
}

}
}


#if _DEBUGNUMBER
template class cg::math::Number<int>;
#endif