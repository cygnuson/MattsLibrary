#pragma once

#define _DEBUGMASTERLOCK _DEBUG && 0

namespace cg {

template<typename Mutex, bool Ref>
inline bool BasicLock<Mutex, Ref>::TryLock()
{
	if (m_unique_hits > 0)
	{
		LogError(__FUNCSTR__, "~~~~~~~DOUBLE LOCK!!!~~~~~~~~~");
	}
	bool locked = m_lock.try_lock();
	if (locked)
	{
		++m_unique_hits;
		return true;
	}
	else
		return false;
}
template<typename Mutex, bool Ref>
inline void BasicLock<Mutex, Ref>::Unlock()
{
	if (m_unique_hits == 0)
	{
		LogError(__FUNCSTR__, "U~~~~~~DOUBLE UNLOCK!!!~~~~~~~~U");
	}
	m_lock.unlock();
	--m_unique_hits;
}
template<typename Mutex, bool Ref>
inline void cg::BasicLock<Mutex, Ref>::Lock()
{
	if (TryLock())
	{
		return;
	}
	else
	{
#if _DEBUGMASTERLOCK
		LogNote(1, __FUNCSTR__, "TryLock Fail, will block.");
#endif
	}
	m_lock.lock();
}
template<typename Mutex, bool Ref>
inline ScopeLock<BasicLock<Mutex, Ref>>
BasicLock<Mutex, Ref>::LockGuard()
{
	return ScopeLock<BasicLock<Mutex, Ref>>(*this,
		&BasicLock<Mutex, Ref>::Unlock,
		&BasicLock<Mutex, Ref>::Lock);
}

template<typename Mutex, bool Ref>
template<typename T>
inline ScopeLock<BasicLock<Mutex, Ref>, T>
BasicLock<Mutex, Ref>::LockGuard(
	T * member)
{
	return ScopeLock<BasicLock<Mutex, Ref>, T>(*this,
		&BasicLock<Mutex, Ref>::Unlock,
		&BasicLock<Mutex, Ref>::Lock, member);
}
/*****************************************************************************/
template<typename Mutex, bool Ref>
inline bool SharedLock<Mutex, Ref>::STryLock()
{
	if (m_unique_hits > 0)
	{
		LogError(__FUNCSTR__, "~~~~~READLOCK AFTER UNIQUE LOCK!!~~~~~~~~~~~~");
	}
	bool locked = m_lock.try_lock_shared();
	if (locked)
	{
		
		++m_shared_hits;
		return true;
	}
	else
		return false;
}
template<typename Mutex, bool Ref>
inline ScopeLock<SharedLock<Mutex, Ref>>
SharedLock<Mutex, Ref>::SLockGuard()
{
	return ScopeLock<SharedLock<Mutex, Ref>>(*this,
		&SharedLock<Mutex, Ref>::SUnLock,
		&SharedLock<Mutex, Ref>::SLock);
}

template<typename Mutex, bool Ref>
template<typename T>
inline ScopeLock<SharedLock<Mutex, Ref>, T>
SharedLock<Mutex, Ref>::SLockGuard(
	T * member)
{
	return ScopeLock<SharedLock<Mutex, Ref>, T>(*this,
		&SharedLock<Mutex, Ref>::SUnLock,
		&SharedLock<Mutex, Ref>::SLock, member);
}
template<typename Mutex, bool Ref>
inline void SharedLock<Mutex, Ref>::SUnLock()
{
	if (m_shared_hits == 0)
	{
		LogError(__FUNCSTR__, "U~~~~~~EXTRA SUNLOCK!!!~~~~~~~~U");
	}
	m_lock.unlock_shared();
}
template<typename Mutex, bool Ref>
inline void SharedLock<Mutex, Ref>::SLock()
{
	if (STryLock())
	{
		return;
	}
	else
	{
#if _DEBUGMASTERLOCK
		LogNote(1, __FUNCSTR__, "TryLock Fail, will block.");
#endif
	}
	m_lock.lock_shared();
}
/*****************************************************************************/

template<typename Mutex, bool Ref>
template<typename T>
inline bool TimedLock<Mutex, Ref>::
TryLockUntil(const T & chronoTimePoint)
{
	return m_lock.try_lock_until(chronoTimePoint);
}
template<typename Mutex, bool Ref>
template<typename T>
inline bool TimedLock<Mutex, Ref>::
TryLockFor(const T & chronoDuration)
{
	return m_lock.try_lock_for(chronoDuration);
}
/*****************************************************************************/

template<typename Mutex, bool Ref>
template<typename T>
inline bool SharedTimedLock<Mutex, Ref>::
STryLockUntil(const T & chronoTimePoint)
{
	return m_lock.try_lock_until_shared(chronoTimePoint);
}
template<typename Mutex, bool Ref>
template<typename T>
inline bool SharedTimedLock<Mutex, Ref>::
STryLockFor(const T & chronoDuration)
{
	return m_lock.try_lock_for_shared(chronoDuration);
}



}