#pragma once

#include "../Timer.hpp"

namespace cg {

class IProcess
{
public:
	/**Virtual dtor*/
	~IProcess()
	{
		Wait();
	};
	/**get the stream that contains the clients output.
	\return An istream reference to the stream containing the clients output.*/
	virtual inline std::istream& ChildOutput();
	/**get the stream that is synced to the childs stdin.
	\return An ostream that will sync to the childs stdin.*/
	virtual inline std::ostream& ChildInput();
	/**Push some input to the child.
	\param str The input to insert to the child.*/
	virtual void InsertInput(const std::string& in) = 0;
	/**Start a process.
	\pram args The args to send along as space seperated strings.
	\return True if the process was started properly.*/
	virtual void Start(const std::string& args = "", 
		double updateFPS = 10) = 0;
	/**Forward athe output.*/
	virtual void UpdateOutput() = 0;
	/**Forward athe output.*/
	virtual inline void AsyncUpdateOutput(double fps);
	/**Forward athe output.*/
	virtual inline void UpdateOutputLoop(double fps);
	/**Forward the input.*/
	virtual void UpdateInput() = 0;
	/**Forward the input.*/
	virtual inline void AsyncUpdateInput(double fps);
	/**Forward the input.*/
	virtual inline void UpdateInputLoop(double fps);
	/**Get the exit code of the running process. Will wait if not finished.
	\return The exit code of the process.*/
	virtual int Result() = 0;
	/**Wait for the threads to finished.*/
	virtual void Wait();
	/**Kill the process immediatly.*/
	virtual void Kill() = 0;
	/**create a runnable command string.
	\tparam Args Anyhting that is convertable to strings for command args.
	\param args command line args for the process.*/
	template<typename...Args>
	static std::string CreateProcessString(const std::string& commandName,
		Args&&...args)
	{
		std::string cmd = commandName;
		cmd += " ";
		cmd += cg::StringTogetherWithSeperator(std::forward<Args>(args)...);
		return cmd;
	}
protected:
	/**The buffer that will go to the childs input handle.*/
	std::stringstream m_childInputBuffer;
	/**The buffer that will receive anything that the child prints to its
	output.*/
	std::stringstream m_childOutputBuffer;
	/**Flag to determine if the process is done or not.*/
	bool m_running = false;
	/**The name and path of the process to run.*/
	std::string m_procPath;
	/**Lock to prevent writing partial data.*/
	cg::MasterLock<std::mutex, false, true> ml_toChildInput;
	/**Lock to prevent reading partial data.*/
	cg::MasterLock<std::mutex, false, true> ml_fromChildOutput;
	/**The thread that will sync input from the process to the buffer.*/
	std::thread* mt_inputSync;
	/**The thread that will sync the output of the process to the buffer.*/
	std::thread* mt_outputSync;
};

inline void IProcess::Wait()
{
	if (mt_inputSync)
		if (mt_inputSync->joinable())
			mt_inputSync->join();
	if (mt_outputSync)
		if (mt_outputSync->joinable())
			mt_outputSync->join();
}

inline void IProcess::AsyncUpdateOutput(double fps)
{
	mt_outputSync = new std::thread(&IProcess::UpdateOutputLoop, this, fps);
}

inline void IProcess::AsyncUpdateInput(double fps)
{
	mt_inputSync = new std::thread(&IProcess::UpdateInputLoop, this, fps);
}
inline void IProcess::UpdateOutputLoop(double fps)
{
	cg::SpeedLimit limit(fps);
	while (m_running)
	{
		limit();
		UpdateOutput();
	}
}
inline void IProcess::UpdateInputLoop(double fps)
{
	cg::SpeedLimit limit(fps);
	while (m_running)
	{
		limit();
		UpdateInput();
	}
}

inline std::istream & cg::IProcess::ChildOutput()
{
	return m_childOutputBuffer;
}

inline std::ostream & cg::IProcess::ChildInput()
{
	return m_childInputBuffer;
}


}

