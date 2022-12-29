#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <vector>
#include <fstream>

#include "client_socket.hpp"
#include "config_parser.hpp"
#include "kqueue_handler.hpp"
#include "server_info.hpp"
#include "server_socket.hpp"
#include "udata.hpp"

class ClientSocket;
class ServerSocket;

class Webserv {
   public:
	typedef ConfigParser::server_configs_type server_configs_type;
	typedef std::map<int, ServerSocket *> servers_type;
	typedef std::map<int, ClientSocket *> clients_type;

	static unsigned long error_log_fd_;
	static unsigned long access_log_fd_;

	explicit Webserv(const server_configs_type &server_configs);
	~Webserv();

	void RunServer();
   private:
	servers_type servers_;
	clients_type clients_;
	KqueueHandler kq_handler_;

	void HandleEvent(struct kevent &event);
	void HandleListenEvent(struct kevent &event);
	void HandleReceiveRequestEvent(struct kevent &event);
	void HandleReadFile(struct kevent &event);
	void HandleSendResponseEvent(struct kevent &event);
	void HandleWriteToPipe(struct kevent &event);
	void HandleReadFromPipe(struct kevent &event);

	static void WriteLog(struct kevent &event);

	ServerSocket *FindServerSocket(int fd);
	ClientSocket *FindClientSocket(int fd);
};

#endif
