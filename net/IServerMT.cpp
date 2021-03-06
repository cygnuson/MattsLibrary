
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
	if (!m_serverStopped)
	{
		Stop();
	}
	for (int i = 0; i < m_dataThreadCount; ++i)
	{
		if (m_dataThreads[i])
			cg::Delete(__FUNCSTR__,m_dataThreads[i]);
	}
	cg::DeleteA(__FUNCSTR__,m_dataThreads);
	cg::Delete(__FUNCSTR__,mt_acceptScanner);
	cg::Delete(__FUNCSTR__,mt_readyScanner);
}

void IServerMT::Start(uint16_t port)
{
	if (m_run)
		Stop();
	m_acceptorLimit.FPS(100);
	m_readyLimit.FPS(100);
	m_run = true;
	m_serverStopped = false;
	m_dataThreads = cg::NewA<std::thread*>(__FUNCSTR__,m_dataThreadCount);
	for (int i = 0; i < m_dataThreadCount; ++i)
	{
		m_dataThreads[i] = cg::New<std::thread>(__FUNCSTR__, 
			&IServerMT::DataLoop, this);
	}
	m_serverSocket.Bind(true, port);
	AutoAccept();
	ScanForReady();
}

void IServerMT::Stop()
{
	m_run = false;
	m_readyBox.NotifyAll();
	m_clientBox.NotifyAll();
	CloseAll();
	WaitForThreads();
	m_serverStopped = true;
}

void IServerMT::Wait()
{
	while (m_run)
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void IServerMT::NotifyAllThreads()
{
	m_readyBox.Notify();
	m_clientBox.Notify();
}

void IServerMT::ChangeAcceptorSpeed(double fps)
{
	m_acceptorLimit.FPS(fps);
}

void IServerMT::ChangeScannerSpeed(double fps)
{
	m_readyLimit.FPS(fps);
}

void IServerMT::DataLoop()
{
	++m_activeDataThreads;
	while (m_run)
	{
		/*will wait for elements.*/
		m_readyBox.WaitForElements(std::ref(m_run));
		cg::net::Socket* sock = nullptr;
		auto writer = m_readyBox.Writer();
		if (writer->size() == 0)
			continue;
		sock = *writer->begin();
		writer->pop_front();
		if (sock->IsOpen() != 1)
		{
			cg::Delete(__FUNCSTR__,sock);
		}
		/*sock should be ready beause it was in the ready list.*/
		ProcessSocket(*sock);
		/*put it back into the client box.*/
		m_clientBox.Writer()->push_back(sock);
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
	{
		auto writer = m_clientBox.Writer();
		auto it = writer->begin();
		auto end = writer->end();
		for (; it != end; ++it)
		{
			(*it)->Close();
			cg::Delete(__FUNCSTR__,*it);
		}
	}
	{
		auto writer = m_readyBox.Writer();
		auto it = writer->begin();
		auto end = writer->end();
		for (; it != end; ++it)
		{
			(*it)->Close();
			cg::Delete(__FUNCSTR__,*it);
		}
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
	mt_readyScanner = cg::New<std::thread>(__FUNCSTR__,
		&IServerMT::ScanForReadyLoop, this);
}

void IServerMT::AutoAccept()
{
	m_acceptScannerStopped = false;
	m_serverSocket.Listen();
	mt_acceptScanner = cg::New<std::thread>(__FUNCSTR__,
		&IServerMT::AcceptLoop, this);
}

void IServerMT::ScanForReadyLoop()
{
	while (m_run)
	{
		/*will wait for elements.*/
		m_readyLimit();
		auto writer = m_clientBox.Writer();
		if (writer->empty())
			continue;
		auto it = writer->begin();
		auto end = writer->end();
		for (; it != end; ++it)
		{
			int open = (*it)->IsOpen();
			if (open != 1)
			{
				SocketClosed(**it, open == 0 ? true : false);
				it = writer->erase(it);
				if (it == end)
					/*If it was the last thing in the list, it now equals .end
					and we will break instead of trying to increment it.*/
					break;
				else
					continue;
			}
			if ((*it)->ReadReady())
			{
				m_readyBox.Writer()->push_back(*it);
				it = writer->erase(it);
				m_readyBox.Notify();
				if (it == end)
					/*If it was the last thing in the list, it now equals .end
					and we will break instead of trying to increment it.*/
					break;
			}
		}
	}
	m_readyScannerStopped = true;
}

void IServerMT::AcceptLoop()
{
	while (m_run)
	{
		m_acceptorLimit();
		while (m_serverSocket.ReadReady())
		{
			auto writer = m_clientBox.Writer();
			auto sock = cg::New<cg::net::Socket>(__FUNCSTR__);
			auto accepted = m_serverSocket.Accept(*sock, false);
			if (accepted)
			{
				SocketAccepted(*sock);
				writer->push_back(sock);
			}
			else
			{
				/*not accepted for some reason, delete it.*/
				cg::Delete(__FUNCSTR__,sock);
			}
		}
	}
	m_acceptScannerStopped = true;
}

}
}