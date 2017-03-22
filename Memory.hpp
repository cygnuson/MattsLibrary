#pragma once

#include <cstdlib>
#include <string>

#include "Logger.hpp"
#include "exception.hpp"

#if defined(_DEBUG)
#include <map>
#endif

namespace cg {

#if defined(_DEBUG)

class DeleteArrayException : public cg::Exception
{
	inline virtual std::string ToString() const override
	{
		return "Attempted to delete an array with Delete() and not DeleteA().";
	}

};

template<typename T>
class DataLeakImpl;

template<typename T>
class DataLeakImpl
{
public:
	/**The amount of bytes allocated.*/
	static std::ptrdiff_t m_allocated;
	/**The total amount of allocated memory, including what has been freed.*/
	static std::ptrdiff_t m_totalAllocated;
	/**Get the peek ram usuage.*/
	static std::ptrdiff_t m_peekRam;
	/**A list of allocator records.*/
	static std::map<void*, std::pair<std::string, std::size_t>> m_records;

};

template<typename T>
std::ptrdiff_t DataLeakImpl<T>::m_allocated = 0;

template<typename T>
std::ptrdiff_t DataLeakImpl<T>::m_totalAllocated = 0;

template<typename T>
std::ptrdiff_t DataLeakImpl<T>::m_peekRam = 0;

template<typename T>
std::map<void*, std::pair<std::string, std::size_t>> DataLeakImpl<T>::m_records;

using DataLeak = DataLeakImpl<int>;

#endif
/**Allocate a unit of T.
\param args The args that will pass to the constructor.
\return A poitner to the new object.*/
template<typename T, typename...Args>
static T* New(const std::string& note, Args&&...args);
/**Allocate a unit of T.
\param args The args that will pass to the constructor.
\param loc The place to do placement construction.
\return A poitner to the new object.*/
template<typename T, typename...Args>
static T* PNew(const std::string& note, T* loc, Args&&...args);
/**Allocate some space without constructing anything.
\param units The amount of units of T to allocate space for.
\param init True to init the memory.
\return A pointer to the allocated space (as void*).*/
template<typename T>
static T* NewA(const std::string& note, std::size_t units, bool init = false);

/**Deallocate some memory. If it is an array, that is detected properly.
Will only work for pointers allocated with this allocator.
\param loc A pointer to the memory to deallocate.*/
template<typename T>
static void Delete(const std::string& note, T* loc);
/**Deallocate some memory. If it is an array, that is detected properly.
Will only work for pointers allocated with this allocator.
\param loc A pointer to the memory to deallocate.*/
template<typename T>
static void DeleteA(const std::string& note, T* loc);
/**Balance the budget.  Will list all the memory that is currently
allocated. Any leaks at this point will show.
\return The amount of allocated bytes.*/

/**Get the number of currently allocated bytes.*/
static std::size_t MemoryBalance();
/**Determine the enire amount of memory that has been allocated, including
that which has been freed.
\return The total ram usage.*/
static std::size_t TotalMemoryUsage();
/**Determine the the highest amount of ram allocated at one time.
\return The total ram usage.*/
static std::size_t PeekMemoryUsage();
/**Get a memory report.
\return A string with the memory report.*/
static std::string MemoryReport();
}


#include "Memory_impl.hpp"


