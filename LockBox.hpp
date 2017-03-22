#pragma once

#include <shared_mutex>

#include "LogAdaptor.hpp"

#include "MasterLock.hpp"

namespace cg {
	
/**Signify some sort of Copy operation.*/
struct Copy { Copy() {} };
/**Signify some sort of Reference operation.*/
struct Ref { Ref() {} };
/**Signify some sort of Swap operation.*/
struct Swap { Swap() {} };
/**Signify some sort of Shared operation.*/
struct Shared { Shared() {} };
	
/**Will allow SFNINAE for forward_iterator ( no size() ) ) and other.*/
template<typename T>
class ContainerTraits
{

	/**True type meaning.*/
	template<typename I, typename = decltype(&I::size)>
	static inline std::true_type _Hassize(int) {};
	/**The false type .*/
	template<typename I>
	static inline std::false_type _Hassize(...) {};
	/**SFINAE strcut .*/
	template<typename I>
	struct Hassize : decltype(_Hassize<I>(int{}))
	{
		/**The value of the sfinae struct. true if T has the noted function.*/
		static const bool value =
			std::decay_t<decltype(_Hassize<I>(int{})) > ::value;
	};


public:
	/**True if T has a function .size()*/
	const static bool Sizable = Hassize<T>::value;
};


/**An atomic container that makes it simple to lock and access the list.

The LockBox is designed for read access and write interupts to alter the
contents of the container.  When a position on the list is checked out from the
reader, the mutex is in a state of shared locking and cannot be written to.
when accessed, the writer will block (or not if used TryLock) untill it can
lock the mutex in a unique locking method, which will prevent any read only or
other write level access.
\tparam Container The container to use to hold the data. It must use either
BiDirectional or Random access iterators.
*/
template<typename Container>
class LockBox :
	public LogAdaptor<LockBox<Container>>
{
public:
	/**The type of value being used.*/
	using Type = typename Container::value_type;
	/**The mutex type.*/
	using Mutex = std::shared_timed_mutex;
	/**static flag for timed locks*/
	const static bool TimedLocks = true;
	/**Create the box.

	Create the box with an empty container and an unlocked lock.  Exactly one
	unit of Container is alloced with new when this constructor is called.*/
	LockBox();
	/**Create a lockbox with another container as a reference.

	This contructor
	will only ever be called if in ConRef mode, and the type of someList
	is the same as the type of Container. No memory is allocated or deleted
	if this is the constructor that is called.
	\param someList The list to reference.
	\param op The operation to use.*/
	LockBox(Container& someList, cg::Ref op);
	/**Create a lockbox with another container as a reference.

	This contructor will allocate a Container with new. It will steal all the
	values of the list given to it.  The list `someList` will be empty when
	this construter is done.
	\param someList The list to swap with.
	\param op The operation to use.*/
	LockBox(Container& someList, cg::Swap op);
	/**Create the lock box by copying from another container.

	If the lockbox is in reference mode, this will be called if a container of
	a different type is used to construct.  This constructer will call new
	exactly once to allocate exactly one unit of Container.
	\param someList The list to copy.
	\param op The operation to use.*/
	template<typename U>
	LockBox(const U& someList, cg::Copy op);
	/*Make sure to delete the container if it was allocated*/
	~LockBox();
	/**Get the current size of the lockbox.
	\return The list of the container held by the lockbox.*/
	std::size_t Size() const;
	/**Quickly determine if the box is empty.
	\return True if the box is empty.*/
	bool Empty() const;
	/**Wait on this box untill its size is not zero any more. If it is already
	not zero, then it returns immediatly.*/
	void WaitForElements();
	/**Wait on this box untill its size is not zero any more. If it is already
	not zero, then it returns immediatly.
	\param extBool A reference to a boolean that when its false, the waiting
	will stop.*/
	void WaitForElements(bool& extBool);
	/**Wait on this box untill its size is not zero any more. If it is already
	not zero, then it returns immediatly.
	\param extBool A reference to and atomic bool that will stop the waiting
	when made to be false*/
	void WaitForElements(std::atomic_bool& extBool);
	/**Notify the waiter that somehting has happened and should wake up.*/
	void Notify();
	/**NotifyAll on the waiter that somehting has happened and should wake up.
	*/
	void NotifyAll();
	/**Try to get reader access to the lock box.
	\return true of the reader is locked for this thread.*/
	inline bool TryReader()
	{
		return m_lock.STryLock();
	}
	/**Unlock the lock gained from tryReader.*/
	inline void UnTryReader()
	{
		return m_lock.SUnLock();
	}
	/*these need to be after m_con so they neet to get initialized after it in
	the init list.*/
	/**The main reader.*/
	inline auto Reader()
	{
		return m_lock.SLockGuard<const Container>(m_con);
	}
	/**Try to get writer access to the lock box.
	\return true of the writer is locked for this thread.*/
	inline bool TryWriter()
	{
		return m_lock.TryLock();
	}
	/**Unlock the lock gained from tryWriter.*/
	inline void UnTryWriter()
	{
		return m_lock.Unlock();
	}
	/**The main writer*/
	inline auto Writer()
	{
		return m_lock.LockGuard(m_con);
	}
private:
	using LogAdaptor<LockBox<Container>>::EnableLogs;
	using LogAdaptor<LockBox<Container>>::ms_log;
	using LogAdaptor<LockBox<Container>>::LogNote;
	using LogAdaptor<LockBox<Container>>::LogError;
	using LogAdaptor<LockBox<Container>>::LogWarn;
	using LogAdaptor<LockBox<Container>>::Log;
	using LogAdaptor<LockBox<Container>>::ms_name;
	/**Size for forwards lists.*/
	std::size_t Size(std::false_type) const;
	/**Size for non forwards lists.*/
	std::size_t Size(std::true_type) const;
	/**The shared mutex access.*/
	MasterLock<Mutex,false, TimedLocks> m_lock;
	/**A unique mutex for waiting.*/
	std::mutex m_waitMutex;
	/**A condition variable for waiting.*/
	std::condition_variable mcv_waiter;
	/**The container to hold the elements. It will be a reference if needed*/
	Container* m_con;
	/*keep track incase we need to delete out list.*/
	bool m_didAllocateContainer;
};


}

#include "LockBox_Impl.hpp"
