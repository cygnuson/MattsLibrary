#include "ScopeLock.hpp"
#pragma once

namespace cg {

template<typename Mutex, typename UseMember>
inline ScopeLock<Mutex, UseMember>::ScopeLock(Mutex & lock,
	FPtr parameter,
	FPtr lockFunc,
	UseMember * ref)
	:m_lock(lock), m_ptr(parameter)
{
	m_ref = ref;
	(m_lock.*lockFunc)();
}
template<typename Mutex, typename UseMember>
inline ScopeLock<Mutex, UseMember>::ScopeLock(
	ScopeLock<Mutex, UseMember>&& other)
	:m_ref(other.m_ref),
	m_lock(other.m_lock),
	m_ptr(other.m_ptr)
{
	/**invalidates the other scope lock.*/
	other.m_ref = nullptr;
}
template<typename Mutex, typename UseMember>
inline ScopeLock<Mutex, UseMember>::~ScopeLock()
{
	if (m_isUnlocked)
	{

	}
	else
	{
		(m_lock.*m_ptr)();
	}
}
template<typename Mutex, typename UseMember>
inline UseMember * ScopeLock<Mutex, UseMember>::operator->()
{
	return m_ref;
}

template<typename Mutex, typename UseMember>
inline const UseMember * ScopeLock<Mutex, UseMember>::operator->() const
{
	return (const UseMember *) m_ref;
}

template<typename Mutex, typename UseMember>
inline void ScopeLock<Mutex, UseMember>::Return()
{
	m_ref = nullptr;
	(m_lock.*m_ptr)();
	m_isUnlocked = true;
}

}

