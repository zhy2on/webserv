#ifndef EVENT_EXECUTOR_HPP
#define EVENT_EXECUTOR_HPP

#include <unistd.h>

#include "client_socket.hpp"
#include "kqueue_handler.hpp"
#include "request_message.hpp"
#include "response_message.hpp"
#include "server_socket.hpp"

class EventExecutor {
   public:
	static int AcceptClient(ServerSocket server_socket);

	static int ReceiveRequest(ClientSocket &client_socket, Udata *user_data);

	static int ReadFile(const int &fd, const int &readable_size,
						ResponseMessage &response_message);

	static int WriteReqBodyToPipe(const int &fd, const RequestMessage &reqeust);

	static int ReadCgiResultFromPipe(const int &fd,
									 const ResponseMessage &response);

	static int SendResponse(const ClientSocket &client_socket,
							Udata *user_data);
};

#endif
