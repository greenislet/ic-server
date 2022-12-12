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
		, game_cycle_timer_(ioc)
	{}

	void server::start_game()
	{
		wake_time_ = std::chrono::ceil<std::chrono::seconds>(std::chrono::steady_clock::now());

		game_cycle_timer_.expires_at(wake_time_);
		game_cycle_timer_.async_wait(std::bind(&server::game_cycle, shared_from_this(), std::placeholders::_1, 0));

		ic_server_log("first game cycle in {}s with tick duration of {}s", "game",
			std::chrono::duration_cast<std::chrono::duration<float>>(wake_time_ - std::chrono::steady_clock::now()).count(),
			std::chrono::duration_cast<std::chrono::duration<float>>(tick_duration_).count());
	}

	void server::game_cycle(boost::system::error_code const& error, unsigned int nb_ticks)
	{
		ic_server_log("new cycle: {} ticks elapsed", "game", nb_ticks);

		

		// Prepare next call
		nb_ticks = 0;
		while (wake_time_ < std::chrono::steady_clock::now())
		{
			wake_time_ += tick_duration_;
			++nb_ticks;
		}
		assert(nb_ticks >= 1);

		if (nb_ticks > 1)
			ic_server_log("end of cycle: retard of {}", "game", nb_ticks - 1);

		//LOG("GAME LOOP", "NEXT CYCLE AT " << std::chrono::duration_cast<std::chrono::duration<float>>(wake_time_ - start_time_).count());
		game_cycle_timer_.expires_at(wake_time_);
		game_cycle_timer_.async_wait(std::bind(&server::game_cycle, shared_from_this(), std::placeholders::_1, nb_ticks));
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