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
#include "StringHelpers.hpp"

#define __FLN__ cg::StringTogether("\n",__FILE__,":",__LINE__,"\n")
#define __FUNCSTR__ cg::StringTogether(__func__,": ")

//TODO:: From String for the rest of the containers/maps/etc...

namespace cg {


/**for const char* and strings. This functions will call whenever an object
needs logged that is a std::string or anything that can be implicitly
converted to one.*/
inline std::string
ToString(const std::string& str)
{
	return str;
}

/**The basic logger class (static).*/
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
