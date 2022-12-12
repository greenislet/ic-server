#pragma once

#include <memory>
#include <list>

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>

#include "config.hpp"

namespace ic_server
{
class server;

class IC_SERVER_API connection : public std::enable_shared_from_this<connection>
{
private:
	enum class state
	{
		none,
		ready,
		handshaking,
		authenticating,
		reading,
		writing,
		to_be_closed,
		closing,
		closed
	};

	std::string                                                   addr_str_;
	boost::beast::websocket::stream<boost::asio::ip::tcp::socket> socket_;
	state                                                         state_;
	boost::asio::io_context::strand                               strand_;
	boost::beast::multi_buffer                                    read_buffer_;
	std::list<std::shared_ptr<std::string const>>                 to_write_;

public:
	connection(boost::asio::ip::tcp::socket&& socket, boost::asio::io_context& ioc);

public:
	void start();
	void on_accept(boost::system::error_code const& ec) noexcept;
	void write(std::shared_ptr<std::string const> msg);
	void write_next();
	void on_write(boost::beast::error_code const& ec, std::size_t const& bytes_transferred) noexcept;
	void do_read();
	void on_read(boost::system::error_code const& ec, std::size_t const& bytes_transferred) noexcept;
	void interpret(std::string const& order_str);
};
}
