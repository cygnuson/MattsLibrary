#pragma once


namespace cg {

struct NoCopy
{
	NoCopy() {};
	NoCopy(const NoCopy& other) = delete;
	void operator=(const NoCopy& other) = delete;
};

struct NoMove
{
	NoMove() {};
	NoMove(NoMove&& other) = delete;
	void operator=(NoMove&& other) = delete;
};

}