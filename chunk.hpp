#pragma once

#include <array>
#include <functional>
#include <concepts>

#include "config.hpp"
#include "utils.hpp"

namespace ic_server
{

class block;
class level;

class IC_SERVER_API chunk
{
public:
	static const int width = 10;
	static const int length = 10;
	static const int height = 100;

private:
	std::array<std::array<std::array<block*, chunk::height>, chunk::length>, chunk::width> blocks_;
	int x_;
	int y_;
	level const& level_;

public:
	template<class gen>
		requires std::invocable<gen, int, int, int>
	chunk(int x, int y, level const& level, gen const& generator = all_dirt)
		: x_(x)
		, y_(y)
		, level_(level)
	{
		std::pair<int, int> pair = local_to_world(x_, y_);

#ifndef NDEBUG
		for (int i = 0; i < width; ++i)
			for (int j = 0; j < length; ++j)
				for (int k = 0; k < height; ++k)
					blocks_[i][j][k] = nullptr;
#endif
		for (int i = 0; i < width; ++i)
			for (int j = 0; j < length; ++j)
				for (int k = 0; k < height; ++k)
					blocks_[i][j][k] = generator(pair.first + i, pair.second + j, k);
	}

	chunk& operator=(chunk&) = delete;

public:
	block* at(int x, int y, int z);
	int x() const;
	int y() const;
};

}
