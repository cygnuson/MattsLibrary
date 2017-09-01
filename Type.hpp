#pragma once

#include <limits>
#include <cstdint>

namespace cg {

/**Get a type that is exactly one demotion of T.
*
*The type member is the numerical type that is exactly one demotions of T.
*
*\tparam T The type for which to get the demoted type.
*/
template<typename T>
class DemoteType {
public:
	/**The type that has been demoted.*/
	using type = T;
	/**Get the max value of a type.
	\return The max value.*/
	constexpr std::size_t Max() const {
		return std::numeric_limits<type>::max();
	}
};
/**\sa DemoteType*/
template<>
class DemoteType<uint64_t> {
public:
	/**The type that has been demoted.*/
	using type = uint32_t;
	/**Get the max value of a type.
	\return The max value.*/
	constexpr std::size_t Max() const {
		return std::numeric_limits<type>::max();
	}
};
/**\sa DemoteType*/
template<>
class DemoteType<uint32_t> {
public:
	/**The type that has been demoted.*/
	using type = uint16_t;
	/**Get the max value of a type.
	\return The max value.*/
	constexpr std::size_t Max() const {
		return std::numeric_limits<type>::max();
	}
};
/**\sa DemoteType*/
template <>
class DemoteType<uint16_t> {
public:
	/**The type that has been demoted.*/
	using type = uint8_t;
	/**Get the max value of a type.
	\return The max value.*/
	constexpr std::size_t Max() const {
		return std::numeric_limits<type>::max();
	}
};

/**Get a type that is exactly one promotion of T.
*
*The type member is the numerical type that is exactly one promotion of T. It
is the callers responsibility to ensure that there exists a type that is larger
than T.
*
*\tparam T The type for which to get the promoted type.
*/
template<typename T>
class PromoteType {
public:
	/**The type that has been promoted.*/
	using type = T;
	/**Get the max value of a type.
	\return The max value.*/
	constexpr std::size_t Max() const {
		return std::numeric_limits<type>::max();
	}
};
/**\sa PromoteType*/
template<> class PromoteType<uint32_t> {
public:
	/**The type that has been promoted.*/
	using type = uint64_t;
	/**Get the max value of a type.
	\return The max value.*/
	constexpr std::size_t Max() const {
		return std::numeric_limits<type>::max();
	}
};
/**\sa PromoteType*/
template<> class PromoteType<uint16_t> {
public:
	/**The type that has been promoted.*/
	using type = uint32_t;
	/**Get the max value of a type.
	\return The max value.*/
	constexpr std::size_t Max() const {
		return std::numeric_limits<type>::max();
	}
};
/**\sa PromoteType*/
template<> class PromoteType<uint8_t> {
public:
	/**The type that has been promoted.*/
	using type = uint16_t;
	/**Get the max value of a type.
	\return The max value.*/
	constexpr std::size_t Max() const {
		return std::numeric_limits<type>::max();
	}
};

}