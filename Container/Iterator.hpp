#pragma once

#include "../Memory.hpp"

namespace cg {

template<typename T>
class IteratorBase
{
public:
	/**The type of value contained*/
	using value_type = T;
	/**A reference to the value type.*/
	using reference = value_type&;
	/**A poitner of value_type*/
	using pointer = value_type*;
	/**the const ref type.*/
	using const_reference = const reference;
	/**the const pointer type.*/
	using const_pointer = const pointer;
	/**The type gotten when doing math on poitners.*/
	using difference_type = std::ptrdiff_t;

private:

};


}




