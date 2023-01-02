#ifndef CGI_HPP
#define CGI_HPP

#include <string>
#include <vector>

class Cgi {
   public:
	Cgi();
	Cgi(std::string &index);
	~Cgi();

	bool FindCgi(const std::string &index);
	const std::vector<std::string> &GetCgi() const;
	std::string ToString() const;

   private:
	std::vector<std::string> cgi_;
};

std::ostream &operator<<(std::ostream &out, const Cgi &x);

#endif
