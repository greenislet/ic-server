#include <spdlog/spdlog.h>

namespace ic_server
{

class log
{
private:
	static log log_;

private:
	log()
	{
		spdlog::set_pattern("[%H:%M:%S.%f] [%5t] %v");
	}
};

log log::log_;

}
