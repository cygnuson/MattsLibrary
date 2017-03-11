#include "WindowsProcess.hpp"

namespace cg {

Process::Process(const std::string & path)
{
	if (path.size() > 0)
	{
		m_procPath = path;
		auto slash = path.find_last_of('/');
		if (slash == std::string::npos)
			slash = path.find_last_of('\\');
		if (slash != std::string::npos)
			EnableLogs(true, "Process[" + path.substr(slash + 1) + "]");
		else
			EnableLogs(true, "Process[" + path + "]");
	}
	std::memset(&m_startInfo, 0, sizeof(m_startInfo));
	std::memset(&m_procInfo, 0, sizeof(m_procInfo));
	std::memset(&m_secAttrib, 0, sizeof(m_secAttrib));
	m_secAttrib.nLength = sizeof(SecurityAttribuites);
	m_secAttrib.bInheritHandle = TRUE;
	m_secAttrib.lpSecurityDescriptor = NULL;
	LogNote(2, "Log initialized.");
	BindPipes();
}

Process::~Process()
{
	UpdateOutput();
	UpdateInput();
	ClosePipes();
}

void Process::Start(const std::string & args, double updateFPS)
{
	m_running = true;
	if (m_procPath.size() == 0)
	{
		/*TODO: Make cg::Exception*/
		throw std::runtime_error("Process path not set");
	}
	auto finalProc = m_procPath;
	if (args[0] != ' ')
		finalProc += ' ';
	finalProc += args;
	LPSTR cmd = &finalProc[0];
	auto created = CreateProcess(0, cmd,
		0, 0, TRUE,
		0, 0, 0, &m_startInfo, &m_procInfo) == TRUE;
	if (!created)
	{
		auto error = LastError();
		LogError(__FUNCSTR__, "The process did not run. Error: ", error);
		/*TODO: Replace with cg::Exception*/
		throw std::runtime_error("Process did not run.");
	}
	AsyncUpdateInput(updateFPS);
	AsyncUpdateOutput(updateFPS);
}

void Process::UpdateOutput()
{
	if (!m_running)
		return;
	/*Get from the child and put into the right buffer.*/
	DWORD read = 0;
	std::array<char, 1024 * 16> readBuffer = {};
	ml_fromChildOutput.Lock();
	DWORD size = (DWORD)readBuffer.size() - 2;
	DWORD ready = 0;
	/*dont read if there is nothing ready.*/
	PeekNamedPipe(m_readFromChildStdOut, 0, 0, 0, &ready, 0);
	while (ready != 0)
	{
		auto readResult = ReadFile(m_readFromChildStdOut, readBuffer.data(),
			size, &read, NULL) == TRUE;
		if (!readResult)
		{
			LogError(__FUNCSTR__, "There was an error: ", LastError());
			break;
		}
		std::string str(readBuffer.data());
		m_childOutputBuffer << str;
		readBuffer = { 0 };
		/*check if more became ready.*/
		PeekNamedPipe(m_readFromChildStdOut, 0, 0, 0, &ready, 0);
		if (read == 0)
			break;
	}
	ml_fromChildOutput.Unlock();
}

void Process::UpdateInput()
{
	/*If there is nothing to write, do nothing*/
	if (!m_childInputBuffer.good())
		return;
	if (!m_running)
		return;

	DWORD written = 0;
	/*now send the child some input, but dont include the nullterm in the
	string.*/
	while (m_childInputBuffer.good())
	{
		std::stringstream::int_type ch = m_childInputBuffer.get();
		if (ch != std::stringstream::traits_type::eof())
		{
			bool writeSuccess = WriteFile(m_writeToChildStdIn, &ch,
				1, &written, NULL) == TRUE;
			if (!writeSuccess)
				LogError("Could not write to the child.");
			if (written == 0)
				break;
		}
	}
}

void Process::BindPipes()
{
	bool stdinPipeSuccess =
		CreatePipe(&m_readFromChildStdIn, &m_writeToChildStdIn,
			&m_secAttrib, 0) == TRUE;
	if (!stdinPipeSuccess)
	{
		LogError(__FUNCSTR__, "Could not bind the stdin pipe.", __FLN__);
		return;
	}

	bool handleInfoInSucces =
		SetHandleInformation(m_writeToChildStdIn, HANDLE_FLAG_INHERIT, 0)
		== TRUE;
	if (!handleInfoInSucces)
	{
		LogError(__FUNCSTR__, "Could not get the in handle info.", __FLN__);
		return;
	}

	bool stdoutPipeSuccess =
		CreatePipe(&m_readFromChildStdOut,
			&m_writeToChildStdOut, &m_secAttrib, 0) == TRUE;
	if (!stdoutPipeSuccess)
	{
		LogError(__FUNCSTR__, "Could not bind the stdout pipe.", __FLN__);
		return;
	}

	bool handleInfoOutSucces =
		SetHandleInformation(m_readFromChildStdOut, HANDLE_FLAG_INHERIT, 0)
		== TRUE;
	if (!handleInfoOutSucces)
	{
		LogError(__FUNCSTR__, "Could not get the out handle info.", __FLN__);
		return;
	}

	m_startInfo.cb = sizeof(StartupInfo);
	m_startInfo.hStdOutput = m_writeToChildStdOut;
	m_startInfo.hStdInput = m_readFromChildStdIn;
	m_startInfo.hStdError = m_writeToChildStdOut;
	m_startInfo.dwFlags |= STARTF_USESTDHANDLES;

	LogNote(2, "Successfully bound all pipes.");
}

void Process::ClosePipes()
{
	CloseHandle(m_readFromChildStdIn);
	LogNote(2, __FUNCSTR__, "Closed the `Read from child stdin` pipe.");
	CloseHandle(m_writeToChildStdIn);
	LogNote(2, __FUNCSTR__, "Closed the `write to child stdin` pipe.");
	CloseHandle(m_writeToChildStdOut);
	LogNote(2, __FUNCSTR__, "Closed the `write to child stdout` pipe.");
	CloseHandle(m_readFromChildStdOut);
	LogNote(2, __FUNCSTR__, "Closed the `Read from child stdout` pipe.");
}


std::string Process::LastError()
{
	//point to the message
	char* errorMessage = nullptr;
	auto errorCode = GetLastError();
	//format the message to a readable string.
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		0,
		errorCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&errorMessage,
		0,
		0);
	//save it
	std::string error(errorMessage);
	//windows wants to free it on its own.
	LocalFree(errorMessage);
	return error;
}

void Process::InsertInput(const std::string & in)
{
	ml_toChildInput.LockGuard();
	m_childInputBuffer << in;
	m_childInputBuffer << "\r\n";
}

int Process::Result()
{
	int exitCode = 0;
	bool gotExitCode = false;
	int waitResult = WAIT_TIMEOUT;
	/*wiat for the process to stop.  Will timeout and restart in order
	to prevent it from hanging in the event that the process is terminated
	prematurly.*/
	while (waitResult == WAIT_TIMEOUT)
	{
		waitResult =
			WaitForSingleObject(m_procInfo.hProcess, (DWORD)50);
	}
	if (GetExitCodeProcess(m_procInfo.hProcess, (LPDWORD)&exitCode) == TRUE)
		;
	else
	{
		Logger::LogError(__FUNCSTR__, "The exit code process had an error.",
			LastError(), __FLN__);
		/*TODO: Replace with cg::Exception*/
		throw std::runtime_error("Could not get the exit code.");
	}
	UpdateInput();
	UpdateOutput();
	m_running = false;
	return exitCode;
}


void Process::Kill()
{
	m_running = false;
	TerminateProcess(m_procInfo.hProcess, -1);
}

}

