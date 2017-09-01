#pragma once


namespace cg {
/**Quickly disable copies.*/
struct NoCopy
{
	NoCopy() {};
	NoCopy(const NoCopy& other) = delete;
	void operator=(const NoCopy& other) = delete;
};

/**Quickly disable moves.*/
struct NoMove
{
	NoMove() {};
	NoMove(NoMove&& other) = delete;
	void operator=(NoMove&& other) = delete;
};

}