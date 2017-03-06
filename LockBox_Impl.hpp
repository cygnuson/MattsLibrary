
#pragma once

#include "LockBox.hpp"

namespace cg {

template<typename Container>
inline LockBox<Container>::~LockBox()
{
	/*clean up only if we are using a list reference and we did allocated.*/
	if (m_didAllocateContainer)
		delete m_con;
}

template<typename Container>
inline std::size_t LockBox<Container>::Size() const
{
	static std::integral_constant
		<bool, ContainerTraits<Container>::Sizable> HasSizeMember;
	return Size(HasSizeMember);
}

template<typename Container>
inline bool LockBox<Container>::Empty() const
{
	return m_con->empty();
}

template<typename Container>
inline LockBox<Container>::LockBox()
	:m_con(new Container())
{
	m_didAllocateContainer = true;
	if (!ms_log)
	{
		EnableLogs(true, "LockBox");
		LogNote(3, __FUNCSTR__, "Logs initializd.");
	}
}

template<typename Container>
LockBox<Container>::LockBox(Container& someList,
	cg::Ref op)
	:m_con(&someList)
{
	m_didAllocateContainer = false;
	if (!ms_log)
	{
		EnableLogs(true, "LockBox");
		LogNote(3, __FUNCSTR__, "Logs initializd.");
	}
}
template<typename Container>
LockBox<Container>::LockBox(Container& someList,
	cg::Swap op)
	:m_con(new Container)
{
	m_didAllocateContainer = true;
	m_con->swap(someList);
	if (!ms_log)
	{
		EnableLogs(true, "LockBox");
		LogNote(3, __FUNCSTR__, "Logs initializd.");
	}
}
template<typename Container>
template<typename U>
LockBox<Container>::LockBox(const U& someList,
	cg::Copy op)
	:m_con(new Container(someList.begin(), someList.end()))
{
	m_didAllocateContainer = true;
	if (!ms_log)
	{
		EnableLogs(true, "LockBox");
		LogNote(3, __FUNCSTR__, "Logs initializd.");
	}
}
template<typename Container>
inline std::size_t LockBox<Container>::Size(std::true_type op) const
{
	return m_con->size();
}
template<typename Container>
inline std::size_t LockBox<Container>::Size(std::false_type op) const
{
	std::size_t size = 0;
	auto it = m_con->begin();
	auto end = m_con->end();
	for (; it != end; ++it)
		++size;
	return size;
}

}