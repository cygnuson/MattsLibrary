#pragma once


namespace cg {
namespace math {

class Object : public cg::Stringable
{
public:
	/**The type of object.*/
	enum Type : uint8_t
	{
		/**Type unknown.*/
		Invalid,
		/**A plain number.*/
		Number,
		/**A variable type*/
		Variable,
		/**A group of Number, Variable, and Term.*/
		Term,
		/**A group of terms added together.*/
		Expression,
		/** Two rationals that equal eachother.*/
		Equation,
		/**An expression with a denominator expression.*/
		Rational,
	};
	/**The dtor*/
	virtual ~Object() {};
	/**Determine if the object is equivalent to one.
	\return True if the object is one.*/
	virtual bool IsOne() const = 0;
	/**Determine if the object is zero.
	\return True if the object is zero.*/
	virtual bool IsZero() const = 0;
	/**Get the type id of the object.
	\return The enum that is the type of the object.*/
	virtual Object::Type TypeOf() const = 0;

};

/**Determine if an object is one.
\return TRue if the object evaulates to one.*/
template<typename T>
bool IsOne(const T& o)
{
	return o == 1;
}
/**Determine if an object is one.
\return TRue if the object evaulates to one.*/
inline bool IsOne(const Object& o)
{
	return o.IsOne();
}
/**Determine if an object is zero.
\return TRue if the object evaulates to zero.*/
template<typename T>
bool IsZero(const T& o)
{
	return o == 1;
}
/**Determine if an object is zero.
\return TRue if the object evaulates to zero.*/
inline bool IsZero(const Object& o)
{
	return o.IsOne();
}

}
}