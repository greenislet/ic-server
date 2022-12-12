#include <cstdlib>
#include <iostream>
#include <string>
#include <future>
#include <chrono>

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <spdlog/spdlog.h>

#include <nlohmann/json.hpp>
#include <gtest/gtest.h>

#include "level.hpp"
#include "blocks.hpp"
#include "server.hpp"

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

using namespace std::chrono_literals;

TEST(all, level)
{
	ic_server::level level;
	ic_server::chunk chunk(-42, 24, level, ic_server::all_dirt);

	ASSERT_EQ(chunk.x(), -42);
	ASSERT_EQ(chunk.y(), 24);

	for (int i = 0; i < ic_server::chunk::width; ++i)
		for (int j = 0; j < ic_server::chunk::length; ++j)
			for (int k = 0; k < ic_server::chunk::height; ++k)
			{
				ic_server::block* block = chunk.at(i, j, k);
				ASSERT_NE(block, nullptr);
				ic_server::dirt* dirt = dynamic_cast<ic_server::dirt*>(block);
				ASSERT_TRUE(dirt);
			}
}

TEST(all, connection)
{
	net::io_context ioc;

	std::shared_ptr<ic_server::server> server = std::make_shared<ic_server::server>(ioc, 12000);

	auto fut = std::async(std::launch::async, [server, &ioc]
		{
			server->start();
			ioc.run_for(500ms);
		});

	tcp::resolver resolver{ ioc };
	websocket::stream<tcp::socket> ws{ ioc };

	auto const results = resolver.resolve("localhost", "12000");

	ASSERT_NO_THROW(auto ep = net::connect(ws.next_layer(), results));
	ASSERT_NO_THROW(ws.handshake("0.0.0.0", "/"));
	ASSERT_NO_THROW(ws.write(net::buffer(std::string("{\"playerName\":\"testPlayer\"}"))));
	beast::flat_buffer buffer;
	ASSERT_NO_THROW(ws.read(buffer));
	ASSERT_NO_THROW(ws.close(websocket::close_code::normal));

	std::string str = beast::buffers_to_string(buffer.data());

	nlohmann::json data;
	
	ASSERT_NO_THROW(data = nlohmann::json::parse(str));

	ASSERT_TRUE(data.contains("authenticated"));
	ASSERT_TRUE(data.contains("spawnPosition"));
	ASSERT_EQ(data["authenticated"].type(), nlohmann::detail::value_t::boolean);

	ASSERT_NO_THROW(fut.get());
}

int main(int argc, char** argv, char** env)
{
	testing::InitGoogleTest(&argc, argv);

	spdlog::set_pattern("[%H:%M:%S %z] [%n] [%^---%L---%$] [thread %t] %v");

	return RUN_ALL_TESTS();
}
