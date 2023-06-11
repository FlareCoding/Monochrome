#include "tcp_friend.hpp"

#ifdef WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#endif

namespace tcp_friend {
	static bool g_is_initialized = false;

	void tcp_socket_base_impl::initialize() {
		g_is_initialized = true;

#ifdef WIN32
		WSAData wsdata;
		int result = WSAStartup(MAKEWORD(2, 2), &wsdata);
		if (result != 0) {
			printf("Failed to initialize WinSock2\n");
			exit(1);
		}
#endif
	}

	void tcp_socket_base_impl::close(uint64_t handle) {
#ifdef WIN32
		closesocket(handle);
#else
		::close(handle);
#endif
	}

	void tcp_socket_base_impl::set_blocking_state_impl(uint64_t handle, int blocking) {
#ifdef WIN32
		unsigned long mode = (unsigned long)blocking;
		ioctlsocket(handle, FIONBIO, &mode);
#else
		int flags = fcntl(static_cast<int>(handle), F_GETFL, 0);
		if (flags == -1) {
			printf("Error retrieving current socket blocking state\n");
			return;
		}
		
		flags = blocking ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK);
		
		int result = fcntl(static_cast<int>(handle), F_SETFL, flags);
		if (result == -1) {
			printf("Failed to set socket blocking state\n");
			return;
		}
#endif
	}
	
	int tcp_socket_base_impl::send_bytes_impl(uint64_t handle, char* buffer, size_t size) {
#ifdef WIN32
		return send(handle, buffer, static_cast<int>(size), 0);
#else
		return send(static_cast<int>(handle), buffer, size, 0);
#endif
	}

	int tcp_socket_base_impl::recv_bytes_impl(uint64_t handle, char* buffer, size_t allowed_buffer_size) {
#ifdef WIN32
		return recv(handle, buffer, static_cast<int>(allowed_buffer_size), 0);
#else
		return recv(static_cast<int>(handle), buffer, allowed_buffer_size, 0);
#endif
	}

	uint64_t tcp_socket_base_impl::server_bind_impl(const std::string& ipv4, int port) {
		if (!g_is_initialized) {
			tcp_socket_base_impl::initialize();
		}

#ifdef WIN32
		struct addrinfo* addrinforesult = NULL;
		struct addrinfo hints;

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;

		int result = getaddrinfo(NULL, std::to_string(port).c_str(), &hints, &addrinforesult);
		if (result != 0) {
			WSACleanup();
			printf("Failed to resolve host address or port\n");
			return 0;
		}

		SOCKET listenfd = socket(addrinforesult->ai_family, addrinforesult->ai_socktype, addrinforesult->ai_protocol);
		if (listenfd == INVALID_SOCKET) {
			freeaddrinfo(addrinforesult);
			WSACleanup();
			printf("Failed to create listening socket\n");
			return 0;
		}

		result = bind(listenfd, addrinforesult->ai_addr, (int)addrinforesult->ai_addrlen);
		if (result == SOCKET_ERROR) {
			freeaddrinfo(addrinforesult);
			closesocket(listenfd);
			WSACleanup();
			printf("Failed to bind listening socket\n");
			return 0;
		}

		freeaddrinfo(addrinforesult);
		return listenfd;
#else
		int listenfd = socket(AF_INET, SOCK_STREAM, 0);
		if (listenfd <= 0) {
			printf("Failed to create listening socket\n");
			return 0;
		}
		
		sockaddr_in serv_addr;
		memset(&serv_addr, 0, sizeof(serv_addr));
		
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_port = htons(port);
		if (inet_pton(AF_INET, ipv4.c_str(), &(serv_addr.sin_addr)) <= 0) {
			printf("Invalid host address\n");
			return 0;
		}
		
		int result = bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
		if (result < 0) {
			printf("Failed to bind listening socket\n");
			return 0;
		}

		return static_cast<uint64_t>(listenfd);
#endif
	}

	int tcp_socket_base_impl::server_listen_impl(uint64_t listenfd, int connections) {
		if (!g_is_initialized) {
			tcp_socket_base_impl::initialize();
		}

#ifdef WIN32
		int result = listen(listenfd, connections);
		if (result == SOCKET_ERROR) {
			closesocket(listenfd);
			WSACleanup();
			printf("Failed to listen for connections\n");
			return SOCKET_ERROR;
		}

		return result;
#else
		int result = listen(listenfd, connections);
    	if (result < 0) {
        	printf("Failed to listen for connections\n");
        	return -1;
    	}

		return result;
#endif
	}

	uint64_t tcp_socket_base_impl::server_accept_impl(uint64_t listenfd, std::string& client_ipv4, int& client_port) {
		if (!g_is_initialized) {
			tcp_socket_base_impl::initialize();
		}

#ifdef WIN32
		sockaddr_in client_address;
		int client_address_sz = sizeof(client_address);

		uint64_t connfd = accept(listenfd, (struct sockaddr*)&client_address, &client_address_sz);
		if (connfd == INVALID_SOCKET) {
			closesocket(listenfd);
			WSACleanup();
			printf("Failed to accept connection\n");
			return 0;
		}

		char ip_buffer[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &(client_address.sin_addr), ip_buffer, INET_ADDRSTRLEN);
		unsigned short port = ntohs(client_address.sin_port);

		client_ipv4 = std::string(ip_buffer);
		client_port = port;
		return connfd;
#else
		sockaddr_in client_addr;
		memset(&client_addr, 0, sizeof(client_addr));

		socklen_t clilen = sizeof(client_addr);
		int connfd = accept(listenfd, (struct sockaddr*)&client_addr, (socklen_t*)&clilen);
		if (connfd < 0) {
			printf("Failed to accept client connection\n");
			return 0;
		}

		char ip_buffer[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &(client_addr.sin_addr), ip_buffer, INET_ADDRSTRLEN);
		unsigned short port = ntohs(client_addr.sin_port);

		client_ipv4 = std::string(ip_buffer);
		client_port = port;
		return static_cast<uint64_t>(connfd);
#endif
	}

	uint64_t tcp_socket_base_impl::client_connect_impl(const std::string& ipv4, int port) {
		if (!g_is_initialized) {
			tcp_socket_base_impl::initialize();
		}

#ifdef WIN32
		SOCKET connfd = socket(AF_INET, SOCK_STREAM, 0);
		if (connfd == INVALID_SOCKET) {
			WSACleanup();
			printf("Failed to create socket\n");
			return 0;
		}

		sockaddr_in hint;
		hint.sin_family = AF_INET;
		hint.sin_port = htons(port);
		int result = inet_pton(AF_INET, ipv4.c_str(), &hint.sin_addr);
		if (result <= 0) {
			WSACleanup();
			printf("Invalid host address\n");
			return 0;
		}

		result = connect(connfd, (sockaddr*)&hint, sizeof(hint));
		if (result == SOCKET_ERROR)
		{
			printf("Failed to connect to server\n");
			return 0;
		}

		return connfd;
#else
		int connfd = socket(AF_INET, SOCK_STREAM, 0);
		if (connfd < 0) {
			printf("Failed to create socket\n");
			return 0;
		}
		
		sockaddr_in serv_addr;
		memset(&serv_addr, 0, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_port = htons(port);
		if (inet_pton(AF_INET, ipv4.c_str(), &(serv_addr.sin_addr)) <= 0) {
			printf("Invalid host address\n");
			return 0;
		}
		
		int result = connect(connfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
		if (result < 0) {
			printf("Failed to connect to the server\n");
			return 0;
		}

		return static_cast<uint64_t>(connfd);
#endif
	}

	void tcp_connection_socket::set_blocking(bool blocking) {
		m_blocking = blocking;
		tcp_socket_base_impl::set_blocking_state_impl(m_handle, static_cast<bool>(blocking));
	}
	
	void tcp_connection_socket::send_bytes(char* buffer, size_t size) {
		int result = tcp_socket_base_impl::send_bytes_impl(m_handle, buffer, size);
		if (result <= 0) {
			m_alive = false;
		}
	}
	
	bool tcp_connection_socket::recv_bytes(char* buffer, size_t size) {
		int result = tcp_socket_base_impl::recv_bytes_impl(m_handle, buffer, size);
		if (result <= 0) {
			m_alive = false;
		}

		return m_alive;
	}
	
	void tcp_connection_socket::send_string(const std::string& str) {
		send_bytes(const_cast<char*>(str.data()), str.size());
	}
	
	bool tcp_connection_socket::recv_string(std::string& out_str, size_t allowed_buffer_size) {
		char* buffer = new char[allowed_buffer_size];
		int bytes = tcp_socket_base_impl::recv_bytes_impl(m_handle, buffer, allowed_buffer_size);

		if (bytes > 0)
			out_str = std::string(buffer, 0, bytes);
		else
			m_alive = false;

		delete[] buffer;
		return m_alive;
	}

	void tcp_connection_socket::close() {
		tcp_socket_base_impl::close(m_handle);
	}

	bool tcp_server_socket::bind(const std::string& ipv4, int32_t port) {
		m_listening_handle = tcp_socket_base_impl::server_bind_impl(ipv4, port);

		return (m_listening_handle != 0);
	}

	bool tcp_server_socket::listen(uint32_t connections) {
		int result = tcp_socket_base_impl::server_listen_impl(m_listening_handle, connections);

		return (result != -1);
	}
	
	void tcp_server_socket::close() {
		tcp_socket_base_impl::close(m_listening_handle);
	}

	std::shared_ptr<tcp_connection_socket> tcp_server_socket::accept() {
		std::string client_ip;
		int client_port;
		uint64_t handle = tcp_socket_base_impl::server_accept_impl(m_listening_handle, client_ip, client_port);

		auto socket = std::make_shared<tcp_connection_socket>();
		socket->m_alive = (handle != 0);
		socket->m_blocking = true;
		socket->m_handle = handle;
		socket->m_ipv4 = client_ip;
		socket->m_port = client_port;

		return socket;
	}

	std::shared_ptr<tcp_connection_socket> tcp_client_socket::connect(const std::string& ipv4, int port) {
		uint64_t handle = tcp_socket_base_impl::client_connect_impl(ipv4, port);

		auto socket = std::make_shared<tcp_connection_socket>();
		socket->m_alive = (handle != 0);
		socket->m_blocking = true;
		socket->m_handle = handle;
		socket->m_ipv4 = ipv4;
		socket->m_port = port;

		return socket;
	}
}
