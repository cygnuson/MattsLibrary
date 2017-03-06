#define _CRT_SECURE_NO_WARNINGS
#include "Logger.hpp"

namespace cg {

uint32_t				Logger::_sCurrentLevel = Logger::e_None;
Logger::StreamMap		Logger::_sStreams;
bool					Logger::_sInited = false;
bool					Logger::_sEnabled = true;
bool					Logger::_sShowTime = true;
bool					Logger::_sShowThread = true;
bool					Logger::_sCloneAllStreams = false;
const char				Logger::_sErrorLabel[8] = "ERROR: ";
const char				Logger::_sWarnLabel[8] = " WARN: ";
const char				Logger::_sNoteLabel1[8] = "NOTE1: ";
const char				Logger::_sNoteLabel2[8] = "NOTE2: ";
const char				Logger::_sNoteLabel3[8] = "NOTE3: ";
std::string				Logger::_sPrimaryLogName = "MAINLOG";
std::string				Logger::_sActiveLogName = Logger::_sPrimaryLogName;
std::recursive_mutex	Logger::_sLogMutex;

std::streambuf * Logger::ActiveLog()
{
	std::lock_guard<std::recursive_mutex> lock(_sLogMutex);
	return _sStreams[_sActiveLogName];
}

std::streambuf * Logger::PrimaryLog()
{
	std::lock_guard<std::recursive_mutex> lock(_sLogMutex);
	return _sStreams[_sPrimaryLogName];
}
std::string Logger::CurrentTime()
{
	std::stringstream time;
	if (_sShowTime)
	{
		time << "[";
		std::time_t t = std::time(0);
		std::tm* now = localtime(&t);
		time << std::setw(2) << std::setfill('0') << now->tm_hour;
		time << ":" << std::setw(2) << std::setfill('0') << now->tm_min;
		time << ":" << std::setw(2) << std::setfill('0') << now->tm_sec;
		time << "] " << std::flush;
	}
	return time.str();
}

std::string Logger::CurrentThread()
{
	if (_sShowThread)
	{
		std::stringstream ss;
		ss << "Thread: " << std::setw(6) << std::setfill(' ')
			<< std::this_thread::get_id() << " " << std::flush;
		return ss.str();
	}
	else
		return "";
}

void Logger::Init(
	uint32_t level,
	std::ostream &logStrm,
	bool showTime,
	bool showThread)
{
	std::lock_guard<std::recursive_mutex> lock(_sLogMutex);
	_sCurrentLevel = level;
	_sStreams[_sPrimaryLogName] = logStrm.rdbuf();
	_sShowTime = showTime;
	_sShowThread = showThread;
	_sInited = true;
}

void Logger::ShowTimeStamp(bool set)
{
	_sShowTime = set;
}

void Logger::ShowThread(bool set)
{
	_sShowThread = set;
}

bool Logger::ShowTimeStamp()
{
	return _sShowTime;
}

bool Logger::ShowThread()
{
	return _sShowThread;
}

void Logger::Supress(Level level)
{
	_sCurrentLevel &= ~level;
}

bool Logger::IsInit()
{
	std::lock_guard<std::recursive_mutex> lock(_sLogMutex);
	return _sInited;
}

void Logger::Enable(bool on)
{
	std::lock_guard<std::recursive_mutex> lock(_sLogMutex);
	_sEnabled = on;
}

std::string Logger::PrimaryLogName()
{
	std::lock_guard<std::recursive_mutex> lock(_sLogMutex);
	return _sPrimaryLogName;
}

std::string Logger::ActiveLogName()
{
	std::lock_guard<std::recursive_mutex> lock(_sLogMutex);
	return _sActiveLogName;
}

void Logger::LogAllStreams(bool set)
{
	_sCloneAllStreams = set;
}

bool Logger::AddLog(const std::string & name,
	std::streambuf * logStream)
{
	std::lock_guard<std::recursive_mutex> lock(_sLogMutex);
	bool exists = LogExists(name);
	if (exists)
		return false;
	else
		_sStreams[name] = logStream;
	return true;
}

bool Logger::ChangeLog(const std::string & name)
{
	std::lock_guard<std::recursive_mutex> lock(_sLogMutex);
	bool exists = LogExists(name);
	if (!exists)
		return false;
	else
		_sActiveLogName = name;
	return true;
}

bool Logger::LogExists(const std::string & name)
{
	return _sStreams.count(name) > 0;
}

void Logger::UsePrimaryLog()
{
	std::lock_guard<std::recursive_mutex> lock(_sLogMutex);
	_sActiveLogName = _sPrimaryLogName;
}

bool Logger::sanity_check()
{
	if (!_sInited)
	{
		return false;
	}

	return true;
}

}