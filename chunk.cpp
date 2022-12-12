#include "chunk.hpp"

namespace ic_server
{

block* chunk::at(int x, int y, int z)
{
	return blocks_[x][y][z];
}

int chunk::x() const
{
	return x_;
}

int chunk::y() const
{
	return y_;
}

}
