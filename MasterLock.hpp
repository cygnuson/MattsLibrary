#pragma once

#include <shared_mutex>
#include <chrono>

#include "LogAdaptor.hpp"
#include "ScopeLock.hpp"

namespace cg {


/**Determine certian things about any mutex that is used in the master lock.*/
template<typename T>
class MutexTraits
{
	/**The YES type*/
	using Yes = char;
	/**The NO type*/
	struct No { char d[2]; };

	/**Called when T does have .lock_shared*/
	template<typename C>
	static Yes TestShared(decltype(&C::lock_shared)) {};
	/**Called when T does not have .lock_shared*/
	template<typename C>
	static No TestShared(...) {};

public:
	/**True if T has a function .crbegin() */
	const static bool Timed = std::is_same<T, std::shared_timed_mutex>::value
		|| std::is_same<T, std::timed_mutex>::value
		|| std::is_same<T, std::recursive_timed_mutex>::value;
	/**True if T has a function .size()*/
	const static bool Shared = (sizeof(TestShared<T>(0)) == sizeof(Yes));

};

/*If Cond= true, RefIf = T&, else RefIf=T*/
template<bool Cond, typename T>
using RefIf = std::conditional_t<Cond, T&, T>;

/*If Cond= true, PtrIf = T*, else PtrIf=T    */
template<bool Cond, typename T>
using PtrIf = std::conditional_t<Cond, T*, T>;

/**A tracker for share/unique locks*/
template<std::size_t temp = 0>
struct LockCounter
{
	static std::size_t m_shared_hits;
	static std::size_t m_unique_hits;
};

template<std::size_t temp>
std::size_t LockCounter<temp>::m_shared_hits;
template<std::size_t temp>
std::size_t LockCounter<temp>::m_unique_hits;

/**The data part of the master lock class.*/
template<typename Mutex, bool Ref>
class LockMember :
	protected cg::LogAdaptor<LockMember<Mutex, Ref>>,
	public LockCounter<0>
{
public:
	/**Create a locking mechanism for the MasterLock.*/
	LockMember()
	{
		if (!ms_log)
		{
			EnableLogs(true, "LockMember[l]");
			LogNote(3, __FUNCSTR__, "Initialized logger.");
		}
	};
protected:
	using LogAdaptor<LockMember<Mutex, Ref>>::ms_log;
	using LogAdaptor<LockMember<Mutex, Ref>>::EnableLogs;
	using LogAdaptor<LockMember<Mutex, Ref>>::LogNote;
	using LogAdaptor<LockMember<Mutex, Ref>>::LogError;
	using LogAdaptor<LockMember<Mutex, Ref>>::LogWarn;
	using LockCounter<0>::m_shared_hits;
	using LockCounter<0>::m_unique_hits;
	/**The mutex created to control the locking.*/
	Mutex m_lock;
};
/**The data part of the master lock class.*/
template<typename Mutex, bool Ref>
class WaitTimedLockMember :
	protected cg::LogAdaptor<WaitTimedLockMember<Mutex, Ref>>,
	public LockCounter<0>
{
public:
	/**Create a locking mechanism for the MasterLock.*/
	WaitTimedLockMember()
		: m_totalWaitTime(std::chrono::nanoseconds(0))
	{
		if (!ms_log)
		{
			EnableLogs(true, "WaitTimedLockMember[l]");
			LogNote(3, __FUNCSTR__, "Initialized logger.");
		}
	};
	/**Get the amount of time so far this MasterLock has spent blocking for its
	entire life span.
	\tparam TimeUnit A std::chrono::duration that the time will be cast to.
	default is seconds as a double.
	\return The time in `TimeUnit` units.*/
	template<typename TimeUnit = std::chrono::duration<double>>
	TimeUnit TimeSpentSleeping() const;
	/**Reset the sleep timer.
	\return The total time spent sleeping in nanoseconds.*/
	std::chrono::nanoseconds ResetTimer();
	/**Signify the start of the lock waiting. Should called right before the 
	line that locks the mutex.*/
	void BeforeLock();
	/**Signify the end of the waiting.  Should be called after the line that 
	locks the mutex.*/
	void AfterLock();
protected:
	using LogAdaptor<WaitTimedLockMember<Mutex, Ref>>::ms_log;
	using LogAdaptor<WaitTimedLockMember<Mutex, Ref>>::EnableLogs;
	using LogAdaptor<WaitTimedLockMember<Mutex, Ref>>::LogNote;
	using LogAdaptor<WaitTimedLockMember<Mutex, Ref>>::LogError;
	using LogAdaptor<WaitTimedLockMember<Mutex, Ref>>::LogWarn;
	using LockCounter<0>::m_shared_hits;
	using LockCounter<0>::m_unique_hits;
	/**The buffer to hold the time point that the locking tried to start.*/
	std::chrono::high_resolution_clock::time_point m_lastStart;
	/**The total amount of time that it took to get the lock.*/
	std::chrono::nanoseconds m_totalWaitTime;
	/**The mutex created to control the locking.*/
	Mutex m_lock;
};
/**The data part of the master lock class.*/
template<typename Mutex>
class LockMember<Mutex, true> :
	protected cg::LogAdaptor<LockMember<Mutex, true>>,
	public LockCounter<0>
{
public:
	/**Create the data part with a reference to the mutex to use.
	\param data A reference to an outside mutex to use for this locker.*/
	LockMember(Mutex& data) :m_lock(data)
	{
		if (!ms_log)
		{
			EnableLogs(true, "LockMember[r]");
			LogNote(3, __FUNCSTR__, "Initialized logger.");
		}
	};
protected:
	using LogAdaptor<LockMember<Mutex, true>>::ms_log;
	using LogAdaptor<LockMember<Mutex, true>>::EnableLogs;
	using LogAdaptor<LockMember<Mutex, true>>::LogNote;
	using LogAdaptor<LockMember<Mutex, true>>::LogError;
	using LogAdaptor<LockMember<Mutex, true>>::LogWarn;
	using LockCounter<0>::m_shared_hits;
	using LockCounter<0>::m_unique_hits;
	/**A reference to a mutex that will be controlling the lock.*/
	Mutex& m_lock;
};
/**The data part of the master lock class.*/
template<typename Mutex>
class WaitTimedLockMember<Mutex, true>
	: protected cg::LogAdaptor<WaitTimedLockMember<Mutex, true>>,
	public LockCounter<0>
{
public:
	/**Create the data part with a reference to the mutex to use.
	\param data A reference to an outside mutex to use for this locker.*/
	WaitTimedLockMember(Mutex& data) :m_lock(data),
		m_totalWaitTime(std::chrono::nanoseconds(0))
	{
		if (!ms_log)
		{
			EnableLogs(true, "WaitTimedLockMember[r]");
			LogNote(3, __FUNCSTR__, "Initialized logger.");
		}
	};
	/**Get the amount of time so far this MasterLock has spent blocking for its
	entire life span.
	\tparam TimeUnit A std::chrono::duration that the time will be cast to.
	default is seconds as a double.
	\return The time in `TimeUnit` units.*/
	template<typename TimeUnit = std::chrono::duration<double>>
	TimeUnit TimeSpentSleeping() const;
	/**Reset the sleep timer.
	\return The total time spent sleeping in nanoseconds.*/
	std::chrono::nanoseconds ResetTimer();
	/**Signify the start of the lock waiting. Should called right before the
	line that locks the mutex.*/
	void BeforeLock();
	/**Signify the end of the waiting.  Should be called after the line that
	locks the mutex.*/
	void AfterLock();
protected:
	using LogAdaptor<WaitTimedLockMember<Mutex, true>>::ms_log;
	using LogAdaptor<WaitTimedLockMember<Mutex, true>>::EnableLogs;
	using LogAdaptor<WaitTimedLockMember<Mutex, true>>::LogNote;
	using LogAdaptor<WaitTimedLockMember<Mutex, true>>::LogError;
	using LogAdaptor<WaitTimedLockMember<Mutex, true>>::LogWarn;
	using LockCounter<0>::m_shared_hits;
	using LockCounter<0>::m_unique_hits;
	/**The buffer to hold the time point that the locking tried to start.*/
	std::chrono::high_resolution_clock::time_point m_lastStart;
	/**The total amount of time that it took to get the lock.*/
	std::chrono::nanoseconds m_totalWaitTime;
	/**A reference to a mutex that will be controlling the lock.*/
	Mutex& m_lock;
};
/**A lock for shared and timed access.*/
template<typename Mutex, bool Ref>
class SharedTimedLock : public LockMember<Mutex, Ref>

{
public:
	/**The portion of the master lock that will deal with shared and times
	parts ofthe lock (eg .try_lock_shared_until).
	\param lock A reference to the lock to use.*/
	SharedTimedLock(Mutex& lock) : LockMember<Mutex, Ref>(lock) {}
	/**The portion of the master lock that will deal with shared and times
	parts ofthe lock (eg .try_lock_shared_until)*/
	SharedTimedLock(){}
	/**Try to lock the mutex with a timeout.
	\return True if the mutex was locked.
	\tparam T A chrono duration.*/
	template<typename T>
	bool STryLockFor(const T& chronoDuration);
	/**Try to lock the mutex and stop the the time has been raeched.
	\return True if the mutex was locked.
	\tparam T A chrono time point.*/
	template<typename T>
	bool STryLockUntil(const T& chronoTimePoint);
private:
	using LockMember<Mutex, Ref>::m_lock;
	using LockMember<Mutex, Ref>::ms_log;
	using LockMember<Mutex, Ref>::EnableLogs;
	using LockMember<Mutex, Ref>::LogNote;
	using LockMember<Mutex, Ref>::LogError;
	using LockMember<Mutex, Ref>::LogWarn;
	using LockCounter<0>::m_shared_hits;
	using LockCounter<0>::m_unique_hits;

};
/**A time monitored lock for shared and timed access.*/
template<typename Mutex, bool Ref>
class WaitTimedSharedTimedLock :
	public WaitTimedLockMember<Mutex, Ref>

{
public:
	/**The portion of the master lock that will deal with shared and times
	parts ofthe lock (eg .try_lock_shared_until).  This version will keep data
	about how long it takes to aquire locks.
	\param lock A reference to the lock to use.*/
	WaitTimedSharedTimedLock(Mutex& lock) :
		WaitTimedLockMember<Mutex, Ref>(lock) {}
	/**The portion of the master lock that will deal with shared and times
	parts ofthe lock (eg .try_lock_shared_until).  This version will keep data
	about how long it takes to aquire locks.*/
	WaitTimedSharedTimedLock() {}
	/**Try to lock the mutex with a timeout.
	\return True if the mutex was locked.
	\tparam T A chrono duration.*/
	template<typename T>
	bool STryLockFor(const T& chronoDuration);
	/**Try to lock the mutex and stop the the time has been raeched.
	\return True if the mutex was locked.
	\tparam T A chrono time point.*/
	template<typename T>
	bool STryLockUntil(const T& chronoTimePoint);
private:
	using WaitTimedLockMember<Mutex, Ref>::m_lock;
	using WaitTimedLockMember<Mutex, Ref>::m_totalWaitTime;
	using WaitTimedLockMember<Mutex, Ref>::ms_log;
	using WaitTimedLockMember<Mutex, Ref>::EnableLogs;
	using WaitTimedLockMember<Mutex, Ref>::LogNote;
	using WaitTimedLockMember<Mutex, Ref>::LogError;
	using WaitTimedLockMember<Mutex, Ref>::LogWarn;
	using WaitTimedLockMember<Mutex, Ref>::BeforeLock;
	using WaitTimedLockMember<Mutex, Ref>::AfterLock;
	using LockCounter<0>::m_shared_hits;
	using LockCounter<0>::m_unique_hits;

};
/**A lock for timed access.*/
template<typename Mutex, bool Ref>
class TimedLock : 
	public std::conditional_t<MutexTraits<Mutex>::Shared,
	SharedTimedLock<Mutex, Ref>, 
	LockMember<Mutex, Ref>>

{
public:
	/**The timed lock portion of the master lock.
	\param lock A reference to a lock to use for the data member portion.*/
	TimedLock(Mutex& lock) : std::conditional_t<MutexTraits<Mutex>::Shared,
		SharedTimedLock<Mutex, Ref>, LockMember<Mutex, Ref>>(lock) {}
	/**The timed lock portion of the master lock.*/
	TimedLock(){}
	/**Try to lock the mutex with a timeout.
	\return True if the mutex was locked.
	\tparam T A chrono duration.*/
	template<typename T>
	bool TryLockFor(const T& chronoDuration);
	/**Try to lock the mutex and stop the the time has been raeched.
	\return True if the mutex was locked.
	\tparam T A chrono time point.*/
	template<typename T>
	bool TryLockUntil(const T& chronoTimePoint);
private:
	using LockMember<Mutex, Ref>::m_lock;
	using LockMember<Mutex, Ref>::ms_log;
	using LockMember<Mutex, Ref>::EnableLogs;
	using LockMember<Mutex, Ref>::LogNote;
	using LockMember<Mutex, Ref>::LogError;
	using LockMember<Mutex, Ref>::LogWarn;
	using LockCounter<0>::m_shared_hits;
	using LockCounter<0>::m_unique_hits;

};
/**A time monitored lock for timed access.*/
template<typename Mutex, bool Ref>
class WaitTimedTimedLock :
	public std::conditional_t<MutexTraits<Mutex>::Shared,
	WaitTimedSharedTimedLock<Mutex, Ref>,
	WaitTimedLockMember<Mutex, Ref>>

{
public:
	/**The timed lock portion of the master lock.  Will keep data about how
	much time is spent trying to aquire a lock.
	\param lock A reference to a lock to use for the data member portion.*/
	WaitTimedTimedLock(Mutex& lock) :
		std::conditional_t<MutexTraits<Mutex>::Shared,
		WaitTimedSharedTimedLock<Mutex, Ref>,
		WaitTimedLockMember<Mutex, Ref>>(lock) {}
	/**The timed lock portion of the master lock.  Will keep data about how
	much time is spent trying to aquire a lock.*/
	WaitTimedTimedLock() {}
	/**Try to lock the mutex with a timeout.
	\return True if the mutex was locked.
	\tparam T A chrono duration.*/
	template<typename T>
	bool TryLockFor(const T& chronoDuration);
	/**Try to lock the mutex and stop the the time has been raeched.
	\return True if the mutex was locked.
	\tparam T A chrono time point.*/
	template<typename T>
	bool TryLockUntil(const T& chronoTimePoint);
private:
	using WaitTimedLockMember<Mutex, Ref>::m_lock;
	using WaitTimedLockMember<Mutex, Ref>::m_totalWaitTime;
	using WaitTimedLockMember<Mutex, Ref>::ms_log;
	using WaitTimedLockMember<Mutex, Ref>::EnableLogs;
	using WaitTimedLockMember<Mutex, Ref>::LogNote;
	using WaitTimedLockMember<Mutex, Ref>::LogError;
	using WaitTimedLockMember<Mutex, Ref>::LogWarn;
	using WaitTimedLockMember<Mutex, Ref>::BeforeLock;
	using WaitTimedLockMember<Mutex, Ref>::AfterLock;
	using LockCounter<0>::m_shared_hits;
	using LockCounter<0>::m_unique_hits;

};

/**A lock for shared access.*/
template<typename Mutex, bool Ref>
class SharedLock :
	public std::conditional_t<MutexTraits<Mutex>::Timed,
	TimedLock<Mutex, Ref>,
	LockMember<Mutex, Ref>>

{
public:
	/**Create the shared lock part only if the mutex supports .lock_shared()
	function.
	\param lock The lock to be used for this master lock instanciation.  Will
	be unused if the member part is set to not be a reference.*/
	SharedLock(Mutex& lock) :
		std::conditional_t<MutexTraits<Mutex>::Timed,
		TimedLock<Mutex, Ref>,
		LockMember<Mutex, Ref>>(lock) {}
	/**Create the shared lock part only if the mutex supports .lock_shared()
	function.*/
	SharedLock(){}
	/**Call the lock funtion pointer.  If Shared is true, it will be a shared
	lock, otherwise it will be a unique lock.*/
	void SLock();
	/**Call the unlock function pointer.  If Shared is true, it will be a
	shared lock, otherwise it will be a unique lock.*/
	void SUnLock();
	/**Call the try lock function pointer.  If Shared is true, it will be a
	shared lock, otherwise it will be a unique lock.
	\return True if the lock was locked, false if it was not.*/
	bool STryLock();
	/**Get a shared lockguard that uses .lock_shared and .unlock_shared
	\return The shared lock guard*/
	ScopeLock<SharedLock<Mutex, Ref>> SLockGuard();
	/**Get a basic .lock_shared and .unlock_shared scoped lock guard with a
	member to access.
	\param member A pointer to something to have access.
	\return A basic scoped lock.*/
	template<typename T>
	ScopeLock<SharedLock<Mutex, Ref>, T> SLockGuard(T* member);
private:
	using LockMember<Mutex, Ref>::m_lock;
	using LockMember<Mutex, Ref>::ms_log;
	using LockMember<Mutex, Ref>::EnableLogs;
	using LockMember<Mutex, Ref>::LogNote;
	using LockMember<Mutex, Ref>::LogError;
	using LockMember<Mutex, Ref>::LogWarn;
	using LockCounter<0>::m_shared_hits;
	using LockCounter<0>::m_unique_hits;

};
/**A time monitored lock for shared access.*/
template<typename Mutex, bool Ref>
class WaitTimedSharedLock :
	public std::conditional_t<MutexTraits<Mutex>::Timed,
	WaitTimedTimedLock<Mutex, Ref>,
	WaitTimedLockMember<Mutex, Ref>>

{
public:
	/**Create the shared lock part only if the mutex supports .lock_shared()
	function.  This version will be time monitored.
	\param lock The lock to be used for this master lock instanciation.  Will
	be unused if the member part is set to not be a reference.*/
	WaitTimedSharedLock(Mutex& lock) :
		std::conditional_t<MutexTraits<Mutex>::Timed,
		WaitTimedTimedLock<Mutex, Ref>,
		WaitTimedLockMember<Mutex, Ref>>(lock) {}
	/**Create the shared lock part only if the mutex supports .lock_shared()
	function.  This version will be time monitored.*/
	WaitTimedSharedLock(){}
	/**Call the lock funtion pointer.  If Shared is true, it will be a shared
	lock, otherwise it will be a unique lock.*/
	void SLock();
	/**Call the unlock function pointer.  If Shared is true, it will be a
	shared lock, otherwise it will be a unique lock.*/
	void SUnLock();
	/**Call the try lock function pointer.  If Shared is true, it will be a
	shared lock, otherwise it will be a unique lock.
	\return True if the lock was locked, false if it was not.*/
	bool STryLock();
	/**Get a shared lockguard that uses .lock_shared and .unlock_shared
	\return The shared lock guard*/
	ScopeLock<WaitTimedSharedLock<Mutex, Ref>> SLockGuard();
	/**Get a basic .lock_shared and .unlock_shared scoped lock guard with a 
	member to access.
	\param member A pointer to something to have access.
	\return A basic scoped lock.*/
	template<typename T>
	ScopeLock<WaitTimedSharedLock<Mutex, Ref>, T> SLockGuard(T* member);
private:
	using WaitTimedLockMember<Mutex, Ref>::m_lock;
	using WaitTimedLockMember<Mutex, Ref>::m_totalWaitTime;
	using WaitTimedLockMember<Mutex, Ref>::ms_log;
	using WaitTimedLockMember<Mutex, Ref>::EnableLogs;
	using WaitTimedLockMember<Mutex, Ref>::LogNote;
	using WaitTimedLockMember<Mutex, Ref>::LogError;
	using WaitTimedLockMember<Mutex, Ref>::LogWarn;
	using WaitTimedLockMember<Mutex, Ref>::BeforeLock;
	using WaitTimedLockMember<Mutex, Ref>::AfterLock;
	using LockCounter<0>::m_shared_hits;
	using LockCounter<0>::m_unique_hits;

};
/**The basic lock mechanism.*/
template<typename Mutex, bool Ref>
class BasicLock :
	public std::conditional_t<MutexTraits<Mutex>::Shared,
	SharedLock<Mutex, Ref>,
	std::conditional_t<MutexTraits<Mutex>::Timed,
	TimedLock<Mutex, Ref>,
	LockMember<Mutex, Ref>>>
{
public:
	/**Create the basic lock.
	\param lock The lock to use.
	*/
	BasicLock(Mutex& lock) :std::conditional_t<MutexTraits<Mutex>::Shared,
		SharedLock<Mutex, Ref>,
		std::conditional_t<MutexTraits<Mutex>::Timed,
		TimedLock<Mutex, Ref>,
		LockMember<Mutex, Ref>>>(lock) {}

	/**Create the basic lock..
	*/
	BasicLock() {}
	/**Call the lock funtion pointer.  If Shared is true, it will be a shared
	lock, otherwise it will be a unique lock.*/
	void Lock();
	/**Call the unlock function pointer.  If Shared is true, it will be a
	shared lock, otherwise it will be a unique lock.*/
	void Unlock();
	/**Call the try lock function pointer.  If Shared is true, it will be a
	shared lock, otherwise it will be a unique lock.
	\return True if the lock was locked, false if it was not.*/
	bool TryLock();
	/**Get a basic .lock and .unlock scoped lock guard.
	\return A basic scoped lock.*/
	ScopeLock<BasicLock<Mutex, Ref>> LockGuard();
	/**Get a basic .lock and .unlock scoped lock guard with a member to access.
	\param member A pointer to something to have access.
	\return A basic scoped lock.*/
	template<typename T>
	ScopeLock<BasicLock<Mutex, Ref>, T> LockGuard(T* member);
private:
	using LockMember<Mutex, Ref>::m_lock;
	using LockMember<Mutex, Ref>::ms_log;
	using LockMember<Mutex, Ref>::EnableLogs;
	using LockMember<Mutex, Ref>::LogNote;
	using LockMember<Mutex, Ref>::LogError;
	using LockMember<Mutex, Ref>::LogWarn;
	using LockCounter<0>::m_shared_hits;
	using LockCounter<0>::m_unique_hits;

};
/**The time-measured basic lock mechanism.*/
template<typename Mutex, bool Ref>
class WaitTimedBasicLock :
	public std::conditional_t<MutexTraits<Mutex>::Shared,
	WaitTimedSharedLock<Mutex, Ref>,
	std::conditional_t<MutexTraits<Mutex>::Timed,
	WaitTimedTimedLock<Mutex, Ref>,
	WaitTimedLockMember<Mutex, Ref>>>
{
public:
	/**Create the basic lock.
	\param lock The lock to use.
	*/
	WaitTimedBasicLock(Mutex& lock) :
		std::conditional_t<MutexTraits<Mutex>::Shared,
		WaitTimedSharedLock<Mutex, Ref>,
		std::conditional_t<MutexTraits<Mutex>::Timed,
		WaitTimedTimedLock<Mutex, Ref>,
		WaitTimedLockMember<Mutex, Ref>>>(lock) {}

	/**Create the basic lock.
	*/
	WaitTimedBasicLock() {}
	/**Call the lock funtion pointer.  If Shared is true, it will be a shared
	lock, otherwise it will be a unique lock.*/
	void Lock();
	/**Call the unlock function pointer.  If Shared is true, it will be a
	shared lock, otherwise it will be a unique lock.*/
	void Unlock();
	/**Call the try lock function pointer.  If Shared is true, it will be a
	shared lock, otherwise it will be a unique lock.
	\return True if the lock was locked, false if it was not.*/
	bool TryLock();
	/**Get a basic .lock and .unlock scoped lock guard.
	\return A basic scoped lock.*/
	ScopeLock<WaitTimedBasicLock<Mutex, Ref>> LockGuard();
	/**Get a basic .lock and .unlock scoped lock guard with a member to access.
	\param member A pointer to something to have access.
	\return A basic scoped lock.*/
	template<typename T>
	ScopeLock<WaitTimedBasicLock<Mutex, Ref>, T> LockGuard(T* member);
private:
	using WaitTimedLockMember<Mutex, Ref>::m_lock;
	using WaitTimedLockMember<Mutex, Ref>::m_totalWaitTime;
	using WaitTimedLockMember<Mutex, Ref>::ms_log;
	using WaitTimedLockMember<Mutex, Ref>::EnableLogs;
	using WaitTimedLockMember<Mutex, Ref>::LogNote;
	using WaitTimedLockMember<Mutex, Ref>::LogError;
	using WaitTimedLockMember<Mutex, Ref>::LogWarn;
	using WaitTimedLockMember<Mutex, Ref>::BeforeLock;
	using WaitTimedLockMember<Mutex, Ref>::AfterLock;
	using LockCounter<0>::m_shared_hits;
	using LockCounter<0>::m_unique_hits;

};


/**An outer class thats sole purpose is to assemble a certian configuration
of subclassed to match the type of mutex supplied.
\tparam Mutex The type of mutex to use.
\tparam Ref True to have the internal mutex be a reference.
\tparam WaitTimed True to keep track of the amount of time that a thread is 
waiting on the internal lock.*/
template<typename Mutex, bool Ref, bool WaitTimed>
class MasterLock : public std::conditional_t<WaitTimed,
	WaitTimedBasicLock<Mutex, Ref>,
	BasicLock<Mutex, Ref>>
{
public:
	/**The type of BasicLock that this master lock will be using.*/
	using MemberType = std::conditional_t<WaitTimed,
		WaitTimedBasicLock<Mutex, Ref>,
		BasicLock<Mutex, Ref>>;
	/**Craete the MasterLock.

	If Ref=true, the lock will take a reference to some other lock. If
	Shared=true the lock will be a shared_timed_mutex.
	\param lock The lock to use for the reference.*/
	MasterLock(Mutex& lock) :
		std::conditional_t<WaitTimed,
		WaitTimedBasicLock<Mutex, Ref>,
		BasicLock<Mutex, Ref>>(lock) 
	{
		static_assert(Ref == true, "When passing a reference to the MasterLock"
			" the Ref tparam must be true.");
	}
	/**Craete the MasterLock.

	If Ref=true, the lock will take a reference to some other lock. If
	Shared=true the lock will be a shared_timed_mutex.*/
	MasterLock()
	{
		static_assert(Ref != true, "When building a MasterLock with no "
			"parameter, the Ref tparam must be false.");
	}

private:
	
};


}

#include "MasterLock_Impl.hpp"

#include "WaitTimeLock_Impl.hpp"




