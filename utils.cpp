#include "utils.hpp"
#include "chunk.hpp"
#include "blocks.hpp"

namespace ic_server
{

block* all_dirt(int x, int y, int z)
{
	return new dirt;
}

std::pair<int, int> local_to_world(int chunkX, int chunkY)
{
	int worldX = chunkX * chunk::width;
	int worldY = chunkY * chunk::length;

	return std::make_pair(worldX, worldY);
}

}
