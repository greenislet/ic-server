#include "level.hpp"

namespace ic_server
{

chunk* level::at(int x, int y)
{
	for (chunk& chunk : chunks_)
		if (chunk.x() == x && chunk.y() == y)
			return &chunk;
	return nullptr;
}

block* level::at(int x, int y, int z)
{
	int chunk_x = x / chunk::width;
	if (x < 0)
		--chunk_x;

	int chunk_y = y / chunk::width;
	if (y < 0)
		--chunk_y;

	int block_x = x % chunk::width;

	int block_y = y % chunk::height;

	chunk* chunk = at(chunk_x, chunk_y);
	
	if (chunk == nullptr)
		return nullptr;

	return chunk->at(block_x, block_y, z);
}

}
