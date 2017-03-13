#pragma once

#include "Memory.hpp"

namespace cg {

template<typename T, typename ...Args>
inline T * New(Args && ...args)
{
#if defined(_DEBUG)
	auto ptr = new T(std::forward<Args>(args)...);
	DataLeak::m_records[ptr] = sizeof(T);
	DataLeak::m_allocated += sizeof(T);
	return ptr;
#else
	return new T(std::forward<Args>(args)...);
#endif
}

template<typename T>
inline T* NewA(std::size_t units, bool init)
{
#if defined(_DEBUG)
	T* ptr = nullptr;
	if (init)
		ptr = new T[units]();
	else
		ptr = new T[units];
	DataLeak::m_records[(void*)ptr] = units*sizeof(T);
	DataLeak::m_allocated += units * sizeof(T);
	return ptr;
#else
	if (init)
		return new T[units]();
	else
		return new T[units];
#endif
}

template<typename T>
inline void Delete(T * loc)
{
#if defined(_DEBUG)
	DataLeak::m_allocated -= sizeof(T);
#endif
	delete loc;
}

template<typename T>
inline void DeleteA(T * loc)
{
#if defined(_DEBUG)
	DataLeak::m_allocated -= (DataLeak::m_records[loc]);
	DataLeak::m_records.erase(loc);
#endif
	delete[] loc;
}


inline std::size_t MemoryBalance()
{
#if defined(_DEBUG)
	return DataLeakImpl<int>::m_allocated;
#else
	return 0;
#endif
}

}