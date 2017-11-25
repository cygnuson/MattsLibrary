/*

(C) Matthew Swanson

This file is part of ColdStorage.

ColdStorage is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
any later version.

ColdStorage is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with ColdStorage.  If not, see <http://www.gnu.org/licenses/>.

*/
#pragma once


#define Move std::move
#define Forward std::forward

namespace cg {

/**The data type that will be used for the size of things.*/
using SizeType = decltype((sizeof(int)));
/**The type for differences of poitners*/
using DiffType = decltype(((int*)10) - ((int*)4));

/**Runtime power*/
inline SizeType RTPower(SizeType num, SizeType pow)
{
    if (pow == 0)
        return 1;
    auto oNum = num;
    while (pow > 1)
    {
        num *= oNum;
        --pow;
    }
    return num;
}
/**Compiletime power operator.
*
*Get the power at compile time.
*
*\tparam num The number that is the base of the operation.
*\tparam power The power in which to raise the base.
*/
template<SizeType num, SizeType power>
class Power {
private:
    /**\sa Power*/
    template<SizeType num, SizeType power>
    class Pow {
    public:
        static const SizeType value = num* Pow<num, power - 1>::value;
    };
    /**\sa Power*/
    template<SizeType num>
    class Pow<num, 0> {
    public:
        /**The value of this portion.*/
        static const SizeType value = 1;
    };
public:
    /**The value of this portion.*/
    static const SizeType value = num* Pow<num, power - 1>::value;
};

/**\sa Power*/
template<SizeType num>
class Power<num, 0>
{
public:
    /**The value of this portion.*/
    static const SizeType value = 1;
};

/**\sa Power*/
template<SizeType N, SizeType I = 0>
class IsPowerOf2
{
public:
    /**A compile time chained value*/
    const static SizeType Power = cg::Power<2, I>::value;
    /**The value of this portion.*/
    const static bool value = (N == Power) ?
        true :
        IsPowerOf2<N, I + 1>::value;
};

/**\sa Power*/
template<SizeType N>
class IsPowerOf2<N, ((sizeof(std::size_t) * 8) - 1)>
{
public:
    /**The value of this portion.*/
    const static bool value =
        N == cg::Power<2, MaxPowerOf2>::value;
};

/**Conditionally determine a type. If _A is true, The \p Type typedef is _A_T
otherwise its _NOT_AT
\tparam _A a boolean value to determine the Type member type def.
\tparam _A_T The type if _A is true.
\tparam _NOT_A_T The type if _A is false.*/
template<bool _A, typename _A_T, typename _NOT_A_T>
class ConditionalTypeImpl {};

/**Conditionally determine a type. If _A is true, The \p Type typedef is _A_T
otherwise its _NOT_AT
\tparam _A a boolean value to determine the Type member type def.
\tparam _A_T The type if _A is true.
\tparam _NOT_A_T The type if _A is false.*/
template<typename _A_T, typename _NOT_A_T>
class ConditionalTypeImpl<true, _A_T, _NOT_A_T>
{
public:
    using Type = _A_T;
};

/**Conditionally determine a type. If _A is true, The \p Type typedef is _A_T
otherwise its _NOT_AT
\tparam _A a boolean value to determine the Type member type def.
\tparam _A_T The type if _A is true.
\tparam _NOT_A_T The type if _A is false.*/
template<typename _A_T, typename _NOT_A_T>
class ConditionalTypeImpl<false, _A_T, _NOT_A_T>
{
public:
    using Type = _NOT_A_T;
};

/**Conditionally determine a type. If _A is true, The \p Type typedef is _A_T
otherwise its _NOT_AT
\tparam _A a boolean value to determine the Type member type def.
\tparam _A_T The type if _A is true.
\tparam _NOT_A_T The type if _A is false.*/
template<bool _A, typename _A_T, typename _NOT_A_T>
using ConditionalType = typename ConditionalTypeImpl<_A, _A_T, _NOT_A_T>::Type;

/**A generic pair class.
\tparam A The first type.
\tparam B The second type*/
template<typename A, typename B>
struct Pair
{
    /**Create a pair.
    \param a The first item.
    \param b The second item.*/
    Pair(A&& a, B&& b) :m_a(Forward<A>(a)), m_b(Forward<B>(b)) {}
    Pair(const Pair<A, B>& o) :m_a(o.m_a), m_b(o.m_b) {}
    Pair(Pair<A, B>&& o) :m_a(Move(o.m_a)), m_b(Move(o.m_b)) {}
    Pair() {}
    Pair<A, B>& operator=(const Pair<A, B>& o) {
        if (this == &o)
            return *this;
        m_a = o.m_a;
        m_b = o.m_b;
        return *this;
    }
    Pair<A, B>& operator=(Pair<A, B>&& o) {
        if (this == &o)
            return *this;
        m_a = Move(o.m_a);
        m_b = Move(o.m_b);
        return *this;
    }
    A m_a;
    B m_b;
};
/**Create a pair.
\param a The first item.
\param b The second item.
\return A pair object with a nd b as members.*/
template<typename A, typename B>
Pair<A, B> MakePair(A&& a, B&& b)
{
    return Pair<A, B>(Forward<A>(a), Forward<B>(b));
}

/**Remove a reference from a type.*/
template<typename T>
struct RemoveReferenceImpl
{
    using Type = T;
};
/**Remove a reference from a type.*/
template<typename T>
struct RemoveReferenceImpl<T&>
{
    using Type = T;
};
/**Remove a reference from a type.*/
template<typename T>
struct RemoveReferenceImpl<T&&>
{
    using Type = T;
};
/**Remove a reference from a type.*/
template<typename T>
using RemoveReference = typename RemoveReferenceImpl<T>::Type;


/**Swap two things with moves.
\param a The first thing.
\param b The second thing.*/
template<typename T>
void Swap(T&& a, T&& b)
{
    RemoveReference<T> temp = Forward<T>(a);
    a = Forward<T>(b);
    b = Forward<T>(temp);
}

/**Less functor*/
template<typename T>
struct Less
{
    /**Compare a with b.
    \param a The first item.
    \param b The second item.
    \return True if a should preceed b.*/
    inline bool operator()(const T& a, const T& b) const
    {
        return a < b;
    }
};
/**Greaterthan functor*/
template<typename T>
struct Greater
{
    /**Compare a with b.
    \param a The first item.
    \param b The second item.
    \return True if a should preceed b.*/
    inline bool operator()(const T& a, const T& b) const
    {
        return a > b;
    }
};

}