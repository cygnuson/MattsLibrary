#pragma once

#include "../LogAdaptor.hpp"
#include "Helpers.hpp"

#define _DEBUGRATIONAL _DEBUG && 1

namespace cg {
namespace math {

template<typename DataType>
class Rational : public cg::LogAdaptor<Rational<DataType>>
{
public:
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
	/**Get a copy of this rational but as a reciprocal.
	\return A rational that is the reciprocol of this number.*/
	Rational<DataType> Reciprocal() const;
	/**Get a string version of this rational.
	\return A string rep of this object.*/
	std::string ToString() const;
private:
	using cg::LogAdaptor<Rational<DataType>>::EnableLogs;
	using cg::LogAdaptor<Rational<DataType>>::LogNote;
	using cg::LogAdaptor<Rational<DataType>>::LogWarn;
	using cg::LogAdaptor<Rational<DataType>>::LogError;
	using cg::LogAdaptor<Rational<DataType>>::Log;
	using cg::LogAdaptor<Rational<DataType>>::ms_log;
	using cg::LogAdaptor<Rational<DataType>>::ms_name;
	/**Make this object into its reciprical.*/
	void MakeReciprocal();
	/**Cause the rational to simplify.*/
	void Simplify();
	/**Set a numerator and denominator.
	\param numerator The top of the rational unit.
	\param denominator The bottom of the rational unit.*/
	void Init(const DataType& numerator, const DataType& denominator);
	/**A unit of DataType that will be the numerator*/
	DataType m_numerator;
	/**A unit of DataType that will be the denominator.*/
	DataType m_denominator;
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
inline void Rational<DataType>::MakeReciprocal()
{
	auto X = m_numerator;
	m_numerator = m_denominator;
	m_denominator = X;
}

template<typename DataType>
inline void Rational<DataType>::Simplify()
{
	if (m_denominator == m_numerator)
	{
		m_denominator = 1;
		m_numerator = 1;
		return;
	}
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
		auto GCD = cg::math::GCD(m_numerator, m_denominator);
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

}
}

#if _DEBUGRATIONAL
template class cg::math::Rational<int>;
#endif