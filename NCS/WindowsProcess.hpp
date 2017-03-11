#pragma once

#ifdef _WIN32

#include <array>
#include <thread>
#include <future>

#include <Windows.h>

#include "../LogAdaptor.hpp"
#include "../MasterLock.hpp"
#include "IProcess.hpp"

namespace cg {

/**The Windows Command System is designed to streamline the usage of the
windows API for created calling of other processes with properties, output
and input sources and other settings and properties the winapi allows.*/
class Process : public IProcess, public cg::LogAdaptor<Process>
{
public:
	/**Alian for windows objects.*/
	using Handle = HANDLE;
	/**Alian for windows objects.*/
	using StartupInfo = STARTUPINFO;
	/**Alian for windows objects.*/
	using ProcessInfo = PROCESS_INFORMATION;
	/**Alian for windows objects.*/
	using SecurityAttribuites = SECURITY_ATTRIBUTES;
	/**Create an empty process.
	\param path The process name and path.*/
	Process(const std::string& path = "");
	/**Close the process.*/
	~Process();
	/**Create a string from the last occured windows error.
	\return A string that is the last occuring error in windows.*/
	static std::string LastError();
	/**Push some input to the child.
	\param str The input to insert to the child.*/
	virtual void InsertInput(const std::string& in);
	/**Start a process.
	\param args The args to send along as space seperated strings.
	\param updateFPS The amount of times that the input and output will update.
	Both input and output frames will operate at this fps.
	\return True if the process was started properly.*/
	virtual void Start(const std::string& args = "",
		double updateFPS = 10);
	/**Forward athe output.*/
	virtual void UpdateOutput();
	/**Forward the input.*/
	virtual void UpdateInput();
	/**Get the exit code of the running process. Will wait if not finished.
	\return The exit code of the process.*/
	virtual int Result();
	/**Kill the process immediatly.*/
	virtual void Kill();
private:
	using cg::LogAdaptor<Process>::EnableLogs;
	using cg::LogAdaptor<Process>::LogNote;
	using cg::LogAdaptor<Process>::LogWarn;
	using cg::LogAdaptor<Process>::LogError;
	using cg::LogAdaptor<Process>::Log;
	using cg::LogAdaptor<Process>::ms_log;
	using cg::LogAdaptor<Process>::ms_name;
	/**Bind the pipes to the input and outputs.*/
	void BindPipes();
	/**Close all the pipes.*/
	void ClosePipes();
	/**Holds the info for the windows process.*/
	ProcessInfo m_procInfo;
	/**Holds info on how to start the process.*/
	StartupInfo m_startInfo;
	/**The security attributes of the process.*/
	SecurityAttribuites m_secAttrib;
	/**Read data from the stout of the child.*/
	Handle m_readFromChildStdOut;
	/**Write data to the childs stdin.*/
	Handle m_writeToChildStdIn;
	/**write data to the stdout of the child (write to the childs window).*/
	Handle m_writeToChildStdOut;
	/**read data from the stdin (what is typed into the childs window).*/
	Handle m_readFromChildStdIn;

};



}


#endif




