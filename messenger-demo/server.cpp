#include <iostream>
#include <vector>
#include <thread>
#include "tcp_friend/tcp_friend.hpp"
using namespace tcp_friend;

#define SERVER_PORT 4554

std::vector<std::shared_ptr<tcp_connection_socket>> g_connections;

void clientListener(std::shared_ptr<tcp_connection_socket> connection) {
	while (connection->is_alive()) {
		// Receive a message from the client
		std::string msg;
		if (connection->recv_string(msg)) {
			printf("[*] Client said '%s'\n", msg.c_str());

			// Broadcasting the message
			for (auto& conn : g_connections) {
				if (conn->is_alive()) {
					conn->send_string(msg);
				}
			}
		}
	}

	connection->close();
	printf("[*] %s disconnected\n", connection->get_address().c_str());

	for (auto& conn : g_connections) {
		if (conn->is_alive()) {
			conn->send_string("* " + connection->get_address() + " disconnected *");
		}
	}
}

int main() {
    // Bind the server socket
	tcp_server_socket server_socket;
	if (!server_socket.bind("0.0.0.0", SERVER_PORT)) {
		printf("Failed to bind server socket to port %i\n", SERVER_PORT);
		server_socket.close();
		return 1;
	}

	printf("[*] Listening for connections...\n");

	// Start listening for 1 incoming client connection
	if (!server_socket.listen(64)) {
		printf("Failed to listen on port %i\n", SERVER_PORT);
		server_socket.close();
		return 1;
	}

	// Accept incoming connections
	while (true) {
		auto connection = server_socket.accept();

		if (!connection->is_alive()) {
			printf("Failed to accept a connection on port %i\n", SERVER_PORT);
			printf("Restarting the listener...\n");
			
			if (!server_socket.listen(64)) {
				printf("Failed to listen on port %i\n", SERVER_PORT);
				server_socket.close();
				return 1;
			}

			continue;
		}

		printf("'%s' connected on port %i\n", connection->get_address().c_str(), connection->get_port());

		g_connections.push_back(connection);
		
		std::thread clientListeningThread(clientListener, g_connections.back());
		clientListeningThread.detach();
	}

	server_socket.close();
	return 0;
}