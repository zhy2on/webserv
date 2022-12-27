//
// Created by 김민준 on 2022/12/04.
//

#ifndef WEBSERV_INCLUDES_CGI_HANDLER_H_
#define WEBSERV_INCLUDES_CGI_HANDLER_H_

extern char **environ;

#include <map>
#include <string>

#include "kqueue_handler.hpp"
#include "request_message.hpp"
#include "udata.hpp"
#include "uri.hpp"

#define READ 0
#define WRITE 1

class CgiHandler {
   public:
	CgiHandler();
	~CgiHandler();

	void OpenPipe(KqueueHandler &kq_handler, Udata *user_data);

	int OpenBodyPipe();
	int OpenResultPipe();

	void SetCgiEnvs(RequestMessage request_, Uri uri);

	void PrepareCgi(RequestMessage request_);

	void SetupCgiResultPipe();
	void SetupReqBodyPipe();

	void RunChildCgi();
	void SetupAndAddEvent(KqueueHandler &kq_handler, Udata *user_data,
						  RequestMessage request_);

	static void WriteReqBodyToCgi(const int &fd, const RequestMessage &reqeust);
	static void ReadCgiResultFormCgi(struct kevent event,
									 KqueueHandler &kq_handler);

   private:
	char **env_list_;
	std::map<std::string, std::string> cgi_envs_;

	int req_body_pipe_[2];
	int cgi_result_pipe_[2];

	void ParseEnviron();
	void ConvertEnvToCharSequence();
};

#endif	// WEBSERV_INCLUDES_CGI_HANDLER_H_
