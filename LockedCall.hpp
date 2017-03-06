#pragma once

#include <mutex>

namespace cg {

/**Will apply a unique lock to the mutex right before calling the caller.  Will
be unlocked when the caller completes.\tparam Call The caller type.
\tparam MutexType The type of mutex being used.
\tparam Args The args goign to the Call type.
\param call The object to call.
\param mutex The mutex to lock (ref).
\param args The arguments that will be sent to the caller.*/
template<typename Call, typename MutexType, typename...Args>
decltype(auto) UniqueLockedCall(Call& call,
	MutexType& mutex,
	Args&&...args)
{
	using LockType = std::unique_lock<MutexType>;
	LockType lock(mutex);
	return call(std::forward<Args>(args)...);
}

/**Will apply a lock guard to the mutex right before calling the caller.  Will
be unlocked when the caller completes.\tparam Call The caller type.
\tparam MutexType The type of mutex being used.
\tparam Args The args goign to the Call type.
\param call The object to call.
\param mutex The mutex to lock (ref).
\param args The arguments that will be sent to the caller.*/
template<typename Call, typename MutexType, typename...Args>
decltype(auto) LockGuardCall(Call& call,
	MutexType& mutex,
	Args&&...args)
{
	using LockType = std::lock_guard<MutexType>;
	LockType lock(mutex);
	return call(std::forward<Args>(args)...);
}
/**Will lock the mutex and then call the caller. When the function returns, 
the mutex will still be locked.
\tparam Call The caller type.
\tparam MutexType The type of mutex being used.
\tparam Args The args goign to the Call type.
\param call The object to call.
\param mutex The mutex to lock (ref).
\param args The arguments that will be sent to the caller.*/
template<typename Call, typename MutexType, typename...Args>
decltype(auto) LockAndCall(Call& call,
	MutexType& mutex,
	Args&&...args)
{
	mutex.lock();
	return call(std::forward<Args>(args)...);
}

}
