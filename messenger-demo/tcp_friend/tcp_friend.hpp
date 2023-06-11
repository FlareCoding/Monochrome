#ifndef TCP_FRIEND_H
#define TCP_FRIEND_H
#include <memory>
#include <cinttypes>
#include <string>

namespace tcp_friend {
class tcp_socket_base_impl {
public:
	static void initialize();
	static void close(uint64_t handle);

	static void set_blocking_state_impl(uint64_t handle, int blocking);

	static int send_bytes_impl(uint64_t handle, char* buffer, size_t size);
	static int recv_bytes_impl(uint64_t handle, char* buffer, size_t allowed_buffer_size);

	static uint64_t server_bind_impl(const std::string& ipv4, int port);
	static int server_listen_impl(uint64_t listenfd, int connections);
	static uint64_t server_accept_impl(uint64_t listenfd, std::string& client_ipv4, int& client_port);

	static uint64_t client_connect_impl(const std::string& ipv4, int port);
};

class tcp_connection_socket {
public:
	tcp_connection_socket() = default;
	~tcp_connection_socket() = default;

	void set_blocking(bool blocking);
	inline bool is_blocking() { return m_blocking; }

	void send_bytes(char* buffer, size_t size);
	bool recv_bytes(char* buffer, size_t size);

	void send_string(const std::string& str);
	bool recv_string(std::string& out_str, size_t allowed_buffer_size = 1024);

	void close();

	inline bool is_alive() { return m_alive; }

	inline const std::string& get_address() const { return m_ipv4; }
	inline int get_port() const { return m_port; }

private:
friend class tcp_server_socket;
friend class tcp_client_socket;

	bool m_alive = false;
	bool m_blocking = true;
	uint64_t m_handle = 0;

	std::string m_ipv4;
	int			m_port = 0;
};
	
class tcp_server_socket {
public:
	bool bind(const std::string& ipv4, int32_t port);
	bool listen(uint32_t connections);
	void close();

	std::shared_ptr<tcp_connection_socket> accept();

private:
	uint64_t m_listening_handle = 0;
};

class tcp_client_socket {
public:
	static std::shared_ptr<tcp_connection_socket> connect(const std::string& ipv4, int port);
};
}

#endif
