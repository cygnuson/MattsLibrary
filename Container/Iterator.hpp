#pragma once

#include "../Memory.hpp"

namespace cg {

template<typename T>
class IteratorBase
{
public:
	/**The type of value contained*/
	using value_type = typename T::value_type;
	/**A reference to the value type.*/
	using reference = value_type&;
	/**A poitner of value_type*/
	using pointer = value_type*;
	/**the const ref type.*/
	using const_reference = const value_type&;
	/**the const pointer type.*/
	using const_pointer = const value_type*;
	/**The type gotten when doing math on poitners.*/
	using difference_type = std::ptrdiff_t;


};


}




