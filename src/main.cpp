#include "gpio.h"
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <iostream>

namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;
namespace beast = boost::beast;
namespace websocket = beast::websocket;

struct message_data
{
	u32 type;
	f32 left;
	f32 right;
};

void do_session(tcp::socket socket)
{
	try
	{
		// Accept the websocket handshake
		websocket::stream<tcp::socket> ws{std::move(socket)};
		ws.accept();

		while (true)
		{
			// Block until a message is read
			beast::flat_buffer buffer;
			ws.read(buffer);

			// Handle the message
			auto data = *static_cast<message_data*>(buffer.data().data());
			gpio::set_speed(gpio::motor::left, data.left);
			gpio::set_speed(gpio::motor::right, data.right);
		}
	}
	catch (const beast::system_error& se)
	{
		if (se.code() != websocket::error::closed)
			throw;
	}
}

i32 main(i32 argc, char** argv)
{
	if (argc != 3)
	{
		std::cerr << "Usage: " << argv[0] << " <address> <port>\n";
		return EXIT_FAILURE;
	}

	gpio::init();

	// Initialize Beast
	const net::ip::address address{net::ip::make_address(argv[1])};
	const auto port = static_cast<u16>(std::atoi(argv[2]));
	net::io_context ioc{1};
	tcp::acceptor acceptor{ioc, {address, port}};

	while (true)
	{
		// Block untill connection
		tcp::socket socket{ioc};
		acceptor.accept();

		// Launch a session
		std::thread(&do_session, std::move(socket)).detach();
	}

	gpio::shutdown();
}
