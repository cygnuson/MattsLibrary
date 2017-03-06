#pragma once

namespace cg {

template<typename Mutex, bool Ref>
inline void WaitTimedBasicLock<Mutex, Ref>::Lock()
{
	if (TryLock())
	{
		return;
	}
	else
	{
		LogNote(1, __FUNCSTR__, "TryLock Fail, will block.");
	}
	BeforeLock();
	m_lock.lock();
	AfterLock();
}

template<typename Mutex, bool Ref>
inline void WaitTimedBasicLock<Mutex, Ref>::UnLock()
{
	m_lock.unlock();
}

template<typename Mutex, bool Ref>
inline bool WaitTimedBasicLock<Mutex, Ref>::TryLock()
{
	return m_lock.try_lock();
}
template<typename Mutex, bool Ref>
inline ScopeLock<WaitTimedBasicLock<Mutex, Ref>>
WaitTimedBasicLock<Mutex, Ref>::LockGuard()
{
	return ScopeLock<WaitTimedBasicLock<Mutex, Ref>> (*this,
		&WaitTimedBasicLock<Mutex, Ref>::UnLock,
		&WaitTimedBasicLock<Mutex, Ref>::Lock);
}

template<typename Mutex, bool Ref>
template<typename T>
inline ScopeLock<WaitTimedBasicLock<Mutex, Ref>, T>
WaitTimedBasicLock<Mutex, Ref>::LockGuard(
	T * member)
{
	return ScopeLock<WaitTimedBasicLock<Mutex, Ref>, T>(*this,
		&WaitTimedBasicLock<Mutex, Ref>::UnLock,
		&WaitTimedBasicLock<Mutex, Ref>::Lock, member);
}
/*****************************************************************************/
template<typename Mutex, bool Ref>
inline bool WaitTimedSharedLock<Mutex, Ref>::STryLock()
{
	return m_lock.try_lock_shared();
}
template<typename Mutex, bool Ref>
inline void WaitTimedSharedLock<Mutex, Ref>::SUnLock()
{
	m_lock.unlock_shared();
}
template<typename Mutex, bool Ref>
inline void WaitTimedSharedLock<Mutex, Ref>::SLock()
{
	if (STryLock())
	{
		return;
	}
	else
	{
		LogNote(1, __FUNCSTR__, "TryLock Fail, will block.");
	}
	BeforeLock();
	m_lock.lock_shared();
	AfterLock();
}
template<typename Mutex, bool Ref>
inline ScopeLock<WaitTimedSharedLock<Mutex, Ref>>
WaitTimedSharedLock<Mutex, Ref>::SLockGuard()
{
	return ScopeLock<WaitTimedSharedLock<Mutex, Ref>>(*this,
		&WaitTimedSharedLock<Mutex, Ref>::SUnLock,
		&WaitTimedSharedLock<Mutex, Ref>::SLock);
}
template<typename Mutex, bool Ref>
template<typename T>
inline ScopeLock<WaitTimedSharedLock<Mutex, Ref>, T>
WaitTimedSharedLock<Mutex, Ref>::SLockGuard(
	T * member)
{
	return ScopeLock<WaitTimedSharedLock<Mutex, Ref>, T>(*this,
		&WaitTimedSharedLock<Mutex, Ref>::SUnLock,
		&WaitTimedSharedLock<Mutex, Ref>::SLock, member);
}
/*****************************************************************************/

template<typename Mutex, bool Ref>
template<typename T>
inline bool WaitTimedTimedLock<Mutex, Ref>::
TryLockUntil(const T & chronoTimePoint)
{
	return m_lock.try_lock_until(chronoTimePoint);
}
template<typename Mutex, bool Ref>
template<typename T>
inline bool WaitTimedTimedLock<Mutex, Ref>::
TryLockFor(const T & chronoDuration)
{
	return m_lock.try_lock_for(chronoDuration);
}
/*****************************************************************************/

template<typename Mutex, bool Ref>
template<typename T>
inline bool WaitTimedSharedTimedLock<Mutex, Ref>::
STryLockUntil(const T & chronoTimePoint)
{
	return m_lock.try_lock_until_shared(chronoTimePoint);
}
template<typename Mutex, bool Ref>
template<typename T>
inline bool WaitTimedSharedTimedLock<Mutex, Ref>::
STryLockFor(const T & chronoDuration)
{
	return m_lock.try_lock_for_shared(chronoDuration);
}
/*****************************************************************************/
template<typename Mutex, bool Ref>
template<typename TimeUnit>
inline TimeUnit WaitTimedLockMember<Mutex, Ref>::
TimeSpentSleeping() const
{
	return std::chrono::duration_cast<TimeUnit>(m_totalWaitTime);
}
template<typename Mutex, bool Ref>
inline std::chrono::nanoseconds WaitTimedLockMember<Mutex, Ref>::ResetTimer()
{
	auto copy = m_totalWaitTime;
	m_totalWaitTime = std::chrono::nanoseconds(0);
	return copy;
}

template<typename Mutex, bool Ref>
inline void WaitTimedLockMember<Mutex, Ref>::BeforeLock()
{
	m_lastStart = std::chrono::high_resolution_clock::now();
}

template<typename Mutex, bool Ref>
inline void WaitTimedLockMember<Mutex, Ref>::AfterLock()
{
	auto callTime
		= (std::chrono::high_resolution_clock::now() - m_lastStart);
	m_totalWaitTime += callTime;
	auto seconds = std::chrono::duration_cast<std::chrono::milliseconds>
		(callTime).count();
	LogNote(1, __FUNCSTR__, "Blocked for ", seconds, " milliseconds.");
}

template<typename Mutex>
template<typename TimeUnit>
inline TimeUnit WaitTimedLockMember<Mutex, true>::
TimeSpentSleeping() const
{
	return std::chrono::duration_cast<TimeUnit>(m_totalWaitTime);
}
template<typename Mutex>
inline std::chrono::nanoseconds WaitTimedLockMember<Mutex, true>::ResetTimer()
{
	auto copy = m_totalWaitTime;
	m_totalWaitTime = std::chrono::nanoseconds(0);
	return copy;
}

template<typename Mutex>
inline void WaitTimedLockMember<Mutex, true>::BeforeLock()
{
	m_lastStart = std::chrono::high_resolution_clock::now();
}

template<typename Mutex>
inline void WaitTimedLockMember<Mutex, true>::AfterLock()
{
	auto callTime
		= (std::chrono::high_resolution_clock::now() - m_lastStart);
	m_totalWaitTime += callTime;
	auto seconds = std::chrono::duration_cast<std::chrono::milliseconds>
		(callTime).count();
	LogNote(1, __FUNCSTR__, "Blocked for ", seconds, " milliseconds.");
}

}