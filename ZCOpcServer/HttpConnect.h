#pragma once
#include <string>
class HttpConnect
{
public:
	HttpConnect();
	~HttpConnect();

	std::string socketHttp(std::string host, std::string request);
	std::string postData(std::string host, std::string path, std::string post_content);
	std::string getData(std::string host, std::string path, std::string get_content);
};

