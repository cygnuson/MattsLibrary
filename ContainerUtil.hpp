#pragma once

#include <cctype>
#include <istream>
#include <sstream>
#include <csignal>
#include <memory>

#define _FS(x) std::forward<std::string>(x)
#define _IFD if(_DEBUG)


namespace cg {

/**Count the amount of char from one string inside another.
\param str The string to check.
\param lookFor The string to hold the chars to look for.
\param noCase true to ignore the case.
\return The amount of times a char in 'lookFor' exists inside str.*/
inline std::size_t ContainsHowMany(std::string&& str,
	std::string&& lookFor,
	bool noCase)
{
	std::size_t hits = 0;
	for (std::size_t i = 0; str[i] != 0; ++i)
	{
		char strI;
		if (noCase)
			strI = (char) std::tolower(str[i]);
		else
			strI = str[i];

		for (std::size_t a = 0; lookFor[a] != 0; ++a)
		{
			char lookA;
			if (noCase)
				lookA = (char)std::tolower(lookFor[a]);
			else
				lookA = lookFor[a];

			if (strI == lookA)
			{
				++hits;
				break;
			}
		}
	}
	return hits;
}
/**Check if a string only contains certian values.
\param str The string to check.
\param lookFor A string of chars to look for.
\param noCase true to ignore the case.
\return true if str contains only the letters found in lookFor string.*/
inline bool ContainsOnly(std::string&& str,
	std::string&& lookFor,
	bool noCase)
{
	return ContainsHowMany(_FS(str), _FS(lookFor), noCase) == str.size();
}

/**Count how many time a caller returns a certian value in a string.
\param caller The caller that will call on the index of the string.
\param callerRetValue The return value that will register as a positive hit
on the indicies of a string.
\param str The string to count.
\return The amount of hits the caller gets from the string.*/
template<typename Caller>
std::size_t Count(std::string&& str,
	Caller caller,
	bool callerRetValue)
{
	std::size_t hits = 0;
	for (std::size_t i = 0; str[i] != 0; ++i)
	{
		if (((bool)caller(str[i])) == callerRetValue)
			++hits;
	}
	return hits;
}

/**Insert a string into another string before or after every given substring.
\param str The string to be inserting into.
\param substr The sub string to look for.
\param insert The thing to insert.
\param before True to insert before the substring or false to insert after.
\return The amount of insertions made.
*/
inline std::size_t InsertAt(std::string& str,
	std::string&& substr,
	std::string&& insert,
	bool before)
{
	const std::size_t subSize = substr.size();
	const std::size_t inSize = insert.size();
	std::size_t offset = 0;
	std::size_t insertsMade = 0;
	std::stringstream sOut;
	for (std::string::size_type strPos = str.find_first_of(substr, offset);
		strPos != std::string::npos;
		strPos = str.find(substr, offset))
	{
		if (before)
		{
			str.insert(strPos, insert);
		}
		else {
			str.insert(strPos + subSize, insert);
		}
		offset += strPos + subSize + inSize;
		++insertsMade;
	}
	return insertsMade;
}

/**Advance a istream past all X type chars at the front of the stream.
\param strm The istream to advance to the first instance of stopChar.
\param stopChar The char to stop advancing when encountered.
\param keepLast True to keep the stopChar in the stream.
\return The amount of units that were removed from the stream.*/
inline std::size_t SkipUntil(
	std::istream& strm,
	char stopChar,
	bool keepLast = true)
{
	std::size_t hits = 0;
	while (strm.good() && strm.get() != stopChar)
		++hits;
	if (keepLast)
		strm.putback(stopChar);
	return hits;
}
/**Advance a istream past all X type chars at the front of the stream.
\param strm The istream to advance to the first instance of stopChar.
\param stopChars The chars to stop advancing when encountered.
\param keepLast True to keep the stopChar in the stream.
\return The amount of units that were removed from the stream.*/
inline std::size_t SkipUntil(
	std::istream& strm,
	std::string&& stopChars,
	bool keepLast = true)
{
	char ch = 0;
	std::size_t hits = 0;
	while (strm.good())
	{
		ch = (char)strm.get();
		if (stopChars.find(ch) != std::string::npos)
		{
			break;
		}
		++hits;
	}
	if (keepLast)
		strm.putback(ch);
	return hits;
}

/**Advance the stream untill the `pred` parameter callable object returns
`predStop` on a char.
\param strm The istream to advance..
\param pred A callable that returns `predStop` when its time to stop skipping.
\param predStop The value that pred should return when its time to stop.
\return The amount of units that were removed from the stream.*/
template<typename Pred>
inline std::size_t SkipUntil(
	std::istream& strm,
	Pred pred,
	bool predStop)
{
	std::size_t hits = 0;
	while (strm.good() && (pred(strm.peek()) != 0) != predStop)
		strm.ignore();

	return hits;
}

/**Get the contents of a stream up till `pred` returns `predStop` on a char.
\param strm The stream to get from.
\param pred The callable object that will be called with each char.
\param predStop The value that when `pred` returns it, the oepration stopss.
\return A string containing the cahrs that were extracted.*/
template<typename Pred>
std::string ExtractUntil(
	std::istream& strm,
	Pred pred,
	bool predStop)
{
	std::string ret;
	while (strm.good() && (pred(strm.peek()) != 0) != predStop)
		ret += strm.get();
	return ret;
}
/**Extract chars from a stream untill `stopChar` is seen.
\param strm The istream to extract from.
\param stopChar The char to stop extracting when encountered.  The stopChar
will remain in the stream when the function returns.
\param keepLast True to keep the stopChar in the stream.
\return A string that is the chars removed.*/
inline std::string ExtractUntil(
	std::istream& strm,
	char stopChar,
	bool keepLast = true)
{
	std::string ret;
	while (strm.good() && strm.peek() != stopChar)
		ret += (char)strm.get();
	if (!keepLast)
		ret += (char)strm.get();
	return ret;
}

/**Extract chars from a stream untill one of the `stopChars` is seen.
\param strm The istream to extract from.
\param stopChars The char to stop extracting when encountered.  The stopChar
will remain in the stream when the function returns.
\param keepLast True to keep the stopChar in the stream.
\return A string that is the chars removed.*/
inline std::string ExtractUntil(
	std::istream& strm,
	std::string&& stopChars,
	bool keepLast = true)
{
	std::string ret;
	char ch = 0;
	while (strm.good())
	{
		ch = (char)strm.get();
		if (stopChars.find(ch) != std::string::npos)
		{
			strm.putback(ch);
			break;
		}
		ret += ch;
	}
	if (!keepLast)
		ret += (char)strm.get();
	return ret;
}

/**Get the remainter of the stream as a string.
\param strm The stream to get the letters from.
\return The string that is whatever is left in the stream.*/
inline std::string ExtractRest(std::istream& strm)
{
	std::stringstream ss;
	ss << strm.rdbuf();
	return ss.str();
}

/**Determine if a number exists in the string.
\param str The string to work with.
\return True if the string can be an integer.*/
inline bool IsInt(std::string&& str)
{
	if (str.size() == 0)
		return false;
	std::size_t i = 0;
	if (str[i] == '-')
		++i;
	for (; str[i] != 0; ++i)
	{
		auto ch = str[i];
		if (std::isdigit(ch) != 0)
		{
			/*is a digit, do nothing*/
		}
		else
			return false;
	}
	return true;
}
/**Determine if a number exists in the string.
\param str The string to work with.
\return True if the string can be an integer.*/
inline bool IsHex(std::string&& str)
{
	if (str.size() == 0)
		return false;
	std::size_t i = 0;
	if (str[i] == '-')
		++i;
	i += 2;
	for (; str[i] != 0; ++i)
	{
		auto ch = str[i];
		if (std::isdigit(ch) != 0 ||
			std::tolower(ch) == 'a' ||
			std::tolower(ch) == 'b' ||
			std::tolower(ch) == 'c' ||
			std::tolower(ch) == 'd' ||
			std::tolower(ch) == 'e' ||
			std::tolower(ch) == 'f')
		{
			/*is a digit, do nothing*/
		}
		else
			return false;
	}
	return true;
}
/**Determine if a number exists in the string.
\param str The string to work with.
\return True if the string can be a double or float.*/
inline bool IsDouble(std::string&& str)
{
	if (str.size() == 0)
		return false;
	std::size_t i = 0;
	if (str[i] == '-')
		++i;
	for (; str[i] != 0; ++i)
	{
		auto ch = str[i];
		if (std::isdigit(ch) != 0 || ch == '.')
		{
			/*is a digit, do nothing*/
		}
		else
			return false;
	}
	return true;
}

enum class NumberType
{
	Dec,
	Hex,
	Oct,
};

/**The template unspecialized.
\param str The string to work with.
\param numType The number type that the string is in.
\return The string*/
template<typename T>
T NumFromString(std::string&& str,
	NumberType numType = NumberType::Dec)
{
	if (!IsInt(std::forward<std::string>(str)) &&
		!IsDouble(std::forward<std::string>(str)) &&
		!IsHex(std::forward<std::string>(str)))
	{
		return T();
	}
	std::stringstream ss(std::forward<std::string>(str));
	T t;
	switch (numType)
	{
	case cg::util::NumberType::Dec:
		ss >> std::dec;
	case cg::util::NumberType::Hex:
		ss >> std::hex;
	case cg::util::NumberType::Oct:
		ss >> std::oct;
	default:
		ss >> std::dec;
	}
	ss >> t;
	return t;
}

/****************************VECTOR PUSH FRONT****************************************************/
template<template<typename, typename> typename C,
	typename Type1, typename...T>
	void PushFrontHelper(C<Type1, std::allocator<Type1>>& intake,
		const C<Type1, std::allocator<Type1>>& toInsert,
		const C<T, std::allocator<T>>&...vecs)
{
	intake.insert(intake.begin(), toInsert.begin(), toInsert.end());
	PushFrontHelper(intake, vecs...);
}
template<template<typename, typename> typename C,
	typename T>
	void PushFrontHelper(C<T, std::allocator<T>>& intake,
		const C<T, std::allocator<T>>& toInsert)
{
	intake.insert(intake.begin(), toInsert.begin(), toInsert.end());
}
template<template<typename, typename> typename C,
	typename Type1, typename...T>
	void PushFront(C<Type1, std::allocator<Type1>>& intake,
		const C<T, std::allocator<T>>&...vecs)
{
	PushFrontHelper(intake, vecs...);
}
/*******************************OBJECT PUSH FRONT*************************************************/
template<template<typename, typename> typename C,
	typename Type1, typename...T>
	void PushFrontHelper(C<Type1, std::allocator<Type1>>& intake,
		const Type1& toInsert,
		const T&...nums)
{
	intake.insert(intake.begin(), toInsert);
	PushFrontHelper(intake, nums...);
}
template<template<typename, typename> typename C,
	typename T>
	void PushFrontHelper(C<T, std::allocator<T>>& intake,
		const T& toInsert)
{
	intake.insert(intake.begin(), toInsert);
}
template<template<typename, typename> typename C,
	typename Type1, typename...T>
	void PushFront(C<Type1, std::allocator<Type1>>& intake,
		const T&...nums)
{
	PushFrontHelper(intake, nums...);
}
/**********************************VECTOR PUSH BACK***********************************************/
template<template<typename, typename> typename C,
	typename Type1, typename...T>
	void PushBackHelper(C<Type1, std::allocator<Type1>>& intake,
		const C<Type1, std::allocator<Type1>>& toInsert,
		const C<T, std::allocator<T>>&...vecs)
{
	intake.insert(intake.end(), toInsert.begin(), toInsert.end());
	PushBackHelper(intake, vecs...);
}
template<template<typename, typename> typename C,
	typename T>
	void PushBackHelper(C<T, std::allocator<T>>& intake,
		const C<T, std::allocator<T>>& toInsert)
{
	intake.insert(intake.end(), toInsert.begin(), toInsert.end());
}
template<template<typename, typename> typename C,
	typename Type1, typename...T>
	void PushBack(C<Type1, std::allocator<Type1>>& intake,
		const C<T, std::allocator<T>>&...vecs)
{
	PushBackHelper(intake, vecs...);
}
/****************************************OBJECT PUSH BACK*****************************************/
template<template<typename, typename> typename C,
	typename Type1, typename...T>
	void PushBackHelper(C<Type1, std::allocator<Type1>>& intake,
		const Type1& toInsert,
		const T&...nums)
{
	intake.insert(intake.end(), toInsert);
	PushBackHelper(intake, nums...);
}
template<template<typename, typename> typename C,
	typename T>
	void PushBackHelper(C<T, std::allocator<T>>& intake,
		const T& toInsert)
{
	intake.insert(intake.end(), toInsert);
}
template<template<typename, typename> typename C,
	typename Type1, typename...T>
	void PushBack(C<Type1, std::allocator<Type1>>& intake,
		const T&...nums)
{
	PushBackHelper(intake, nums...);
}
/**********************************************VECTOR INSERT**************************************/
template<template<typename, typename> typename C,
	typename Type1, typename...T>
	void InsertHelper(C<Type1, std::allocator<Type1>>& intake,
		std::size_t index,
		const C<Type1, std::allocator<Type1>>& toInsert,
		const C<T, std::allocator<T>>&...vecs)
{
	intake.insert(intake.begin() + index, toInsert.begin(), toInsert.end());
	InsertHelper(intake, index, vecs...);
}
template<template<typename, typename> typename C,
	typename T>
	void InsertHelper(C<T, std::allocator<T>>& intake,
		std::size_t index,
		const C<T, std::allocator<T>>& toInsert)
{
	intake.insert(intake.begin() + index, toInsert.begin(), toInsert.end());
}
template<template<typename, typename> typename C,
	typename Type1, typename...T>
	void Insert(C<Type1, std::allocator<Type1>>& intake,
		std::size_t index,
		const C<T, std::allocator<T>>&...vecs)
{
	InsertHelper(intake, index, vecs...);
}
/************************************OBJECT INSERT************************************************/
template<template<typename, typename> typename C,
	typename Type1, typename...T>
	void InsertHelper(C<Type1, std::allocator<Type1>>& intake,
		std::size_t index,
		const Type1& toInsert,
		const T&...nums)
{
	intake.insert(intake.begin() + index, toInsert);
	InsertHelper(intake, index, nums...);
}
template<template<typename, typename> typename C,
	typename T>
	void InsertHelper(C<T, std::allocator<T>>& intake,
		std::size_t index,
		const T& toInsert)
{
	intake.insert(intake.begin() + index, toInsert);
}
template<template<typename, typename> typename C,
	typename Type1, typename...T>
	void Insert(C<Type1, std::allocator<Type1>>& intake,
		std::size_t index,
		const T&...nums)
{
	InsertHelper(intake, index, nums...);
}
/******************************************************************************Equality assist****/
template<
	typename T,
	template<typename, typename> class List,
	template<typename> class Alloc
>
bool Equals(const List<T, Alloc<T>>& lhs,
	const List<T, Alloc<T>>& rhs)
{
	if (lhs.size() != rhs.size())
		return false;
	std::size_t size = lhs.size();
	for (std::size_t i = 0; i < size; ++i)
	{
		if (lhs[i] != rhs[i])
			return false;
	}
	return true;
}

template<
	template<typename, typename> class Pair,
	typename A,
	typename B
>
bool Equals(const Pair<A, B>& lhs,
	const Pair<A, B>& rhs)
{
	return lhs.first == rhs.first && lhs.second == rhs.second;
}

/*******************************************************************************UNIT SIZES********/


}



