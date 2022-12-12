#pragma once

#include <vector>
#include <array>

#include "config.hpp"
#include "chunk.hpp"

namespace ic_server
{

class block;

class IC_SERVER_API level
{
private:
	std::vector<chunk> chunks_;

public:
	level& operator=(level&) = delete;

public:
	chunk const* at(int x, int y) const;
	chunk* at(int x, int y);
	block* at(int x, int y, int z);
};

}
