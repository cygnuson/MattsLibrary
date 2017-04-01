#pragma once

#include "../LogAdaptor.hpp"
#include "Helpers.hpp"

#define _DEBUGRATIONAL _DEBUG && 1

namespace cg {
namespace math {

template<typename DataType>
class Rational : public cg::LogAdaptor<Rational<DataType>> , 
	public cg::Stringable
{
public:
	/**The type of floating point to use.*/
	using FloatType = long double;
	/**The self type*/
	using SelfType = Rational<DataType>;
	/**Create a ratioal unit without any initialized data.*/
	Rational();
	/**Create a rational unit with a numerator and denominator.
	\param numerator The top of the rational unit.
	\param denominator The bottom of the rational unit.*/
	Rational(const DataType& numerator, const DataType& denominator);
	/**Create a rational number with just a numerator.  The denominator will be
	initialized to 1.
	\param num The number to create.*/
	Rational(const DataType& num);
	/**Create a rational number from another rational.
	\param other The other rational number.*/
	template<typename NewType>
	Rational(const Rational<NewType>& other);
	/**Get a copy of this rational but as a reciprocal.
	\return A rational that is the reciprocol of this number.*/
	Rational<DataType> Reciprocal() const;
	/**Get a string version of this rational.
	\return A string rep of this object.*/
	std::string ToString() const;
	/**Equalize two rationals to the same denominator.
	\param lhs The first thing to equalize.
	\param rhs The second thing to equalize.*/
	static void Equalize(SelfType& lhs, SelfType& rhs);
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
	/**Do some math.
	\param other The other thing to add to this.
	\return This object after the operation.*/
	SelfType& operator+=(SelfType other);
	/**Do some math.
	\param other The other thing to sub from this.
	\return This object after the operation.*/
	SelfType& operator-=(SelfType other);
	/**Multiply a number to this one.
	\param other The other number to be multiplied to this one.
	\return This object after the operation.*/
	SelfType& operator*=(const SelfType& other);
	/**Devide this number by another one.
	\param other The other number to be used in the operation.
	\return This object after the operation.*/
	SelfType& operator/=(const SelfType& other);
	/**Devide this number by another one.
	\param other The other number to be used in the operation.
	\return This object after the operation.*/
	SelfType& operator%=(SelfType other);
	/**Math operator
	\param other The other thing in the op.
	\return A copy with the result.*/
	SelfType operator+(const SelfType& other)const;
	/**Math operator
	\param other The other thing in the op.
	\return A copy with the result.*/
	SelfType operator-(const SelfType& other)const;
	/**Math operator
	\param other The other thing in the op.
	\return A copy with the result.*/
	SelfType operator*(const SelfType& other)const;
	/**Math operator
	\param other The other thing in the op.
	\return A copy with the result.*/
	SelfType operator/(const SelfType& other)const;
	/**Math operator
	\param other The other thing in the op.
	\return A copy with the result.*/
	SelfType operator%(const SelfType& other)const;
	/**Math operator
	\param other The other thing in the op.
	\return A copy with the result.*/
	SelfType operator%(const DataType& other)const;
	/*Get the logarithm of the number.
	\param base The base for the log operation (default 2).
	\return The largest floating point type that is the log_b of this object.
	*/
	FloatType Log(const DataType& base);
	/*Get the logarithm of the number.
	\param base The base for the log operation (default 2).
	\return The largest floating point type that is the log_b of this object.
	*/
	FloatType Ln();
	/**Do some comparing.
	\param other The other thing to compare to.
	\return True if this object is less than `other`.*/
	bool operator<(SelfType other)const;
	/**Do some comparing.
	\param other The other thing to compare to.
	\return True if this object is greater than `other`.*/
	bool operator>(SelfType other)const;
	/**Do some comparing.
	\param other The other thing to compare to.
	\return True if this object is greater than or equal to `other`.*/
	bool operator>=(SelfType other)const;
	/**Do some comparing.
	\param other The other thing to compare to.
	\return True if this object is less than or equal to `other`.*/
	bool operator<=(SelfType other)const;
	/**Do some comparing.
	\param other The other thing to compare to.
	\return True if this object is equal to `other`.*/
	bool operator==(SelfType other)const;
	/**Do some comparing.
	\param other The other thing to compare to.
	\return True if this object is not equal to `other`.*/
	bool operator!=(SelfType other) const;
	/**Convert to the FloatType by performing the division.
	\return The number as a FloatType.*/
	operator FloatType() {
		return FloatType(m_numerator) / FloatType(m_denominator);
	}
	/**A unit of DataType that will be the numerator*/
	DataType m_numerator;
	/**A unit of DataType that will be the denominator.*/
	DataType m_denominator;
	/**Cause the rational to simplify.*/
	void Simplify();
	/**Make this object into its reciprical.*/
	void MakeReciprocal();
private:
	using cg::LogAdaptor<Rational<DataType>>::EnableLogs;
	using cg::LogAdaptor<Rational<DataType>>::LogNote;
	using cg::LogAdaptor<Rational<DataType>>::LogWarn;
	using cg::LogAdaptor<Rational<DataType>>::LogError;
	using cg::LogAdaptor<Rational<DataType>>::Log;
	using cg::LogAdaptor<Rational<DataType>>::ms_log;
	using cg::LogAdaptor<Rational<DataType>>::ms_name;
	/**Set a numerator and denominator.
	\param numerator The top of the rational unit.
	\param denominator The bottom of the rational unit.*/
	void Init(const DataType& numerator, const DataType& denominator);
};

template<typename DataType>
inline Rational<DataType>::Rational()
{
	EnableLogs(true, "Rational");
}

template<typename DataType>
inline Rational<DataType>::Rational(const DataType & numerator,
	const DataType & denominator)
{
	EnableLogs(true, "Rational");
	Init(numerator, denominator);
}

template<typename DataType>
inline Rational<DataType>::Rational(const DataType & num)
{
	EnableLogs(true, "Rational");
	Init(num, 1);
}

template<typename DataType>
inline Rational<DataType> Rational<DataType>::Reciprocal() const
{
	return Rational<DataType>(m_denominator, m_numerator);
}

template<typename DataType>
inline std::string Rational<DataType>::ToString() const
{
	return std::string("(") + cg::ToString(m_numerator) + "/"
		+ cg::ToString(m_denominator) + ")";
}

template<typename DataType>
inline void Rational<DataType>::Equalize(SelfType & lhs, SelfType & rhs)
{
	auto LCM = cg::math::LCM<DataType>(lhs.m_denominator, rhs.m_denominator);
	auto lhsFactor = LCM / lhs.m_denominator;
	auto rhsFactor = LCM / rhs.m_denominator;
	lhs.m_denominator = LCM;
	rhs.m_denominator = LCM;
	lhs.m_numerator *= lhsFactor;
	rhs.m_numerator *= rhsFactor;
}

template<typename DataType>
inline typename Rational<DataType>::SelfType &
Rational<DataType>::operator++()
{
	m_numerator += m_denominator;
	return *this;
}

template<typename DataType>
inline typename Rational<DataType>::SelfType
Rational<DataType>::operator++(int nothing)
{
	auto copy = *this;
	m_numerator += m_denominator;
	return copy;
}

template<typename DataType>
inline typename Rational<DataType>::SelfType &
Rational<DataType>::operator--()
{
	m_numerator -= m_denominator;
	return *this;
}

template<typename DataType>
inline typename Rational<DataType>::SelfType
Rational<DataType>::operator--(int nothing)
{
	auto copy = *this;
	m_numerator -= m_denominator;
	return copy;
}

template<typename DataType>
inline Rational<DataType>&
Rational<DataType>::operator+=(SelfType other)
{
	Simplify();
	other.Simplify();
	DataType lhsWholes = 0;
	DataType rhsWholes = 0;
	if (m_numerator > m_denominator)
	{
		lhsWholes = m_numerator / m_denominator;
		m_numerator %= m_denominator;
	}
	if (other.m_numerator > other.m_denominator)
	{
		rhsWholes = other.m_numerator / other.m_denominator;
		other.m_numerator %= other.m_denominator;
	}
	Equalize(*this, other);
	m_numerator += other.m_numerator;
	m_numerator += (lhsWholes + rhsWholes)*m_denominator;
	Simplify();
	other.Simplify();
	return *this;
}

template<typename DataType>
inline Rational<DataType>& Rational<DataType>::operator-=(SelfType other)
{
	Simplify();
	other.Simplify();
	DataType lhsWholes = 0;
	DataType rhsWholes = 0;
	if (m_numerator > m_denominator)
	{
		lhsWholes = m_numerator / m_denominator;
		m_numerator %= m_denominator;
	}
	if (other.m_numerator > other.m_denominator)
	{
		rhsWholes = other.m_numerator / other.m_denominator;
		other.m_numerator %= other.m_denominator;
	}
	Equalize(*this, other);
	m_numerator -= other.m_numerator;
	m_numerator += lhsWholes * m_denominator;
	m_numerator -= rhsWholes * m_denominator;
	Simplify();
	other.Simplify();
	return *this;
}

template<typename DataType>
inline typename Rational<DataType>::SelfType &
Rational<DataType>::operator*=(const SelfType & other)
{
	m_numerator *= other.m_numerator;
	m_denominator *= other.m_denominator;
	return *this;
}

template<typename DataType>
inline typename Rational<DataType>::SelfType &
Rational<DataType>::operator/=(const SelfType & other)
{
	auto copy = other.Reciprocal();
	return *this *= copy;
}

template<typename DataType>
inline typename Rational<DataType>::SelfType &
Rational<DataType>::operator%=(SelfType other)
{
	Equalize(*this, other);
	m_numerator %= other.m_numerator;
	Simplify();
	return *this;
}

template<typename DataType>
inline typename Rational<DataType>::SelfType
Rational<DataType>::operator+(const SelfType & other)const
{
	auto copy = *this;
	return copy += other;
}

template<typename DataType>
inline typename Rational<DataType>::SelfType
Rational<DataType>::operator-(const SelfType & other)const
{
	auto copy = *this;
	return copy -= other;
}

template<typename DataType>
inline typename Rational<DataType>::SelfType
Rational<DataType>::operator*(const SelfType & other)const
{
	auto copy = *this;
	return copy *= other;
}

template<typename DataType>
inline typename Rational<DataType>::SelfType
Rational<DataType>::operator/(const SelfType & other)const
{
	auto copy = *this;
	return copy /= other;
}

template<typename DataType>
inline typename Rational<DataType>::SelfType
Rational<DataType>::operator%(const SelfType & other)const
{
	auto copy = *this;
	return copy %= other;
}

template<typename DataType>
inline typename Rational<DataType>::SelfType
Rational<DataType>::operator%(const DataType & other) const
{
	return *this % SelfType(other);
}

template<typename DataType>
inline typename Rational<DataType>::FloatType
Rational<DataType>::Log(const DataType & base)
{
	FloatType ans = 0;
	ans = std::log10l(m_numerator);
	if (m_denominator != 1)
		ans -= std::log10l(m_denominator);
	if (base != 10)
		ans /= std::log10l(base);
	return ans;
}

template<typename DataType>
inline typename Rational<DataType>::FloatType
Rational<DataType>::Ln()
{
	FloatType ans = 0;
	ans = std::log1p(m_numerator);
	if (m_denominator != 1)
		ans -= std::log1p(m_denominator);
	return ans;
}

template<typename DataType>
inline bool Rational<DataType>::operator<(SelfType other)const
{
	auto copy = *this;
	Equalize(copy, other);
	return copy.m_numerator < other.m_numerator;
}

template<typename DataType>
inline bool Rational<DataType>::operator>(SelfType other)const
{
	return !(*this <= other);
}

template<typename DataType>
inline bool Rational<DataType>::operator>=(SelfType other)const
{
	auto copy = *this;
	Equalize(copy, other);
	auto eq = copy.m_numerator == other.m_numerator;
	auto eq2 = copy.m_numerator > other.m_numerator;
	copy.Simplify();
	other.Simplify();
	return eq || eq2;
}

template<typename DataType>
inline bool Rational<DataType>::operator<=(SelfType other)const
{
	auto copy = *this;
	Equalize(copy, other);
	auto eq = copy.m_numerator == other.m_numerator;
	auto eq2 = copy.m_numerator < other.m_numerator;
	copy.Simplify();
	other.Simplify();
	return eq || eq2;
}

template<typename DataType>
inline bool Rational<DataType>::operator==(SelfType other)const
{
	auto copy = *this;
	Equalize(copy, other);
	auto eq = copy.m_numerator == other.m_numerator;
	copy.Simplify();
	other.Simplify();
	return eq;
}

template<typename DataType>
inline bool Rational<DataType>::operator!=(SelfType other)const
{
	return !(*this == other);
}

template<typename DataType>
inline void Rational<DataType>::MakeReciprocal()
{
	auto X = m_numerator;
	m_numerator = m_denominator;
	m_denominator = X;
}

template<typename DataType>
inline void Rational<DataType>::Simplify()
{
	if (m_numerator == 0)
		return;
	if (m_denominator == m_numerator)
	{
		m_denominator = 1;
		m_numerator = 1;
		return;
	}
	if (m_denominator == 1 || m_numerator == 1)
		return;
	if (m_numerator > m_denominator)
	{
		if (m_numerator % m_denominator == 0)
		{
			m_numerator /= m_denominator;
			m_denominator = 1;
			return;
		}
		DataType wholeUnits = m_numerator / m_denominator;
		m_numerator %= m_denominator;
		auto GCD = cg::math::GCD<DataType>(m_numerator, m_denominator);
		m_numerator /= GCD;
		m_denominator /= GCD;
		m_numerator += wholeUnits * m_denominator;
		return;
	}
	if (m_denominator > m_numerator)
	{
		MakeReciprocal();
		Simplify();
		MakeReciprocal();
		return;
	}
}

template<typename DataType>
inline void Rational<DataType>::Init(const DataType & numerator,
	const DataType & denominator)
{
	m_numerator = numerator;
	m_denominator = denominator;
	Simplify();
}

template<typename DataType>
template<typename NewType>
inline Rational<DataType>::Rational(const Rational<NewType>& other)
{
	if (std::is_signed<DataType>::value != std::is_signed<NewType>::value)
		cg::Logger::LogError("Converting signed rational to unsigned.");
	m_numerator = other.m_numerator;
	m_denominator = other.m_denominator;
}

}
}

#if _DEBUGRATIONAL
template class cg::math::Rational<int>;
#endif