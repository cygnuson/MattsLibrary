#pragma once

#include <map>

#include "Serial.hpp"

namespace cg {
	
template<typename MType, typename MKey>
void Push(cg::Serial& s, const std::map<MKey, MType>& map)
{
	uint64_t sz = map.size();
	s << sz;
	auto it = map.begin();
	auto end = map.end();
	for (; it != end; ++it)
	{
		Push(s, it->first);
		Push(s, it->second);
	}
}
template<typename MType, typename MKey>
void Pull(cg::Serial& s, std::map<MKey, MType>& map)
{
	map.swap(std::map<MKey, MType>());
	uint64_t size = 0;
	s >> size;
	for (uint64_t i = 0; i < size; ++i)
	{
		MKey key;
		MType data;
		Pull(s, key);
		Pull(s, data);
		map[key] = data;
	}
}


}