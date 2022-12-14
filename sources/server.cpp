#include "server.hpp"

Server::Server(): client_max_body_size_(1000000), autoindex_(false), server_name_("") {}

Server::~Server() {}

// // 얘들 디폴트 값 생각 
// 	int client_max_body_size_;
// 	bool autoindex_;
// 	std::string host_; //? 
// 	std::string port_; // ?
// 	std::string root_; // default html 
// 	std::string server_name_;
// 	std::vector<std::string> index_;
// 	std::vector<std::string> allow_methods_; // ? 
// 	std::map<int, std::string> error_pages_;
// 	std::vector<Location> locations_;

const int &Server::GetClientMaxBodySize() const {
    return this->client_max_body_size_;
}
const bool &Server::GetAutoindex() const{
    return this->autoindex_;
}
const std::string &Server::GetHost() const{
    return this->host_;
}
const std::string &Server::GetPort() const{
    return this->port_;
}
const std::string &Server::GetRoot() const{
    return this->root_;
}
const std::string &Server::GetServerName() const{
    return this->server_name_;
}
const std::vector<std::string> &Server::GetIndex() const{
    return this->index_;
}
const std::vector<std::string> &Server::GetAllowMethods() const{
    return this->allow_methods_;
}
std::map<int, std::string> &Server::GetErrorPages(){
    return this->error_pages_;
}
const std::vector<Location> &Server::GetLocations() const{
    return this->locations_;
}

// setter
void Server::SetClientMaxBodySize(const int &x){
    this->client_max_body_size_ = x;
}
void Server::SetAutoindex(const bool &x){
    this->autoindex_ = x;
}
void Server::SetHost(const std::string &x){
    this->host_ = x;
}
void Server::SetPort(const std::string &x){
    this->port_ = x;
}
void Server::SetRoot(const std::string &x){
    this->root_ = x;
}
void Server::SetServerName(const std::string &x){
    this->server_name_ = x;
}
void Server::SetIndex(const std::vector<std::string> &x){
    this->index_ = x;
}
void Server::SetIndex(const std::string &x){
    this->index_.push_back(x);
}
void Server::SetAllowMethods(const std::vector<std::string> &x){
    this->allow_methods_ = x;
}
void Server::SetAllowMethods(const std::string &x){
    this->allow_methods_.push_back(x);
}
void Server::SetErrorPages(const std::map<int, std::string> &x){
    this->error_pages_ = x;
}

void Server::SetErrorPages(const std::pair<int, std::string> &x){
    this->error_pages_.insert(x);
}
void Server::SetLocations(const std::vector<Location> &x){
    this->locations_ = x;
}
void Server::SetLocations(const Location &x){
    this->locations_.push_back(x);
}
