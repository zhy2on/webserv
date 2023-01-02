#include "cgi.hpp"

#include <sstream>

#include "config_utils.hpp"

Cgi::Cgi() {}

Cgi::Cgi(std::string &x) { cgi_ = Split(x, ' '); }

Cgi::~Cgi() {}

bool Cgi::FindCgi(const std::string &x) {
	for (size_t i = 0; i < cgi_.size(); i++) {
		if (cgi_[i] == x) return true;
	}
	return false;
}

const std::vector<std::string> &Cgi::GetCgi() const { return cgi_; }

std::string Cgi::ToString() const {
	std::stringstream ss;

	for (size_t i = 0; i < cgi_.size(); i++) {
		ss << cgi_[i] << ' ';
	}
	return ss.str();
}

std::ostream &operator<<(std::ostream &out, const Cgi &x) {
	out << x.ToString();
	return out;
}
