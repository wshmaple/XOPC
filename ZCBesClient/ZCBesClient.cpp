// ZCBesClient.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "soap/soapH.h"
#include "soap/add.nsmap"
#include <string>
#include <sstream>
#include <chrono>
using namespace std;
#pragma comment(lib,"wsock32.lib")
const char server[] = "http://localhost:17890";
#include "resource.h"
#include "../ZCOpcServer/Markup.h"
#include <comutil.h>  
#include "D:/Windows Kits/10/Include/10.0.18362.0/um/WinUser.h"
#pragma comment(lib, "comsuppw.lib")
//将wstring转换成string  
#include <tchar.h>
string wstring2string(const wstring& ws)
{
	_bstr_t t = ws.c_str();
	char* pchar = (char*)t;
	string result = pchar;
	return result;
}

string ws2s(const wstring& ws)
{
	_bstr_t t = ws.c_str();
	char* pchar = (char*)t;
	string result = pchar;
	return result;
}

wstring s2ws(const string& s)
{
	_bstr_t t = s.c_str();
	wchar_t* pwchar = (wchar_t*)t;
	wstring result = pwchar;
	return result;
}

#include <shlobj.h>    

#include <map>
using namespace std;

map <string, string> g_map;
void kk()
{
	//printf("kk\n");
	HRSRC hRsrc = ::FindResource(NULL, MAKEINTRESOURCE(IDR_XML1), TEXT("xml"));
	if (NULL == hRsrc)
	{
		printf("aa1\n");
		return;
	}
	else
	{
		//	printf("error1\n");
	}

	//获取资源的大小

	DWORD dwSize = SizeofResource(NULL, hRsrc);

	if (0 == dwSize)
	{
		printf("aa2\n");
		return;
	}
	else
	{
		//	printf("error2\n");
	}


	//加载资源

	HGLOBAL hGlobal = LoadResource(NULL, hRsrc);

	if (NULL == hGlobal)
	{
		printf("aa3\n");
		return;
	}
	else
	{
		//	printf("error3\n");
	}

	//锁定资源

	LPVOID pBuffer = LockResource(hGlobal);

	if (NULL == pBuffer)

	{
		printf("aa4\n");
		return;
	}
	else
	{
		//	printf("error4\n");
	}


	BOOL bRt = FALSE;
	TCHAR MyDir[_MAX_PATH];

	SHGetSpecialFolderPath(NULL, MyDir, CSIDL_PERSONAL, 0);
	//	printf("路径2：%ws \n", MyDir);

	TCHAR MyDirtemp[_MAX_PATH];

	wsprintf(MyDirtemp, _T("%s%s"), MyDir, _T("\\conf.xml"));
	//	printf("路径3：%ws \n", MyDirtemp);
	FILE* fp = _tfopen(MyDirtemp, _T("wb"));

	if (fp != NULL)
	{

		if (dwSize == fwrite(pBuffer, sizeof(char), dwSize, fp))
		{
			//printf("kk2\n");
			CMarkup xml;
			bool brt = xml.Load(MyDirtemp);

			//printf("配置文件加载%d\n", brt);
			//string sk = string(p);
			//xml.SetDoc(s2ws(sk).c_str());

			while (xml.FindElem(_T("data"))) {
				xml.IntoElem();
				xml.FindElem(_T("unit"));
				MCD_STR strunit = xml.GetData();
				xml.FindElem(_T("describe"));
				MCD_STR strdescribe = xml.GetData();
				xml.FindElem(_T("opcKey"));
				MCD_STR stropcKey = xml.GetData();
				xml.OutOfElem();
				string it = wstring2string(strunit);
				string des = wstring2string(strdescribe);
				string key = wstring2string(stropcKey);
				g_map.insert(std::pair <string, string>(key, des));
				
			}
			//	printf("kk3\n");
		}

		bRt = TRUE;

		fclose(fp);

		DeleteFile(MyDirtemp);

	}
	else

	{
		printf("error511\n");
	}
	::FreeResource(hGlobal);
}


int main()
{
    std::cout << "Hello World!\n";
	int n = 0;
	kk();
	std::chrono::time_point<std::chrono::high_resolution_clock> p0 = std::chrono::high_resolution_clock::now();
	//for (int i = 0; i < 10; i++)
	//{
	//	struct soap soap;
	//	std::string result;
	//	soap_init1(&soap, SOAP_XML_INDENT);
	//	
	//	//string sk = "{\"key\":\"setopctag\",\"data\":[{\"sname\":\"key_hello_world\",\"ucType\":\"11\",\"ucWriteable\":\"0\",\"cvalue\":\"helloworld\",\"sDes\":\"helloworld\",\"sUnit\":\"%\"},{\"sname\":\"key_jone\",\"ucType\":\"11\",\"cvalue\":\"aksdjflajdfjkjsldafj\"}]}";
	//	string sk = "{\"key\": \"setopctag\",\"data\": [{\"o_key\": \"fac.item1.key_hello_world\",\"o_type\": \"11\", \"o_writeable\": \"0\", \"o_val\": \"helloworld\", \"o_descripe\": \"helloworld\", \"o_unit\": \"%\" }, {\"o_key\": \"key_jone\",\"o_type\": \"11\", \"o_val\": \"aksdjflajdfjkjsldafj\", \"o_descripe\": \"helloworld\"}]}";
	//	
	//	string s1 = "{\"key\": \"setopctag\",\"data\": [{\"o_key\": \"fac.item1.key_hello_world";
	//	string s2 = "\",\"o_type\": \"11\", \"o_writeable\": \"0\", \"o_val\": \"helloworld\", \"o_descripe\": \"helloworld\", \"o_unit\": \"%\" }]}"; 
	//	std::ostringstream   ostr;
	//
	//	std::ostringstream   ostr2;
	//	ostr2 << s1 << i << s2;
	//	ostr << i << sk;
	//	std::cout << sk << std::endl;
	//	string sss = ostr.str();
	//	soap_call_ns__GetInfo(&soap, server, "", ostr2.str(), result);
	//	std::cout << result << std::endl;
	//
	//	soap_destroy(&soap);
	//	soap_end(&soap);
	//	soap_done(&soap);
	//	std::cout << i << endl;
	//}
	string skk = "{\"key\": \"setopctag\",\"data\": [{\"o_key\": \"fac\",\"o_val\": \"helloworld\"},{\"sname\":\"key_hello_world\",\"ucType\":\"11\",\"ucWriteable\":\"0\",\"cvalue\":\"helloworld\",\"sDes\":\"helloworld\",\"sUnit\":\"%\"}]}";
	map <string, string>::iterator m1_Iter;
	unsigned seed;  // Random generator seed
	// Use the time function to get a "seed” value for srand
	seed = time(0);
	srand(seed);

	struct soap soap;
	std::string result;
	soap_init1(&soap, SOAP_XML_INDENT);
	//while(n<10000)
	// for (m1_Iter = g_map.begin(); m1_Iter != g_map.end(); m1_Iter++)
	// {
	// 	if (m1_Iter->first.empty())
	// 		continue;
	// 	char buf[1255] = {0};
	// 	int ns = rand();
	// 	sprintf_s(buf, "{\"key\": \"setopctag\",\"data\": [{\"o_key\": \"%s\",\"o_val\": \"%d\",\"o_type\": \"11\", \"o_writeable\": \"0\", \"o_descripe\": \"helloworld\", \"o_unit\": \"C\" }]}", m1_Iter->first.c_str(), ns);
	// 	std::ostringstream   ostr;
	// 	ostr << buf;
	// 	soap_call_ns__GetInfo(&soap, server, "", ostr.str(), result);
	// 	n++;
	// }


	std::ostringstream   ostr;
	std::ostringstream   ostrtmp;
	int sk = 0;
	int nnn = g_map.size()-2;
	bool flg = false;
	for (m1_Iter = g_map.begin(); m1_Iter != g_map.end(); m1_Iter++)
	{
		if (m1_Iter->first.empty())
			continue;
		char buf[1024] = { 0 };
		int ns = rand();
		sprintf_s(buf, "{\"o_key\": \"%s\",\"o_val\": \"%d\",\"o_type\": \"11\", \"o_writeable\": \"0\", \"o_descripe\": \"helloworld\", \"o_unit\": \"C\" }", m1_Iter->first.c_str(), ns);	
		if (sk==nnn)
		{
			ostrtmp << buf;
			flg = true;
		}
		else {
			ostrtmp << buf << ",";
		}
		
		sk++;
	}
	ostr << "{\"key\": \"setopctag\",\"data\": [" << ostrtmp.str() << "]}";
	while (n<1000000)
	{
		Sleep(3000);
		soap_call_ns__GetInfo(&soap, server, "", ostr.str(), result);
		n++;
	}
  
	cout << ostr.str()<< sk <<endl<< nnn << endl<<flg<<endl;
	soap_destroy(&soap);
	soap_end(&soap);
	soap_done(&soap);

	std::chrono::time_point<std::chrono::high_resolution_clock> p1 = std::chrono::high_resolution_clock::now();
	cout << "stitch high_resolution_clock time:" << (float)std::chrono::duration_cast<std::chrono::microseconds>(p1 - p0).count() / 1000 << "ms" << endl;

	getchar();
}

