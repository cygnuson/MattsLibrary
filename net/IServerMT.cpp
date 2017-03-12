
#include "IServerMT.hpp"

namespace cg {
namespace net {



IServerMT::IServerMT(int dataThreads)
	:m_dataThreadCount(dataThreads)
{
	m_run = false;
}

IServerMT::~IServerMT()
{
	Stop();
	for (int i = 0; i < m_dataThreadCount; ++i)
	{
		if (m_dataThreads[i])
			delete m_dataThreads[i];
	}
	delete[] m_dataThreads;
}

void IServerMT::Start(uint16_t port)
{
	if (m_run)
		Stop();
	m_dataThreads = new std::thread*[m_dataThreadCount];
	for (int i = 0; i < m_dataThreadCount; ++i)
	{
		m_dataThreads[i] = new std::thread(&IServerMT::DataLoop, this);
	}
	m_run = true;
	m_serverSocket.Bind(true, port);
	AutoAccept();
	ScanForReady();
}

void IServerMT::Stop()
{
	m_run = false;
	m_readyBox.Notify();
	m_clientBox.Notify();
	CloseAll();
	WaitForThreads();
}

void IServerMT::Wait()
{
	while (m_run)
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void IServerMT::DataLoop()
{
	++m_activeDataThreads;
	while (m_run)
	{
		/*will wait for elements.*/
		m_readyBox.WaitForElements(std::ref(m_run));
		auto writer = m_readyBox.Writer();
		auto it = writer->begin();
		auto end = writer->end();
		for (; it != end; ++it)
		{
			if (!(*it)->IsOpen())
			{
				it = writer->erase(it);
					continue;
			}
			if ((*it)->ReadReady())
			{
				ProcessSocket(**it);
			}
		}
	}
	--m_activeDataThreads;
}

void IServerMT::CloseAll()
{
	bool a = false;
	bool b = false;
	bool c = false;

	cg::SpeedLimit limit(10);
	while (!a || !b || !c)
	{
		limit();
		a = m_readyScannerStopped;
		b = m_acceptScannerStopped;
		c = m_activeDataThreads == 0;
	}

	if (m_clientBox.Size() == 0)
		return;

	auto writer = m_clientBox.Writer();
	auto it = writer->begin();
	auto end = writer->end();
	for (; it != end; ++it)
	{
		it->Close();
	}
}

void IServerMT::WaitForThreads()
{
	if (mt_readyScanner)
		if (mt_readyScanner->joinable())
			mt_readyScanner->join();
	if (mt_acceptScanner)
		if (mt_acceptScanner->joinable())
			mt_acceptScanner->join();
	for (int i = 0; i < m_dataThreadCount; ++i)
	{
		if (m_dataThreads[i])
			if (m_dataThreads[i]->joinable())
				m_dataThreads[i]->join();
	}
}

void IServerMT::ScanForReady()
{
	m_readyScannerStopped = false;
	mt_readyScanner = new std::thread(&IServerMT::ScanForReadyLoop, this);
}

void IServerMT::AutoAccept()
{
	m_acceptScannerStopped = false;
	m_serverSocket.Listen();
	mt_acceptScanner = new std::thread(&IServerMT::AcceptLoop, this);
}

void IServerMT::ScanForReadyLoop()
{
	cg::SpeedLimit limit(100);
	while (m_run)
	{
		/*will wait for elements.*/
		limit();
		auto writer = m_clientBox.Writer();
		auto it = writer->begin();
		auto end = writer->end();
		for (; it != end; ++it)
		{
			if (!it->IsOpen())
			{
				SocketClosed(*it, false);
				it = writer->erase(it);
				continue;
			}
			if (it->ReadReady())
			{
				m_readyBox.Writer()->emplace_back(std::addressof(*it));
				m_readyBox.Notify();
			}
		}
	}
	m_readyScannerStopped = true;
}

void IServerMT::AcceptLoop()
{
	cg::SpeedLimit limit(100);
	while (m_run)
	{
		limit();
		while (m_serverSocket.ReadReady())
		{
			auto writer = m_clientBox.Writer();
			auto it = writer->emplace(writer->end(), false);
			auto accepted = m_serverSocket.Accept(*it, false);
			if(accepted)
			{
				SocketAccepted(*it);
			}
			else
			{
				writer->pop_back();
			}
		}
	}
	m_acceptScannerStopped = true;
}

}
}