#pragma once

#include "Object.hpp"

#define _DEBUGNUMBER _DEBUG && 1

namespace cg {
namespace math {

template<typename DataType>
class Number : public cg::math::Object
{
public:
	/**Create a number with an initial value.
	\param val The initial value.*/
	Number(const DataType& val);
	/**Create a number with no initial value.
	\param pow The power of the number (optional).*/
	Number(Object* pow = nullptr);
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
		return Object::Type::Number;
	}

	Number<DataType>& operator +=(const Number<DataType>& other);
	Number<DataType>& operator -=(const Number<DataType>& other);
	Number<DataType>& operator *=(const Number<DataType>& other);
	Number<DataType>& operator /=(const Number<DataType>& other);
	Number<DataType>& operator %=(const Number<DataType>& other);
	Number<DataType> operator +(const Number<DataType>& other)const;
	Number<DataType> operator -(const Number<DataType>& other)const;
	Number<DataType> operator *(const Number<DataType>& other)const;
	Number<DataType> operator /(const Number<DataType>& other)const;
	Number<DataType> operator %(const Number<DataType>& other)const;

	Number<DataType>& operator++();
	Number<DataType> operator++(int);
	Number<DataType>& operator--();
	Number<DataType> operator--(int);

	Number<DataType>& operator<<=(std::size_t amt);
	Number<DataType>& operator>>=(std::size_t amt);
	Number<DataType> operator<<(std::size_t amt) const;
	Number<DataType> operator >> (std::size_t amt)const;

	bool operator<(const Number<DataType>& other) const;
	bool operator<=(const Number<DataType>& other) const;
	bool operator>(const Number<DataType>& other) const;
	bool operator>=(const Number<DataType>& other) const;
	bool operator==(const Number<DataType>& other) const;
	bool operator!=(const Number<DataType>& other) const;

	/**The internal number.*/
	DataType m_data;
	/**The power*/
	std::shared_ptr<Object> m_power;

};

template<typename DataType>
inline Number<DataType>::Number(const DataType & val)
	:m_data(val) {}

template<typename DataType>
inline Number<DataType>::Number(Object* pow)
{
	if (pow)
		m_power = std::shared_ptr<Object>(pow);
}

template<typename DataType>
inline std::string Number<DataType>::ToString() const
{
	if (m_power)
		return std::string("(") + cg::ToString(m_data) + ")^["
		+ m_power->ToString() + "]";
	else
		return std::string("(") + cg::ToString(m_data) + ")";
}

template<typename DataType>
inline bool Number<DataType>::IsOne() const
{
	return m_data == 1;
}

template<typename DataType>
inline bool Number<DataType>::IsZero() const
{
	return m_data == 0;
}


template<typename DataType>
inline Number<DataType>&
Number<DataType>::operator+=(const Number<DataType> & other)
{
	m_data += other.m_data;
	return *this;
}

template<typename DataType>
inline Number<DataType>&
Number<DataType>::operator-=(const Number<DataType> & other)
{
	m_data -= other.m_data;
	return *this;
}

template<typename DataType>
inline Number<DataType>&
Number<DataType>::operator*=(const Number<DataType> & other)
{
	m_data *= other.m_data;
	return *this;
}

template<typename DataType>
inline Number<DataType>&
Number<DataType>::operator/=(const Number<DataType> & other)
{
	m_data /= other.m_data;
	return *this;
}

template<typename DataType>
inline Number<DataType>&
Number<DataType>::operator%=(const Number<DataType> & other)
{
	m_data %= other.m_data;
	return *this;
}

template<typename DataType>
inline Number<DataType>
Number<DataType>::operator+(const Number<DataType> & other) const
{
	auto copy = *this;
	return copy += other;
}

template<typename DataType>
inline Number<DataType>
Number<DataType>::operator-(const Number<DataType> & other) const
{
	auto copy = *this;
	return copy -= other;
}

template<typename DataType>
inline Number<DataType>
Number<DataType>::operator*(const Number<DataType> & other) const
{
	auto copy = *this;
	return copy *= other;
}

template<typename DataType>
inline Number<DataType>
Number<DataType>::operator/(const Number<DataType> & other) const
{
	auto copy = *this;
	return copy /= other;
}

template<typename DataType>
inline Number<DataType>
Number<DataType>::operator%(const Number<DataType> & other) const
{
	auto copy = *this;
	return copy %= other;
}

template<typename DataType>
inline Number<DataType>& Number<DataType>::operator++()
{
	return *this += 1;
}

template<typename DataType>
inline Number<DataType> Number<DataType>::operator++(int)
{
	auto copy = *this;
	++(*this);
	return copy;
}

template<typename DataType>
inline Number<DataType>& Number<DataType>::operator--()
{
	return *this -= 1;
}

template<typename DataType>
inline Number<DataType> Number<DataType>::operator--(int)
{
	auto copy = *this;
	--(*this);
	return copy;
}

template<typename DataType>
inline Number<DataType>& Number<DataType>::operator<<=(std::size_t amt)
{
	m_data <<= amt;
	return *this;
}

template<typename DataType>
inline Number<DataType>& Number<DataType>::operator>>=(std::size_t amt)
{
	m_data >>= amt;
	return *this;
}

template<typename DataType>
inline Number<DataType> Number<DataType>::operator<<(std::size_t amt)const
{
	auto copy = *this;
	return copy <<= amt;
}

template<typename DataType>
inline Number<DataType> Number<DataType>::operator >> (std::size_t amt)const
{
	auto copy = *this;
	return copy >>= amt;
}

template<typename DataType>
inline bool Number<DataType>::operator<(const Number<DataType>& other) const
{
	return m_data < other.m_data;
}

template<typename DataType>
inline bool Number<DataType>::operator<=(const Number<DataType>& other) const
{
	return *this < other || *this == other;
}

template<typename DataType>
inline bool Number<DataType>::operator>(const Number<DataType>& other) const
{
	return !(*this <= other);
}

template<typename DataType>
inline bool Number<DataType>::operator>=(const Number<DataType>& other) const
{
	return !(*this < other);
}

template<typename DataType>
inline bool Number<DataType>::operator==(const Number<DataType>& other) const
{
	return m_data == other.m_data;
}

template<typename DataType>
inline bool Number<DataType>::operator!=(const Number<DataType>& other) const
{
	return !(*this == other);
}


}
}

#if _DEBUGNUMBER
template class cg::math::Number<int>;
#endif