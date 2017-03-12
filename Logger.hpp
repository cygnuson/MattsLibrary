#pragma once

#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <sstream>
#include <map>
#include <ctime> 
#include <iomanip>
#include <type_traits>
#include <thread>
#include <cctype>

#include "Colors.hpp"

#define __FLN__ cg::StringTogether("\n",__FILE__,":",__LINE__,"\n")
#define __FUNCSTR__ cg::StringTogether(__func__,": ")

//TODO:: From String for the rest of the containers/maps/etc...

namespace cg {

struct Stringable
{
	virtual std::string ToString() const = 0;
	virtual void FromString(std::string&& str) {};
};

template<typename T>
struct IsStringable
{
	using Type = std::decay_t<T>;
	enum {
		value = std::is_base_of<Stringable, Type>::value,
	};
};


/**Get a string with a thread id.*/
inline std::string ToString(const std::thread::id& id)
{
	std::stringstream ss;
	ss << id;
	return ss.str();
}

/**Convert a stream to a string.*/
inline std::string ToString(std::streambuf* strm)
{
	std::stringstream ss;
	auto strmIt = std::istreambuf_iterator<char>(strm);
	auto strmEnd = std::istreambuf_iterator<char>();
	while (strmIt != strmEnd)
		ss << *(strmIt++);
	return ss.str();
}

/**for const char* and strings. This functions will call whenever an object
needs logged that is a std::string or anything that can be implicitly
converted to one.*/
inline std::string
ToString(const std::string& str)
{
	return str;
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
/*****************************************************For std::shared_pointer*/
template<template<typename> class Ptr, typename T>
std::string ToString(const Ptr<T>& o)
{
	return ToString(*o);
}
/**For non-strings. This function only calls on types that pass the test
IsStringable<type>. Meaning that it must impliment
std::string ToString(bool)
\sa cg::serialization::IsStringable
*/
template<typename T>
std::enable_if_t<
	IsStringable<T>::value, std::string>
	ToString(T&& t)
{
	return t.ToString();
}

/**For non-strings. This function only calls on types that pass the test
IsStringable<type>. Meaning that it must impliment
std::string ToString(bool)
\sa cg::serialization::IsStringable
*/
template<typename T>
std::enable_if_t<IsStringable<T>::value, T>
FromString(const std::string& t)
{
	return t;
}


template<typename T>
inline std::enable_if_t<
	std::is_same<std::string, T>::value, std::string>
	FromString(const std::string& str)
{
	return str;
}

/**For fundamentals. This function will call whenever a fundamental type that
is not implicitly converible is logged.*/
template<typename T>
std::enable_if_t<
	std::is_fundamental<
	std::decay_t<T>>::value, std::string>
	ToString(T&& t)
{
	std::stringstream ss;
	ss << t;
	return ss.str();
}


/**For fundamentals. This function will call whenever a fundamental type that
is not implicitly converible is logged.*/
template<typename T>
std::enable_if_t<
	std::is_fundamental<
	std::decay_t<T>>::value, T>
	FromString(const std::string& t)
{
	std::stringstream ss(t);
	T item;
	ss >> item;
	return item;
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

class Logger
{
public:
	/**Logger levels are intended to be bitwise or`d*/
	enum Level : unsigned int
	{
		e_None = 0,
		e_Note1 = 2, /*For the least important notes.*/
		e_Note2 = 4, /* for the almost important notes.*/
		e_Note3 = 8, /*For the important notes.*/
		e_Warning = 16,
		e_Error = 32,
		e_ErrorWarning = e_Error | e_Warning,
		e_Important = e_Note3 | e_Error | e_Warning, /*exclude e_Note1 and e_Note2*/
		e_Notes = e_Note1 | e_Note2 | e_Note3,
		e_All = e_Note1 | e_Note2 | e_Note3 | e_Warning | e_Error,
	};
private:
	using StreamMap = std::map<std::string, std::streambuf*>;
	static uint32_t             _sCurrentLevel;
	static StreamMap            _sStreams;
	static bool                 _sInited;
	static bool                 _sEnabled;
	static bool	                _sUsingColor;
	static bool                 _sShowTime;
	static bool                 _sSupressNextThreadOutput;
	static bool                 _sSuppressNextTimeOutput;
	static bool                 _sSupressNextLocalOutput;
	static bool                 _sShowThread;
	static bool                 _sCloneAllStreams;
	const static char           _sErrorLabel[8];
	const static char           _sWarnLabel[8];
	const static char           _sNoteLabel1[8];
	const static char           _sNoteLabel2[8];
	const static char           _sNoteLabel3[8];
	static std::string          _sPrimaryLogName;
	static std::string          _sActiveLogName;
	static std::recursive_mutex _sLogMutex;
	static std::streambuf*      _sDefaultCoutBuffer;
	static std::pair<int, int>  _sErrorColor;
	static std::pair<int, int>  _sWarnColor;
	static std::pair<int, int>  _sNote1Color;
	static std::pair<int, int>  _sNote2Color;
	static std::pair<int, int>  _sNote3Color;

	static bool sanity_check();

	template<typename T>
	static void LogHelper(T&& t);
	template<typename T, typename...Args>
	static void LogHelper(T&& t, Args&&...args);

	/**Get the streambuf for the active log.*/
	static std::streambuf* ActiveLog();
	/**Get the streambuf for the Primary log.*/
	static std::streambuf* PrimaryLog();
public:
	/**Initialize the logging system.  Use bitwise OR to have multiple levels.
	EG: Init(Level::note | Level::e_Error,&std::cout)*/
	static void Init(
		uint32_t level,
		std::ostream &primaryLog,
		bool showTime = true,
		bool showThread = true,
		bool useColor = true);
	/**Initialize the logging system.  Use bitwise OR to have multiple levels.
	EG: Init(Level::note | Level::e_Error,&std::cout)*/
	static void Init(
		uint32_t level,
		std::streambuf* primaryLog,
		bool showTime = true,
		bool showThread = true,
		bool useColor = true);
	/**Supress the next thread output only once.*/
	static void SupressNextTimeOutput();
	/**Supress the next time output only once.*/
	static void SupressNextThreadOutput();
	/**Supress the next [LOCAL] output only.*/
	static void SupressNextLocalOutput();
	/**Set the status of the colorization of the text.
	\param use True to use color.*/
	static void UseColor(bool use);
	/**Get the current time.
	\return A string that is the current time.*/
	static std::string CurrentTime();
	/**Get the current thread id.
	\return A string that is the current threads id.*/
	static std::string CurrentThread();
	/**Start a colorized output (lin or win). Color output only works when the
	current log is set to cout.
	\param level The level to colorize.*/
	static void StartColor(cg::Logger::Level level);
	/**End a colorized output.*/
	static void EndColor();
	/**Show to logger timestamp.
	\param set True to show the timestamp.*/
	static void ShowTimeStamp(bool set);
	/**Show to logger threadstamp.
	\param set True to show the threadstamp.*/
	static void ShowThread(bool set);
	/**Determine if the timestamp is being shown or not.
	\return true if the timestamp will be shown.*/
	static bool ShowTimeStamp();
	/**Determine if the threadstamp is being shown or not.
	\return true if the threadstamp will be shown.*/
	static bool ShowThread();
	/**Supress one type of log.
	\param level The level to supress.*/
	static void Supress(Level level);
	/**determine if the Logger is initialized.*/
	static bool IsInit();
	/**Enable the logging temporarily.*/
	static void Enable(bool on);
	/**Get the name of the primary log.
	\return An std::string with the name ofthe primary log.*/
	static std::string PrimaryLogName();
	/**Get the name of the active log.
	\return An std::string with the name ofthe active log.*/
	static std::string ActiveLogName();
	/**Start logging to all the streams in the system.
	\param set True to log to all the streams. False to log only to the active
	log.*/
	static void LogAllStreams(bool set);
	/**Add an existing log to the log map.
	\param logStream A std::streambuff* that points to the stream for this log.
	\param name The name for the log.
	\return True if the log was added, false if it already existed under that
	name.*/
	static bool AddLog(const std::string& name, std::streambuf* logStream);
	/**Change to a different log.
	\param name The name of the log to change to.
	\return True if the log was changed. False if there was no log by that
	name.
	*/
	static bool ChangeLog(const std::string& name);
	/**Determine if a log exists under a name.
	\param name The name to check.
	\return True if the log exists.*/
	static bool LogExists(const std::string& name);
	/**Return the main log.*/
	static void UsePrimaryLog();
	/**Log any amount of things to the Log. ALL objects logged must be one of
	the following:
	A fundamental type,
	A std::string (or convertible to std::string EG. const char*),
	A type that passes the IsStringable<type> test.
	\sa cg::serialization::IsStringable
	*/
	template<typename...Args>
	static void Log(
		Logger::Level level,
		Args&&...args);
	/**Log any amount of things to the Log. ALL objects logged must be one of
	the following:
	A fundamental type,
	A std::string (or convertible to std::string EG. const char*),
	A type that passes the IsStringable<type> test.
	\param cond A boolean that, if true, logging will happen. If false, no log
	will be written.
	\param level The level that the log will appear on.
	\param args Anything that can be stringed and output.
	\sa cg::serialization::IsStringable
	*/
	template<typename...Args>
	static void Log(
		bool cond,
		Logger::Level level,
		Args&&...args);
	/**Log any amount of things to the Log. ALL objects logged must be one of
	the following:
	A fundamental type,
	A std::string (or convertible to std::string EG. const char*),
	A type that passes the IsStringable<type> test.
	\param l The not logging level for the note. e_Note1, e_Note2, or e_Note3
	\param args Anyhting stringable to be output in the log.
	\sa cg::serialization::IsStringable
	*/
	template<typename...Types>
	static void LogNote(
		int l,
		Types&&...args);
	/**Log any amount of things to the Log. ALL objects logged must be one of
	the following:
	A fundamental type,
	A std::string (or convertible to std::string EG. const char*),
	A type that passes the IsStringable<type> test.
	\param l The not logging level for the note. e_Note1, e_Note2, or e_Note3
	\param cond A boolean that, if true, logging will happen. If false, no log
	will be written.
	\param args Anyhting stringable to be output in the log.
	\sa cg::serialization::IsStringable
	*/
	template<typename...Types>
	static void LogNote(
		bool cond,
		int l,
		Types&&...args);
	/**Log any amount of things to the Log. ALL objects logged must be one of
	the following:
	A fundamental type,
	A std::string (or convertible to std::string EG. const char*),
	A type that passes the IsStringable<type> test.
	\param args Anyhting stringable to be output in the log.
	\sa cg::serialization::IsStringable
	*/
	template<typename...Types>
	static void LogError(
		Types&&...args);
	/**Log any amount of things to the Log. ALL objects logged must be one of
	the following:
	A fundamental type,
	A std::string (or convertible to std::string EG. const char*),
	A type that passes the IsStringable<type> test.
	\param cond A boolean that, if true, logging will happen. If false, no log
	will be written.
	\param args Anyhting that is stringable to be output.
	\sa cg::serialization::IsStringable
	*/
	template<typename...Types>
	static void LogError(
		bool cond,
		Types&&...args);
	/**Log any amount of things to the Log. ALL objects logged must be one of
	the following:
	A fundamental type,
	A std::string (or convertible to std::string EG. const char*),
	A type that passes the IsStringable<type> test.
	\param args Anyhting stringable to be output in the log.
	\sa cg::serialization::IsStringable
	*/
	template<typename...Types>
	static void LogWarn(
		Types&&...args);
	/**Log any amount of things to the Log. ALL objects logged must be one of
	the following:
	A fundamental type,
	A std::string (or convertible to std::string EG. const char*),
	A type that passes the IsStringable<type> test.
	\param cond A boolean that, if true, logging will happen. If false, no log
	will be written.
	\param args Anyhting stringable to be output in the log.
	\sa cg::serialization::IsStringable
	*/
	template<typename...Types>
	static void LogWarn(
		bool cond,
		Types&&...args);
};

template<typename T>
void Logger::LogHelper(T&& t)
{
	std::lock_guard<std::recursive_mutex> lock(_sLogMutex);
	std::ostream stream(ActiveLog());
	stream << ToString(std::forward<T>(t));
}
template<typename T, typename...Args>
void Logger::LogHelper(
	T&& t,
	Args&&...args)
{
	std::lock_guard<std::recursive_mutex> lock(_sLogMutex);
	std::ostream stream(ActiveLog());
	stream << ToString(t);
	LogHelper(std::forward<Args>(args)...);
}
template<typename...Args>
void Logger::Log(
	Logger::Level level,
	Args&&...args)
{
	if (!sanity_check() || !_sEnabled)
	{
		/*if the Logger is not in a sane state, do nothing.*/
		return;
	}
	std::lock_guard<std::recursive_mutex> lock(_sLogMutex);
	if ((level & _sCurrentLevel) > 0)
	{
		std::string label;
		switch (level)
		{
		case Logger::Level::e_Warning:
			label += _sWarnLabel;
			break;
		case Logger::Level::e_Error:
			label += _sErrorLabel;
			break;
		case Logger::Level::e_Note1:
			label += _sNoteLabel1;
			break;
		case Logger::Level::e_Note2:
			label += _sNoteLabel2;
			break;
		case Logger::Level::e_Note3:
			label += _sNoteLabel3;
			break;
		default:
			label += "Invalid Level:";
			break;
		}
		if (_sCloneAllStreams)
		{
			/*log to all the current streams.*/
			auto oldLog = ActiveLog();
			for (auto& e : _sStreams)
			{
				ChangeLog(e.first);
				std::ostream stream(ActiveLog());
				std::stringstream ss;
				if (stream.rdbuf() == _sDefaultCoutBuffer && _sUsingColor)
				{
					/*disable cout temproarily so that it will be exclusive to this message.*/
					std::ostream& out = ss;
					std::cout.rdbuf(ss.rdbuf());
					out.rdbuf(_sDefaultCoutBuffer);
					StartColor(level);
				}
				stream << label;
				if (!_sSupressNextLocalOutput)
					stream << "[LOCAL]";
				else
					_sSupressNextLocalOutput = false;
				if (!_sSupressNextThreadOutput)
					stream << CurrentThread();
				else
					_sSupressNextThreadOutput = false;
				if(!_sSuppressNextTimeOutput)
					stream << CurrentTime();
				else
					_sSuppressNextTimeOutput = false;
				LogHelper(std::forward<Args>(args)...);
				stream << std::endl;
				if (stream.rdbuf() == _sDefaultCoutBuffer && _sUsingColor)
				{
					EndColor();
					/*put cout back.*/
					std::cout.rdbuf(_sDefaultCoutBuffer);
					/*write anything that was intented for cout while it was disabled.*/
					std::cout << ss.str();
				}
			}
		}
		else
		{
			/*log only the active stream.*/
			std::ostream stream(ActiveLog());
			std::stringstream ss;
			if (stream.rdbuf() == _sDefaultCoutBuffer && _sUsingColor)
			{
				/*disable cout temproarily so that it will be exclusive to this message.*/
				std::ostream& out = ss;
				std::cout.rdbuf(ss.rdbuf());
				out.rdbuf(_sDefaultCoutBuffer);
				StartColor(level);
			}
			stream << label;
			if (!_sSupressNextLocalOutput)
				stream << "[LOCAL]";
			else
				_sSupressNextLocalOutput = false;
			if (!_sSupressNextThreadOutput)
				stream << CurrentThread();
			else
				_sSupressNextThreadOutput = false;
			if (!_sSuppressNextTimeOutput)
				stream << CurrentTime();
			else
				_sSuppressNextTimeOutput = false;
			LogHelper(std::forward<Args>(args)...);
			stream << std::endl;
			if (stream.rdbuf() == _sDefaultCoutBuffer && _sUsingColor)
			{
				EndColor();
				/*put cout back.*/
				std::cout.rdbuf(_sDefaultCoutBuffer);
				/*write anything that was intented for cout while it was disabled.*/
				std::cout << ss.str();
			}
		}

	}
}

template<typename ...Args>
inline void Logger::Log(
	bool cond,
	Logger::Level level,
	Args && ...args)
{
	if (cond)
		Log(level, std::forward<Args>(args)...);
}

template<typename...Types>
void Logger::LogNote(
	int l,
	Types&&...types)
{
	std::lock_guard<std::recursive_mutex> lock(_sLogMutex);
	switch (l)
	{
	case 1:
		Log(Level::e_Note1, std::forward<Types>(types)...);
		break;
	case 2:
		Log(Level::e_Note2, std::forward<Types>(types)...);
		break;
	case 3:
		Log(Level::e_Note3, std::forward<Types>(types)...);
		break;
	default:
		Log(Level::e_Error, "Tried to Log an invalid note-level.");
	}


}
template<typename ...Types>
inline void Logger::LogNote(
	bool cond,
	int l,
	Types && ...types)
{
	if (cond)
		LogNote(l, std::forward<Types>(types)...);
}
template<typename...Types>
void Logger::LogError(Types&&...types)
{
	std::lock_guard<std::recursive_mutex> lock(_sLogMutex);
	Log(Level::e_Error, std::forward<Types>(types)...);
}
template<typename ...Types>
inline void Logger::LogError(
	bool cond,
	Types && ...types)
{
	if (cond)
		LogError(std::forward<Types>(types)...);
}
template<typename...Types>
void Logger::LogWarn(Types&&...types)
{
	std::lock_guard<std::recursive_mutex> lock(_sLogMutex);
	Log(Level::e_Warning, std::forward<Types>(types)...);
}

template<typename ...Types>
inline void Logger::LogWarn(
	bool cond,
	Types && ...types)
{
	if (cond)
		LogWarn(std::forward<Types>(types)...);
}

}
