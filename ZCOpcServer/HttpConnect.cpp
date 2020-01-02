#include "pch.h"
#include "HttpConnect.h"
#include <sstream>
#include <iostream>
using namespace std;
#ifdef WIN32
#pragma comment(lib,"ws2_32.lib")
#endif
#include "D:/Windows Kits/10/Include/10.0.17763.0/um/WinSock2.h"
HttpConnect::HttpConnect()
{
#ifdef WIN32
	//�˴�һ��Ҫ��ʼ��һ�£�����gethostbyname����һֱΪ��
	WSADATA wsa = { 0 };
	WSAStartup(MAKEWORD(2, 2), &wsa);
#endif
}
HttpConnect::~HttpConnect()
{
}
string HttpConnect::socketHttp(std::string host, std::string request)
{
	int sockfd;
	struct sockaddr_in address;
	struct hostent* server;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	address.sin_family = AF_INET;
	address.sin_port = htons(8082);
	server = gethostbyname(host.c_str());
	memcpy((char*)&address.sin_addr.s_addr, (char*)server->h_addr, server->h_length);
	if (-1 == connect(sockfd, (struct sockaddr*) & address, sizeof(address))) {
		//DBG << "connection error!" << std::endl;
		return "";
	}
	//DBG << request << std::endl;
#ifdef WIN32
	send(sockfd, request.c_str(), request.size(), 0);
#else
	write(sockfd, request.c_str(), request.size());
#endif
	char *buf = new char[1024 * 1024];
	int offset = 0;
	int rc;
#ifdef WIN32
	while (rc = recv(sockfd, buf + offset, 1024, 0))
#else
	while (rc = read(sockfd, buf + offset, 1024))
#endif
	{
		offset += rc;
	}
#ifdef WIN32
	closesocket(sockfd);
#else
	close(sockfd);
#endif
	buf[offset] = 0;
	//std::cout << buf << endl;
	
	string ss = string(buf);

	//std::stringstream stream;
	//stream << buf;
	//std::cout << stream.str() << endl;
	//DBG << buf << std::endl;
	delete[]buf;
	return ss;
}
string  HttpConnect::postData(std::string host, std::string path, std::string post_content)
{
	//POST����ʽ
	std::stringstream stream;
	stream << "POST " << path;
	stream << " HTTP/1.0\r\n";
	stream << "Host: " << host << "\r\n";
	stream << "User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; zh-CN; rv:1.9.2.3) Gecko/20100401 Firefox/3.6.3\r\n";
	stream << "Content-Type:application/x-www-form-urlencoded\r\n";
	stream << "Content-Length:" << post_content.length() << "\r\n";
	stream << "Connection:close\r\n\r\n";
	stream << post_content.c_str();
	return socketHttp(host, stream.str());
}
string HttpConnect::getData(std::string host, std::string path, std::string get_content)
{
	//GET����ʽ
	std::stringstream stream;
	stream << "GET " << path << "?" << get_content;
	stream << " HTTP/1.0\r\n";
	stream << "Host: " << host << "\r\n";
	stream << "User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; zh-CN; rv:1.9.2.3) Gecko/20100401 Firefox/3.6.3\r\n";
	stream << "Connection:close\r\n\r\n";
	return socketHttp(host, stream.str());
}
