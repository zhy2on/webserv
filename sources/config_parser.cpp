#include "config_parser.hpp"

#include <iostream>
#include <sstream>

const char *whitespace = " \r\n\t\v\f";

std::string &ltrim(std::string &str,
				   std::string const &whitespace) {
	str.erase(0, str.find_first_not_of(whitespace));
	return str;
}

std::string &rtrim(std::string &str,
				   std::string const &whitespace) {
	str.erase(str.find_last_not_of(whitespace) + 1);
	return str;
}

std::string &trim(std::string &str) {
	return ltrim(rtrim(str, whitespace), whitespace);
}

ConfigParser::ConfigParser(const char *file) {
	std::ifstream in(file);
	std::string line;

	if (!in) {
		throw FstreamException();
	}
	int bracket = 0;
	config_.clear();
	while (std::getline(in, line)) {
		trim(line);
		if (line[0] == '#' || line.length() <= 0) continue;
		config_.append(line + '\n');
		// std::cout << config_ << std::endl;
		for (size_t i = 0; i < line.length(); i++) {
			if (line[i] == '{')
				bracket++;
			else if (line[i] == '}')
				bracket--;
		}
		char eol = line[line.length() - 1];
		if (eol != '{' && eol != '}' && eol != ';') throw BracketException();
	}
	in.close();
	if (bracket) throw BracketException();
}

ConfigParser::~ConfigParser() {}

void ConfigParser::Parse() {
	size_t pre = 0;
	// size_t cur = config_.find_first_not_of(whitespace, pre);
	size_t cur = 0;
	// if (cur == std::string::npos) {
	// 	throw NoContentException();
	// }
	while (cur != std::string::npos) {
		pre = config_.find_first_not_of(whitespace, cur);
		cur = config_.find_first_of(" \t\n\v\f\r{", pre);
		std::string key = config_.substr(pre, cur - pre);
		if (key != "server") {
			throw ServerException();
		}
		server_.push_back(ParseServer(cur));
	}
}

Server ConfigParser::ParseServer(size_t &i) {
	std::string key;
	std::string value;
	Server server;
	size_t pre = config_.find_first_not_of(whitespace, i);
	if (pre == std::string::npos || config_[pre] != '{') {
		throw ServerException();
	}
	pre++;
	size_t cur = config_.find_first_not_of(whitespace, pre);
	while (cur != std::string::npos) {
		if ((pre = config_.find_first_not_of(whitespace, cur)) ==
			std::string::npos) {
			throw ServerException();
		}
		if ((cur = config_.find_first_of(whitespace, pre)) ==
			std::string::npos) {
			throw ServerException();
		}
		key = config_.substr(pre, cur - pre);
		if (key == "}") {
			i = config_.find_first_not_of(whitespace, cur + 1);
			break;
		}
		if (key == "location") {
			server.SetLocations((ParseLocation(cur)));
		} else {
			if ((pre = config_.find_first_not_of(whitespace, cur)) ==
				std::string::npos) {
				throw ServerException();
			}
			if ((cur = config_.find_first_of("\n", pre)) == std::string::npos) {
				throw ServerException();
			}
			value = config_.substr(pre, cur - pre - 1);
			SetServer(server, key, value);
		}
	}
	return server;
}

Location ConfigParser::ParseLocation(size_t &i) {
	std::string key;
	std::string value;
	Location location;

	size_t pre = config_.find_first_not_of(whitespace, i);
	size_t cur = config_.find_first_of(whitespace, pre);
	location.SetPath(config_.substr(pre, cur - pre));

	pre = config_.find_first_not_of(whitespace, cur);
	if (pre == std::string::npos || config_[pre] != '{') {
		throw LocationException();
	}
	pre++;
	cur = config_.find_first_of(whitespace, pre);
	while (cur != std::string::npos) {
		if ((pre = config_.find_first_not_of(whitespace, cur)) ==
			std::string::npos) {
			throw LocationException();
		}
		if ((cur = config_.find_first_of(whitespace, pre)) ==
			std::string::npos) {
			throw LocationException();
		}
		key = config_.substr(pre, cur - pre);
		if (key == "}") {
			i = config_.find_first_not_of(whitespace, cur + 1);
			break;
		} else {
			if ((pre = config_.find_first_not_of(whitespace, cur)) ==
				std::string::npos) {
				throw LocationException();
			}
			if ((cur = config_.find_first_of("\n", pre)) == std::string::npos) {
				throw LocationException();
			}
			value = config_.substr(pre, cur - pre - 1);
			SetLocation(location, key, value);
		}
	}
	return location;
}

// listen case
// listen 127.0.0.1:8000;
// listen 127.0.0.1;
// listen localhost:8000;
// listen 8000;
// listen *:8000;
void ConfigParser::SetServer(Server &server, std::string key,
							  std::string value) {
	if (key == "server_name") {
		server.SetServerName(value);
	} else if (key == "listen") {
		if (value.find_first_of(":") == std::string::npos) {
			if (value.find_first_of(".") == std::string::npos) {
				server.SetHost("0.0.0.0");
				server.SetPort(value);
			} else {
				server.SetPort("8000");
				server.SetHost(value); 
				}
		} else {
			std::vector<std::string> temp = Split(value, ':');
			if (temp[0] == "*")
				temp[0] = "0.0.0.0";
			if (temp[1] == "*")
				temp[1] = "0.0.0.0";
			server.SetHost(temp[0]);
			server.SetPort(temp[1]);
		}
	} else if (key == "client_max_body_size") {
		server.SetClientMaxBodySize(atoi(value.c_str()));
	} else if (key == "root") {
		server.SetRoot(value);
	} else if (key == "autoindex") {
		if (value == "on")
			server.SetAutoindex(true);
		else
			server.SetAutoindex(false);
	} else if (key == "index") {
		std::vector<std::string> temp = Split(value, ' ');
		for (size_t i = 0; i != temp.size(); i++)
			server.SetIndex(temp[i]);
	} else if (key == "allow_methods") {
		std::vector<std::string> temp = Split(value, ' ');
		for (size_t i = 0; i < temp.size(); i++) {
			if (temp[i] != "GET" && temp[i] != "POST" && temp[i] != "DELETE")
				server.SetAllowMethods("INVALID");
			else
				server.SetAllowMethods(temp[i]);
		}
	} else if (key == "error_page") {
		std::vector<std::string> temp = Split(value, ' ');
		std::vector<int> pages; // 얘 어디서 쓰이지?
		std::string path = temp[temp.size() - 1];
		for (size_t i = 0; i < temp.size() - 1; i++) {
			server.SetErrorPages(std::pair<int, std::string>(atoi(temp[i].c_str()), path));
		}
	}
}

void ConfigParser::SetLocation(Location &location, std::string key,
								std::string value) {
	if (key == "root") {
		location.SetRoot(value);
	} else if (key == "index") {
		std::vector<std::string> temp = Split(value, ' ');
		for (size_t i = 0; i != temp.size(); i++)
			location.SetIndex(temp[i]);
	} else if (key == "allow_methods") {
		std::vector<std::string> temp = Split(value, ' ');
		for (size_t i = 0; i < temp.size(); i++) {
			if (temp[i] != "GET" && temp[i] != "POST" && temp[i] != "DELETE")
				location.SetAllowMethods("INVAILD");
				// allow_methods_.push_back("INVALID");
			else
				location.SetAllowMethods(temp[i]);
				// allow_methods_.push_back(temp[i]);
		}
	} else if (key == "cgi") {
		std::vector<std::string> temp = Split(value, ' ');
		for (size_t i = 0; i != temp.size(); i++)
			location.SetCgi(temp[i]);
			// cgi_.push_back(temp[i]);
	} else if (key == "client_max_body_size") {
		location.SetClientMaxBodySize(atoi(value.c_str()));
	} else {
		throw LocationException();
	}
}

std::vector<std::string> ConfigParser::Split(std::string input,
											 char delimiter) {
	std::vector<std::string> str;
	std::stringstream ss(input);
	std::string temp;

	while (std::getline(ss, temp, delimiter)) str.push_back(temp);

	return str;
}

void ConfigParser::PrintConf() {
	for (size_t i = 0; i < server_.size(); i++) {
		std::cout << "server " << i + 1 << '\n';
		std::cout << "server_name : " << server_[i].GetServerName() << '\n';
		std::cout << "host : " << server_[i].GetHost() << '\n';
		std::cout << "port : " << server_[i].GetPort() << '\n';
		std::cout << "root : " << server_[i].GetRoot() << '\n';
		std::cout << "autoindex : " << server_[i].GetAutoindex() << '\n';
		std::cout << "client_max_body_size : "
				  << server_[i].GetClientMaxBodySize() << '\n';
		std::cout << "index : ";
		for (size_t j = 0; j < server_[i].GetIndex().size(); j++)
			std::cout << server_[i].GetIndex()[j] << ' ';
		std::cout << '\n';
		std::cout << "allow_methods : ";
		for (size_t j = 0; j < server_[i].GetAllowMethods().size(); j++)
			std::cout << server_[i].GetAllowMethods()[j] << ' ';
		std::cout << '\n';
		std::cout << "error_pages : " << '\n';
		for (std::map<int, std::string>::iterator it =
				 server_[i].GetErrorPages().begin();
			 it != server_[i].GetErrorPages().end(); it++) {
			std::cout << it->first << ' ' << it->second << '\n';
		}
		std::cout << "\n\n";
		for (size_t j = 0; j < server_[i].GetLocations().size(); j++) {
			std::cout << "location " << j + 1 << '\n';
			std::cout << "path : " << server_[i].GetLocations()[j].GetPath() << '\n';
			std::cout << "root : " << server_[i].GetLocations()[j].GetRoot() << '\n';
			std::cout << "client_max_body_size : "
					  << server_[i].GetLocations()[j].GetClientMaxBodySize() << '\n';
			std::cout << "index : ";
			for (size_t k = 0; k < server_[i].GetLocations()[j].GetIndex().size(); k++)
				std::cout << server_[i].GetLocations()[j].GetIndex()[k] << ' ';
			std::cout << '\n';
			std::cout << "allow_methods : ";
			for (size_t k = 0; k < server_[i].GetLocations()[j].GetAllowMethods().size();
				 k++)
				std::cout << server_[i].GetLocations()[j].GetAllowMethods()[k] << ' ';
			std::cout << '\n';
			std::cout << "cgi : ";
			for (size_t k = 0; k < server_[i].GetLocations()[j].GetCgi().size(); k++)
				std::cout << server_[i].GetLocations()[j].GetCgi()[k] << ' ';
			std::cout << "\n\n";
		}
		std::cout << "\n\n";
	}
}
