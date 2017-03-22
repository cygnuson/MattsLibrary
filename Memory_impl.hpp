#pragma once

#include "Memory.hpp"

namespace cg {

template<typename T, typename ...Args>
inline T * New(const std::string& note, Args && ...args)
{
#if defined(_DEBUG)
	auto ptr = new T(std::forward<Args>(args)...);
	DataLeak::m_records[(void*)ptr].first = note;
	DataLeak::m_records[ptr].second = sizeof(T);
	DataLeak::m_allocated += sizeof(T);
	DataLeak::m_totalAllocated += sizeof(T);
	if (DataLeak::m_allocated > DataLeak::m_peekRam)
		DataLeak::m_peekRam = DataLeak::m_allocated;
	cg::Logger::LogNote(1, "New: ", sizeof(T), " bytes.");
	return ptr;
#else
	return new T(std::forward<Args>(args)...);
#endif
}

template<typename T, typename ...Args>
T* PNew(const std::string& note, T * loc, Args && ...args)
{
#if defined(_DEBUG)
	auto ptr = new (loc) T(std::forward<Args>(args)...);
	cg::Logger::LogNote(1, "PNew: ", sizeof(T), " bytes.");
	return ptr;
#else
	return new T(std::forward<Args>(args)...);
#endif
}

template<typename T>
inline T* NewA(const std::string& note, std::size_t units, bool init)
{
#if defined(_DEBUG)
	T* ptr = nullptr;
	if (init)
		ptr = new T[units]();
	else
		ptr = new T[units];
	DataLeak::m_records[(void*)ptr].first = note;
	DataLeak::m_records[(void*)ptr].second = units * sizeof(T);
	DataLeak::m_allocated += units * sizeof(T);
	DataLeak::m_totalAllocated += units * sizeof(T);
	if (DataLeak::m_allocated > DataLeak::m_peekRam)
		DataLeak::m_peekRam = DataLeak::m_allocated;
	cg::Logger::LogNote(1, "NewA: ", units * sizeof(T), " bytes.");
	return ptr;
#else
	if (init)
		return new T[units]();
	else
		return new T[units];
#endif
}

template<typename T>
inline void Delete(const std::string& note, T * loc)
{
#if defined(_DEBUG)
	std::size_t allocAmt = (DataLeak::m_records[loc].second);
	/*T may be polymorphic. Subtract the amount allocated, not the size of T.*/
	DataLeak::m_allocated -= allocAmt;
	DataLeak::m_records.erase(loc);
	cg::Logger::LogNote(1, "Delete: ", sizeof(T), " bytes.");
#endif
	delete loc;
}

template<typename T>
inline void DeleteA(const std::string& note, T * loc)
{
#if defined(_DEBUG)
	DataLeak::m_allocated -= (DataLeak::m_records[loc].second);
	cg::Logger::LogNote(1, "DeleteA: ", (DataLeak::m_records[loc].second),
		" bytes.");
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

std::size_t TotalMemoryUsage()
{
#if defined(_DEBUG)
	return DataLeakImpl<int>::m_totalAllocated;
#else
	return 0;
#endif
}

std::size_t PeekMemoryUsage()
{
#if defined(_DEBUG)
	return DataLeakImpl<int>::m_peekRam;
#else
	return 0;
#endif
}

std::string MemoryReport()
{
#if defined(_DEBUG)
	std::string report = "\n\n\nMemory Report:";
	report += "\n\tCurrentlyAllocated: ";
	report += std::to_string(MemoryBalance());
	report += "\n\t   TotalAllocation: ";
	report += std::to_string(TotalMemoryUsage());
	report += "\n\t     PeekAllocated: ";
	report += std::to_string(PeekMemoryUsage());
	if (DataLeakImpl<int>::m_records.size() > 0)
	{
		report += "\n\n\nCalls that produce leaks:\n\n\n";
		auto begin = DataLeakImpl<int>::m_records.begin();
		auto end = DataLeakImpl<int>::m_records.end();
		for (; begin != end; ++begin)
		{
			report += begin->second.first;
			report += std::to_string(begin->second.second);
			report += "\n";
		}
	}
	else
	{
		report += "\n";
		report += "There are no outstanding memory allocations.";
	}
	return report + "\n\n\n";
#else
	return "The memory tracker is off.";
#endif
}

}