#ifndef URI_HPP
#define URI_HPP

#include <iostream>
#include <map>

class Uri {
   public:
	Uri(const std::string &uri);
	~Uri();

	const std::string &GetOriginUri() const;
	const std::string &GetPath() const;
	const std::string &GetQuery() const;

	void SplitUri();
	std::string ToString();

   private:
	std::string origin_uri_;
	std::string path_;
	std::string query_;

	void ApplyConfigToPath();
};

std::ostream &operator<<(std::ostream &out, Uri uri);
#endif
