/*

(C) Matthew Swanson

This file is part of ColdStorage.

ColdStorage is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
any later version.

ColdStorage is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with ColdStorage.  If not, see <http://www.gnu.org/licenses/>.

*/
#pragma once

#include "cgdef.hpp"

namespace cg {

/**Exceptions for iterators.*/
enum class ArrayIteratorException
{
	/**The iterator was not dereferencable.*/
	NotDereferenceable,

};

/**A basic array iterator.
\tparam DataType The type of data.
\tparam Const True for a const iterator.
\tparam Reverse True to move in reverese direction.*/
template<typename DataType, bool Const, bool Reverse>
class ArrayIterator {
public:
	/**The type of data reference to return.*/
	using Ref = ConditionalType<Const, const DataType& ,DataType&>;
	/**The type of data reference to return.*/
	using Ptr = ConditionalType<Const, const DataType* ,DataType*>;
	/**The type of this object.*/
	using SelfType = ArrayIterator<DataType, Const, Reverse>;

	/**Cosntruct an uninitialized iterator.
	\post The iterator is NOT in a usable state.*/
	ArrayIterator() :m_ptr(nullptr) {}

	ArrayIterator(Ptr p);

	ArrayIterator(const SelfType& other);

	SelfType& operator=(const SelfType& other);

	ArrayIterator(SelfType&& other);

	SelfType& operator=(SelfType&& other);

	Ref operator*();

	const Ref operator*() const;

	Ptr operator->();

	const Ptr operator->() const;

	SelfType operator++(int);

	SelfType& operator++();

	SelfType operator--(int);

	SelfType& operator--();

	bool operator!=(const SelfType& other) const;

	bool operator==(const SelfType& other) const;

	bool operator<(const SelfType& other)const;

	bool operator<=(const SelfType& other)const;

	bool operator>(const SelfType& other)const;

	bool operator>=(const SelfType& other)const;

	SelfType operator+(const SizeType& amt) const;

	SelfType& operator+=(const SizeType& amt);

	SelfType operator-(const SizeType& amt) const;

	DiffType operator-(const SelfType& it) const;

	SelfType& operator-=(const SizeType& amt);

	Ref operator[](const DiffType& i);

	const Ref operator[](const DiffType& i) const;

    Ptr Addr();

    const Ptr Addr() const;
private:
	void CheckAndThrow() const;
	/**The iterators current location*/
	Ptr m_ptr;

};

/**Allow N + I where N is a number and I is an iterator.
\pre \p i must be initialized and point to some valid memory.
\param n Some positive number.
\param i An initialized iterator that points to valid memory.
\return A copy of \p i after adjustment.*/
template<typename DataType, bool Const, bool Reverse>
auto operator+(SizeType n, const ArrayIterator<DataType, Const, Reverse>& i)
{
	auto copy = i;
	copy += n;
	return copy;
}


}