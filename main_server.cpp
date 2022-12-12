#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <string>
#include <thread>


#include <spdlog/spdlog.h>

#include <nlohmann/json.hpp>

#include "server.hpp"

namespace beast = boost::beast;
namespace asio = boost::asio;
using tcp = boost::asio::ip::tcp;

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		std::cerr <<
			"Usage: ic-server <port>\n" <<
			"Example:\n" <<
			"    ic-server 12000\n";
		return EXIT_FAILURE;
	}
	auto const port = static_cast<unsigned short>(std::atoi(argv[1]));

	int nb_threads = std::thread::hardware_concurrency();

	asio::io_context ioc{ nb_threads };

	std::shared_ptr<ic_server::server> server = std::make_shared<ic_server::server>(ioc, port);

	server->start();

	std::vector<std::thread> network_threads;

	for (unsigned int i = 0; i < nb_threads; ++i)
		network_threads.emplace_back([&ioc, i]
			{
				//std::cout << "THREAD # " << i + 1 << " RUNNING" << std::endl;
				try {
					ioc.run();
				}
				catch (std::exception const& e) {
					std::cerr << "main: exception thrown: " << e.what() << std::endl;
				}
				//std::cout << "THREAD # " << i + 1 << " STOPPED" << std::endl;
			});

	for (std::thread& thread : network_threads)
		thread.join();

}
