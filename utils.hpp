#pragma once

#include <utility>

#include "config.hpp"

namespace ic_server
{

class block;

IC_SERVER_API block* all_dirt(int x, int y, int z);

IC_SERVER_API std::pair<int, int> local_to_world(int chunkX, int chunkY);

}
