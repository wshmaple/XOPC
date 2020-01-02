// ZCBesCore.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
//#include "../ZCOpcServer/HttpConnect.h"
#include <string>
#include <sstream>
#include <iostream>
#include <tchar.h>
using namespace std;

#include "../ZCOpcServer/Markup.h"
#include <comutil.h>  
#pragma comment(lib, "comsuppw.lib")
//将wstring转换成string  
#include "resource.h"
#include "D:/Windows Kits/10/Include/10.0.17134.0/um/WinUser.h"
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

BOOL GetRes(LPVOID pBuffer)
{
	HRSRC hRsrc = ::FindResource(NULL, MAKEINTRESOURCE(IDR_XML1), TEXT("xml"));
	if (NULL == hRsrc)
		return FALSE;
	//获取资源的大小

	DWORD dwSize = SizeofResource(NULL, hRsrc);

	if (0 == dwSize)

		return FALSE;

	//加载资源

	HGLOBAL hGlobal = LoadResource(NULL, hRsrc);

	if (NULL == hGlobal)

		return FALSE;

	//锁定资源

	pBuffer = LockResource(hGlobal);

	if (NULL == pBuffer)

		return FALSE;
	return TRUE;
}
void kk()
{
	HRSRC hRsrc = ::FindResource(NULL, MAKEINTRESOURCE(IDR_XML1), TEXT("xml"));
	if (NULL == hRsrc)
		return ;
	//获取资源的大小

	DWORD dwSize = SizeofResource(NULL, hRsrc);

	if (0 == dwSize)

		return ;

	//加载资源

	HGLOBAL hGlobal = LoadResource(NULL, hRsrc);

	if (NULL == hGlobal)

		return ;

	//锁定资源

	LPVOID pBuffer = LockResource(hGlobal);

	if (NULL == pBuffer)

		return ;


	char* p = (char*)pBuffer;
	CMarkup xml;
	//xml.Load(_T("conf.json"));
	string sk = string(p);
	xml.SetDoc(s2ws(sk).c_str());
	while (xml.FindElem(_T("data"))) {
		xml.IntoElem();
		xml.FindElem(_T("unit"));
		MCD_STR strunit = xml.GetData();
		xml.FindElem(_T("describe"));
		MCD_STR strdescribe = xml.GetData();
		xml.FindElem(_T("opcKey"));
		MCD_STR stropcKey = xml.GetData();
		xml.OutOfElem();
		string a = wstring2string(strunit);
		string b = wstring2string(strdescribe);
		string c = wstring2string(stropcKey);
		printf("%s,%s,%s\n", a.c_str(), b.c_str(), c.c_str());

	}
}
int main()
{
    std::cout << "Hello World!\n"; 
	kk();
	//HttpConnect* http = new HttpConnect();
	//http://172.16.14.95:8082/BESServer/view/basedatamanage/eqmanage/getOpcKeyAll
	//string ss = http->getData("172.16.14.95", "/BESServer/view/basedatamanage/eqmanage/getOpcKeyAll", "");
	
	//std::cout << ss.c_str() << endl;
	getchar();
}
