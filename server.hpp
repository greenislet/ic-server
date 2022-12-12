#pragma once

#include <chrono>
#include <memory>
#include <list>

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/steady_timer.hpp>

#include "config.hpp"

namespace ic_server
{
	class connection;

	class IC_SERVER_API server : public std::enable_shared_from_this<server>
	{
	private:
		boost::asio::io_context&			   ioc_;
		std::chrono::steady_clock::duration    tick_duration_;
		boost::asio::ip::tcp::endpoint		   local_endpoint_;
		boost::asio::ip::tcp::acceptor		   acceptor_;
		boost::asio::ip::tcp::socket		   new_client_socket_;
		std::list<std::shared_ptr<connection>> connections_;
		std::chrono::steady_clock::time_point  wake_time_;
		boost::asio::steady_timer              game_cycle_timer_;

	public:
		server(boost::asio::io_context& ioc, unsigned short port);

		template<class Rep = int, class Per = std::milli>
		void start(std::chrono::duration<Rep, Per> const& tick_duration = std::chrono::duration<Rep, Per>(250))
		{
			tick_duration_ = std::chrono::duration_cast<decltype(tick_duration_)>(tick_duration);

			start_game();

			start_network();
		}

		void start_game();

		void game_cycle(boost::system::error_code const& error, unsigned int nb_ticks);

		void start_network();

		void on_accept(const boost::system::error_code& ec) noexcept;

		void do_accept();
	};
}