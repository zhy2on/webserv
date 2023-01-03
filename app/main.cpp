#include <iostream>

#include "config_parser.hpp"
#include "webserv.hpp"
#include "config_utils.hpp"

int main(int argc, char **argv) {
	if (argc > 2) {
		std::cerr << "Usage: ./webserv [config]" << std::endl;
		return EXIT_FAILURE;
	}
	const char *config;
	if (argc == 1)
		config = "./conf/sample.conf";
	else
		config = argv[1];
	ConfigParser::server_configs_type server_configs;
	try {
		//server_configs = ConfigParse::ConfigSetup(config);
		ConfigParser config_parser(config);
		config_parser.RunConfigParser(server_configs);
		Webserv webserv(server_configs);
		webserv.RunServer();
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
