#include "uri.hpp"

#include <sstream>

Uri::Uri(const std::string &uri) : origin_uri_(uri) { SplitUri(); }

Uri::~Uri() {}

const std::string &Uri::GetOriginUri() const { return origin_uri_; }

const std::string &Uri::GetPath() const { return path_; }

const std::string &Uri::GetQuery() const {
	return query_;
}

void Uri::SplitUri() {
	size_t index = origin_uri_.find("?");
	if (index == std::string::npos) {
		path_ = origin_uri_;
	} else {
		path_ = origin_uri_.substr(0, index);
		std::string query_ =
			origin_uri_.substr(index + 1, origin_uri_.length() - (index + 1));
	}
	ApplyConfigToPath();
}

void Uri::ApplyConfigToPath() {
	path_.insert(0, "/Users/zhy2on/Documents/www");
}

std::string Uri::ToString() {
	std::stringstream ss;
	std::string query_string;

	ss << "origin _uri : " << origin_uri_ << '\n'
		<< "path : " << path_ << '\n'
		<< "query : " << query_ << '\n';
	return ss.str();
}


std::ostream &operator<<(std::ostream &out, Uri uri) {
	out << uri.ToString();
	return out;
}
