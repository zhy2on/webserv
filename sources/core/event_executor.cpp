#include "event_executor.hpp"

#include "client_socket.hpp"
#include "request_message.hpp"
#include "request_parser.hpp"
#include "request_validation.hpp"
#include "resolve_uri.hpp"
#include "response_message.hpp"

int EventExecutor::AcceptClient(ServerSocket server_socket) {
    int client_sock_d = server_socket.AcceptClient();
    return client_sock_d;
}

/*
 * Request Message에 resolved uri가 있는 경우
 * */

int EventExecutor::ReceiveRequest(ClientSocket &client_socket,
										Udata *user_data) {
	ResponseMessage &response = user_data->response_message_;
	RequestMessage &request = user_data->request_message_;

	(void)response;
	(void)request;

	char buf[RequestMessage::BUFFER_SIZE];
	int recv_len =
		recv(client_socket.GetSocketDescriptor(), buf, sizeof(buf), 0);
	buf[recv_len] = '\0';
	if (recv_len < 0) {
		throw(HttpException(INTERNAL_SERVER_ERROR));
	}
	ParseRequest(request, client_socket.GetServerInfos(), buf);
	if (request.GetState() == HEADER_END) { 
		// check header validation
		//  if invalid throw exception
	
		// socket info 정하고,
		// request validation 체크하고, uri resolve
		// 	// client_socket.FindServerInfoWithHost(request.GetHeader()); //
		// request에 GetHeader 구현 필요
		// 	client_socket.FindLocationWithUri(request.GetUri());
		// 	RequestValidationCheck(client_socket);
		// 	Resolve_URI(client_socket, request, user_data);
		// 	// exception 처리 내부에서 해줌 response 설정까지 해줘야함
		// } -> 이 부분이 ParseRequest로 들어감.
	} else if (request.GetState() == DONE) {
		std::cout << C_BOLD << C_BLUE << "PARSE DONE!" << C_RESET << std::endl;
		std::cout << request << std::endl;

		return Udata::WRITE_TO_PIPE; // 일단 WRITE_TO_PIPE test
	}
	return Udata::RECV_REQUEST;
}

int EventExecutor::ReadFile(const int &fd, const int &readable_size,
							ResponseMessage &response_message) {
	char buf[ResponseMessage::BUFFER_SIZE];
	int size = read(fd, buf, ResponseMessage::BUFFER_SIZE);
	if (size < 0) {
		perror("open: INTERNAL_SERVER_ERROR");
	}
	response_message.AppendBody(buf);
	if (size < readable_size) {
		return Udata::READ_FILE;
	}
	return Udata::SEND_RESPONSE;
}

/**
 * Response Message의 total_length가 Response Message를 만들 때 이미
 * 설정되었다고 가정.
 * TODO: chunked response message
 */
int EventExecutor::SendResponse(const ClientSocket &client_socket,
										Udata *user_data) {
	ResponseMessage &response = user_data->response_message_;
	RequestMessage &request = user_data->request_message_;

	std::string response_str = response.ToString();
	int send_len = send(client_socket.GetSocketDescriptor(),
						response_str.c_str() + response.current_length_,
						response_str.length() - response.current_length_, 0);
	if (send_len < 0) {     // send 실패
		// throw SystemException;
		return Udata::CLOSE;
	}
	response.AddCurrentLength(send_len);
	if (response.IsDone()) {
		RequestMessage::headers_type headers = request.GetHeaders();
		if (headers["connection"] == "close") {     // connection: close
			return Udata::CLOSE;
		}
		return Udata::RECV_REQUEST;    // connection: keep-alive
	}
	return Udata::CLOSE;
}
