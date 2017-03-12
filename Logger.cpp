#define _CRT_SECURE_NO_WARNINGS
#include "Logger.hpp"

namespace cg {

uint32_t				Logger::_sCurrentLevel = Logger::e_None;
Logger::StreamMap		Logger::_sStreams;
bool					Logger::_sInited = false;
bool					Logger::_sEnabled = true;
bool					Logger::_sUsingColor = true;
bool					Logger::_sSuppressNextTimeOutput = false;
bool					Logger::_sSupressNextThreadOutput = false;
bool					Logger::_sSupressNextLocalOutput = false;
bool					Logger::_sShowTime = true;
bool					Logger::_sShowThread = true;
bool					Logger::_sCloneAllStreams = false;
const char				Logger::_sErrorLabel[8] = "ERROR: ";
const char				Logger::_sWarnLabel[8] = " WARN: ";
const char				Logger::_sNoteLabel1[8] = "NOTE1: ";
const char				Logger::_sNoteLabel2[8] = "NOTE2: ";
const char				Logger::_sNoteLabel3[8] = "NOTE3: ";
std::string				Logger::_sPrimaryLogName = "MAINLOG";
std::string				Logger::_sActiveLogName = _sPrimaryLogName;
std::recursive_mutex	Logger::_sLogMutex;
std::streambuf*         Logger::_sDefaultCoutBuffer = std::cout.rdbuf();
std::pair<int, int>     Logger::_sErrorColor =
{(int)colors::ForegroundColor::Red , (int)colors::BackgroundColor::Black};
std::pair<int, int>     Logger::_sWarnColor =
{ (int)colors::ForegroundColor::Yellow , (int)colors::BackgroundColor::Black };
std::pair<int, int>     Logger::_sNote1Color =
{ (int)colors::ForegroundColor::Cyan , (int)colors::BackgroundColor::Black };
std::pair<int, int>     Logger::_sNote2Color =
{ (int)colors::ForegroundColor::Cyan , (int)colors::BackgroundColor::Black };
std::pair<int, int>     Logger::_sNote3Color =
{ (int)colors::ForegroundColor::Cyan , (int)colors::BackgroundColor::Black };

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
		ss << "[Thread:" << std::setw(6) << std::setfill(' ')
			<< std::this_thread::get_id() << "]" << std::flush;
		return ss.str();
	}
	else
		return "";
}

void Logger::UseColor(bool use)
{
	_sUsingColor = use;
}

void Logger::StartColor(cg::Logger::Level level)
{
	/*only change if using cout.*/
	if (ActiveLog() != _sDefaultCoutBuffer)
		return;
	int fore = 0;
	int back = 0;
	if (level == Level::e_Note1)
	{
		fore = _sNote1Color.first;
		back = _sNote1Color.second;
	}
	if (level == Level::e_Note2)
	{
		fore = _sNote2Color.first;
		back = _sNote2Color.second;
	}
	if (level == Level::e_Note3)
	{
		fore = _sNote3Color.first;
		back = _sNote3Color.second;
	}
	if (level == Level::e_Warning)
	{
		fore = _sWarnColor.first;
		back = _sWarnColor.second;
	}
	if (level == Level::e_Error)
	{
		fore = _sErrorColor.first;
		back = _sErrorColor.second;
	}
#ifdef _WIN32
	unsigned int col = fore | back;
	if (!SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), col))
	{
		/*could not set*/
	}
#else
	/*linux color setting*/
#endif
}

void Logger::EndColor()
{

#ifdef _WIN32
	unsigned int col = (unsigned int)colors::BackgroundColor::Black |
		(unsigned int)colors::ForegroundColor::White;
	if (!SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), col))
	{
		/*could not set*/
	}
#else
	/*linux color setting*/
#endif
}

void Logger::Init(
	uint32_t level,
	std::ostream &logStrm,
	bool showTime,
	bool showThread,
	bool useColor)
{
	std::lock_guard<std::recursive_mutex> lock(_sLogMutex);
	_sCurrentLevel = level;
	_sStreams[_sPrimaryLogName] = logStrm.rdbuf();
	_sShowTime = showTime;
	_sShowThread = showThread;
	UseColor(useColor);
	_sInited = true;
}
void Logger::Init(
	uint32_t level,
	std::streambuf* logStrm,
	bool showTime,
	bool showThread,
	bool useColor)
{
	std::lock_guard<std::recursive_mutex> lock(_sLogMutex);
	_sCurrentLevel = level;
	_sStreams[_sPrimaryLogName] = logStrm;
	_sShowTime = showTime;
	_sShowThread = showThread;
	UseColor(useColor);
	_sInited = true;
}

void Logger::SupressNextTimeOutput()
{
	_sSuppressNextTimeOutput = true;
}

void Logger::SupressNextThreadOutput()
{
	_sSupressNextThreadOutput = true;
}

void Logger::SupressNextLocalOutput()
{
	_sSupressNextLocalOutput = true;
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