#include "Timer.hpp"

namespace cg {

Timer::Timer(bool startPaused)
{
	m_pausedAmount = std::chrono::nanoseconds(0);
	if (startPaused)
	{
		Start();
		Pause();
	}
	else
	{
		m_paused = false;
		Start();
	}

}
inline void Timer::Start()
{
	t1 = std::chrono::high_resolution_clock::now();
}

void Timer::Pause()
{
	if (!m_paused)
	{
		m_pausedTime = std::chrono::high_resolution_clock::now();
		m_paused = true;
	}
}

void Timer::UnPause()
{
	if (m_paused)
	{
		m_pausedAmount +=
			(std::chrono::high_resolution_clock::now() - m_pausedTime);
		m_paused = false;
	}
}

std::chrono::nanoseconds Timer::GetDuration() const
{
	auto end = std::chrono::high_resolution_clock::now();
	auto timeInNSec = (end - t1);
	auto pausedAmt = m_pausedAmount;
	if (m_paused)
	{
		pausedAmt +=
			(std::chrono::high_resolution_clock::now() - m_pausedTime);
	}
	return timeInNSec - pausedAmt;
}
// restart the timer.
std::chrono::nanoseconds  Timer::Restart() {
	auto t = GetDuration();
	Start();
	return t;
}

}