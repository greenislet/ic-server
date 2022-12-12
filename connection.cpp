#include <iostream>
#include <codecvt>

#include <nlohmann/json.hpp>

#include "connection.hpp"

#include "log.hpp"
#include "server.hpp"

using namespace std::string_literals;

namespace beast = boost::beast;
namespace asio = boost::asio;

namespace ic_server
{

connection::connection(boost::asio::ip::tcp::socket&& socket, boost::asio::io_context& ioc)
	: addr_str_(socket.remote_endpoint().address().to_string() + ":" + std::to_string(socket.remote_endpoint().port()))
	, socket_(std::move(socket))
	, strand_(ioc)
	, state_(state::none)
{
	state_ = state::ready;
	socket_.auto_fragment(true);
}
void connection::start()
{
	socket_.async_accept(asio::bind_executor(strand_, std::bind(&connection::on_accept, shared_from_this(), std::placeholders::_1)));
	state_ = state::handshaking;
}

void connection::on_accept(boost::system::error_code const& ec) noexcept
{
	if (ec)
	{
		ic_server_log("error on accept: {}", addr_str_, ec.message());;
		return;
	}

	ic_server_log("accept successful", addr_str_);

	state_ = state::authenticating;

	do_read();
}

void connection::write(std::shared_ptr<std::string const> msg)
{
	to_write_.emplace_back(msg);

	if (to_write_.size() == 1)
		asio::post(socket_.get_executor(), asio::bind_executor(strand_, std::bind(&connection::write_next, shared_from_this())));
}

void connection::write_next()
{
	if (to_write_.empty())
		return;

	socket_.async_write(asio::buffer(*to_write_.front()), asio::bind_executor(strand_, std::bind(&connection::on_write, shared_from_this(), std::placeholders::_1, std::placeholders::_2)));
	state_ = state::writing;
}

void connection::on_write(beast::error_code const& ec, std::size_t const& bytes_transferred) noexcept
{
	to_write_.pop_front();

	if (ec)
		return;

	if (state_ != state::writing)
		return;

	state_ = state::reading;

	write_next();
}

void connection::do_read()
{
	socket_.async_read(read_buffer_, asio::bind_executor(strand_, std::bind(&connection::on_read, shared_from_this(), std::placeholders::_1, std::placeholders::_2)));
}

void connection::on_read(boost::system::error_code const& ec, std::size_t const& bytes_transferred) noexcept
{
	if (ec == beast::websocket::error::closed)
	{
		ic_server_log("graceful close", addr_str_);
		return;
	}

	ic_server_log("read successful: {} bytes read", addr_str_, bytes_transferred);

	std::string order_str = boost::beast::buffers_to_string(read_buffer_.data());

	read_buffer_.consume(read_buffer_.size());

	interpret(order_str);

	do_read();
}

void connection::interpret(std::string const& order_str)
{
	nlohmann::json receivedJSON;
	try {
		receivedJSON = nlohmann::json::parse(order_str);
	}
	catch (...) {
		ic_server_log("error on interpret: invalid JSON", addr_str_);
	}

	if (state_ == state::authenticating)
	{
		if (!receivedJSON.contains("playerName"))
		{
			ic_server_log("authenticating: no playerName found in order", addr_str_);
			return;
		}

		std::string playerName = receivedJSON["playerName"];

		ic_server_log("authenticating: player name is {}", addr_str_, playerName);

		nlohmann::json responseJSON;
		responseJSON["authenticated"] = true;
		responseJSON["spawnPosition"]["x"] = 1;
		responseJSON["spawnPosition"]["y"] = 1;
		responseJSON["spawnPosition"]["z"] = 1;

		write(std::make_shared<std::string>(responseJSON.dump()));
		return;
	}
}
}
