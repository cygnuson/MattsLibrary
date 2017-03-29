#pragma once

#include <string>
#include <sstream>
#include <chrono>

namespace cg {


struct Stringable
{
	virtual std::string ToString() const = 0;
};

template<typename T>
struct IsStringable
{
	using Type = std::decay_t<T>;
	enum {
		value = std::is_base_of<Stringable, Type>::value,
	};
};

/**Convert anyhting streamable to a string.
\param t The thing to convert.
\return A string with the parameter.*/
template<typename T>
std::enable_if_t<!IsStringable<T>::value, std::string>
ToString(T&& t)
{
	std::stringstream ss;
	ss << std::forward<T>(t);
	return ss.str();
}

/**Convert anyhting streamable to a string.
\param t The thing to convert.
\return A string with the parameter.*/
template<typename T>
std::enable_if_t<IsStringable<T>::value, std::string>
ToString(const T& t)
{
	return t.ToString();
}



/******************************************containers with only two elements.*/
template<
	template<typename, typename> class Pair,
	typename A,
	typename B
>
std::string ToString(const Pair<A, B>& p)
{
	std::stringstream ss;
	ss << "(" << ToString(p.first) << "," << ToString(p.second) << ")";
	return ss.str();
}
template<
	template<typename, typename> class Pair,
	typename A,
	typename B
>
Pair<A, B> FromString(const std::string& str)
{
	std::stringstream ss(str);
	ss.ignore(); //ignore one
	auto str1 = util::ExtractUntil(ss, ',', true);
	ss.ignore(); //ignore the comma.
	auto str2 = util::ExtractUntil(ss, ')', true);
	Pair<A, B> p;
	ss.clear();
	ss.str(str1);
	ss >> p.first;
	ss.clear();
	ss.str(str2);
	ss >> p.second;
	return p;
}
/***************************************************Container with allocator.*/

template<
	typename T,
	template<typename, typename> class List,
	template<typename> class Alloc
>
std::string ToString(const List<T, Alloc<T>>& o)
{
	std::stringstream ss;
	for (const auto& e : o)
	{
		ss << ToString(e) << ",";
	}
	return ss.str();
}

template<
	typename T,
	template<typename, typename> class List,
	template<typename> class Alloc
>
List<T, Alloc<T>> FromString(const std::string& str)
{
	List<T, Alloc<T>> list;
	std::stringstream ss(str);
	while (ss.good())
	{
		util::SkipUntil<int(int)>(ss, std::isalnum, true);
		auto tStr = util::ExtractUntil(ss, ',', true);
		if (tStr.size() > 0)
			list.push_back(FromString<T>(tStr));
	}
	return list;
}
/*******************************************Containers with alloc and compare*/

template<
	template<typename, typename, typename> class List,
	template<typename> class Comp,
	template<typename> class Alloc,
	typename T
>
std::string ToString(const List<T, Comp<T>, Alloc<T>>& o)
{
	std::stringstream ss;
	for (const auto& e : o)
	{
		ss << ToString(e) << ",";
	}
	return ss.str();
}
/************************************************************************Maps*/
template<
	template<typename, typename, typename, typename> class Map,
	typename Key,
	typename T,
	template<typename> class Comp,
	template<typename> class Alloc,
	template<typename, typename> class Pair
>
std::string ToString(
	const Map<Key, T, Comp<Key>, Alloc<Pair<const Key, T>>>& o)
{
	std::stringstream ss;
	for (const auto& e : o)
	{
		ss << ToString(e);
	}
	return ss.str();
}
/************************************************Unordered sets and multisets*/
template<
	template<typename, typename, typename, typename> class USet,
	typename Key,
	template<typename> class Hasher,
	template<typename> class Pred,
	template<typename> class Alloc
>
std::string ToString(const USet<Key, Hasher<Key>, Pred<Key>, Alloc<Key>>& o)
{
	std::stringstream ss;
	for (const auto& e : o)
	{
		ss << ToString(e) << ",";
	}
	return ss.str();
}
/***************************************************************Unordered Map*/
template<
	template<typename, typename, typename, typename, typename> class Map,
	typename Key,
	typename T,
	template<typename> class Hasher,
	template<typename> class Comp,
	template<typename> class Alloc,
	template<typename, typename> class Pair
>
std::string ToString(
	const Map<Key, T, Hasher<Key>, Comp<Key>,
	Alloc<Pair<const Key, T>>>& o)
{
	std::stringstream ss;
	for (const auto& e : o)
	{
		ss << ToString(e);
	}
	return ss.str();

}
/**************************************************************For std::array*/
template<
	template<typename, std::size_t> class Array,
	typename T,
	std::size_t Size
>
std::string ToString(const Array<T, Size>& o)
{
	std::stringstream ss;
	for (const auto& e : o)
	{
		ss << ToString(e) << ",";
	}
	return ss.str();
}

/*seconds*/
inline std::string ToString(std::chrono::duration<double> time)
{
	std::string str = ToString(time.count());
	str += " s";
	return str;
}
/*milliseconds*/
inline std::string ToString(std::chrono::milliseconds time)
{
	std::string str = ToString(time.count());
	str += " ms";
	return str;
}
/*microSeconds*/
inline std::string ToString(std::chrono::microseconds time)
{
	std::string str = ToString(time.count());
	str += " us";
	return str;
}
/*nano sec*/
inline std::string ToString(std::chrono::nanoseconds time)
{
	std::string str = ToString(time.count());
	str += " ns";
	return str;
}
/*min*/
inline std::string ToString(std::chrono::minutes time)
{
	std::string str = ToString(time.count());
	str += " min";
	return str;
}
/*hr*/
inline std::string ToString(std::chrono::hours time)
{
	std::string str = ToString(time.count());
	str += " hr";
	return str;
}


template<typename Str>
std::string StringTogetherHelper(Str&& str)
{
	return std::string(ToString(str));
}
template<typename...Args, typename Str>
std::string StringTogetherHelper(Str&& str, Args&&...args)
{
	return std::string(ToString(str)) +
		StringTogetherHelper(std::forward<Args>(args)...);
}
template<typename...Args>
std::string StringTogether(Args&&...args)
{
	return StringTogetherHelper(std::forward<Args>(args)...);
}


template<typename Str>
std::string StringTogetherWithSeperatorHelper(Str&& str)
{
	return std::string(ToString(str));
}
template<typename...Args, typename Str>
std::string StringTogetherWithSeperatorHelper(Str&& str, Args&&...args)
{
	return std::string(ToString(str)) + " " +
		StringTogetherWithSeperatorHelper(std::forward<Args>(args)...);
}
template<typename...Args>
std::string StringTogetherWithSeperator(Args&&...args)
{
	return StringTogetherWithSeperatorHelper(std::forward<Args>(args)...);
}

}