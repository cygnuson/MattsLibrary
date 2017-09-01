#pragma once

#include <cstdint>
#include <type_traits>

namespace cg {
/**
value is FALSE for little endian systems, and TRUE for big endian systems.
*/
class Endian {
public:
	/**The value `little` is true on little endian system and false on big
	endian systems.
	*/
	const static bool little;
	/**The value `big` is false on little endian system and true on big
	endian systems.
	*/
	const static bool big;
	/**make sure that the endian of a variable is the same is this system.
	\param data A reference to the data.
	\param isLittle True to signal that the data is already in little endian.*/
	template<typename T>
	static void MakeHostOrder(T& data,
		bool isLittle);
private:
	const static uint32_t __ENDIANNESS_CHECKER;
};

template<typename T>
inline void Endian::MakeHostOrder(T & data, 
	bool isLittle)
{
	static_assert(std::is_fundamental<T>::value || std::is_enum<T>::value,
		"The type must be fundamental.");
	/**If host and data are already the same, do nothing.*/
	if (Endian::little == isLittle)
		return;
	
	char* ptr = (char*) &data;
	static const std::size_t TypeSize = sizeof(T);
	for (std::size_t i = 0; i < (TypeSize / 2); ++i)
	{
		char buffer;
		/*The left size of the swap*/
		char* fSwap = ptr + i;
		/*The right side of the swap*/
		char* sSwap = ptr + TypeSize - 1 - i;
		/*do swaps*/
		buffer = *fSwap;
		*fSwap = *sSwap;
		*sSwap = buffer;
	}

}

}
