// TestOpc.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "soap/soapH.h"
#include "soap/add.nsmap"
#include <string>
#include <sstream>
using namespace std;
#pragma comment(lib,"wsock32.lib")
const char server[] = "http://localhost:17890";
int main()
{
	int n = 0;
	for (int i = 0; i < 1000; i++)
	{
		struct soap soap;
		std::string result;
		soap_init1(&soap, SOAP_XML_INDENT);

		//string sk = "{\"key\":\"setopctag\",\"data\":[{\"sname\":\"key_hello_world\",\"ucType\":\"11\",\"ucWriteable\":\"0\",\"cvalue\":\"helloworld\",\"sDes\":\"helloworld\",\"sUnit\":\"%\"},{\"sname\":\"key_jone\",\"ucType\":\"11\",\"cvalue\":\"aksdjflajdfjkjsldafj\"}]}";
		string sk = "{\"key\": \"setopctag\",\"data\": [{\"o_key\": \"fac.item1.key_hello_world\",\"o_type\": \"11\", \"o_writeable\": \"0\", \"o_val\": \"helloworld\", \"o_descripe\": \"helloworld\", \"o_unit\": \"%\" }, {\"o_key\": \"key_jone\",\"o_type\": \"11\", \"o_val\": \"aksdjflajdfjkjsldafj\", \"o_descripe\": \"helloworld\"}]}";

		std::ostringstream   ostr;
		ostr << i << sk;
		std::cout << sk << std::endl;
		string sss = ostr.str();
		soap_call_ns__GetInfo(&soap, server, "", sk, result);
		std::cout << result << std::endl;

		soap_destroy(&soap);
		soap_end(&soap);
		soap_done(&soap);
		std::cout << i << endl;
	}

	getchar();

}
