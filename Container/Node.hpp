#pragma once

namespace cg {

template<typename T>
struct Node
{
	/**default ctor*/
	Node() {};
	/**The type of value contained*/
	using value_type = T;
	/**The type of value contained*/
	using ValueType = value_type;
	/**A reference to the value type.*/
	using reference = value_type&;
	/**A reference to the value type.*/
	using Reference = reference&;
	/**A poitner of value_type*/
	using pointer = value_type*;
	/**A poitner of value_type*/
	using Pointer = pointer*;
	/**the const ref type.*/
	using const_reference = const value_type&;
	/**the const ref type.*/
	using ConstReference = const_reference;
	/**the const pointer type.*/
	using const_pointer = const value_type*;
	/**the const pointer type.*/
	using ConstPointer = const_pointer;
	/**The type gotten when doing math on poitners.*/
	using difference_type = std::ptrdiff_t;
	/**The type gotten when doing math on poitners.*/
	using DifferenceType = difference_type;
};

}