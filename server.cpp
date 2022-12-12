#include "server.hpp"

#include "log.hpp"
#include "connection.hpp"

namespace beast = boost::beast;
namespace asio = boost::asio;

namespace ic_server
{

	server::server(asio::io_context& ioc, unsigned short port)
		: ioc_(ioc)
		, local_endpoint_(asio::ip::tcp::endpoint(asio::ip::tcp::v6(), port))
		, acceptor_(ioc_)
		, new_client_socket_(ioc_)
	{}

	void server::start_game()
	{

	}

	void server::start_network()
	{
		acceptor_.open(local_endpoint_.protocol());
		acceptor_.set_option(boost::asio::socket_base::reuse_address(true));
		acceptor_.bind(local_endpoint_);
		acceptor_.listen();

		new_client_socket_ = asio::ip::tcp::socket(ioc_);

		do_accept();
	}


	void server::on_accept(const boost::system::error_code& ec) noexcept
	{
		ic_server_log("new tcp connection", "server");

		auto new_conn = std::make_shared<connection>(std::move(new_client_socket_), ioc_);

		connections_.emplace_back(new_conn);

		new_conn->start();

		do_accept();
	}

	void server::do_accept()
	{
		acceptor_.async_accept(new_client_socket_, std::bind(&server::on_accept, shared_from_this(), std::placeholders::_1));
	}

}