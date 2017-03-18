#pragma once

#include <cstddef>
#include <string>

namespace cg {


/**Hash an. The paramter T for this object must be fundamental or have a
.Hash() const; function.
\param obj The object to hash.
\tparam T A fundamental/std::string object or an object with .Hash() const; 
implimented.
\return A 64bit hash number. Not fit for security purposes.*/
inline std::size_t Hash(const std::string& str)
{
	std::size_t hash = 31;
	for (auto i = 0; str[i] != 0; ++i)
		hash += str[i] * 59;
	return hash;
}
/**Hash an. The paramter T for this object must be fundamental or have a
.Hash() const; function.
\param obj The object to hash.
\tparam T A fundamental/std::string object or an object with .Hash() const;
implimented.
\return A 64bit hash number. Not fit for security purposes.*/
template<typename T>
inline std::enable_if_t<std::is_fundamental<T>::value, std::size_t>
Hash(const T& obj)
{
	return obj * 59;
}
/**Hash an. The paramter T for this object must be fundamental or have a
.Hash() const; function.
\param obj The object to hash.
\tparam T A fundamental/std::string object or an object with .Hash() const;
implimented.
\return A 64bit hash number. Not fit for security purposes.*/
template<typename T>
inline std::enable_if_t<!std::is_fundamental<T>::value, std::size_t>
Hash(const T& obj)
{
	return T.Hash() * 59;
}

}