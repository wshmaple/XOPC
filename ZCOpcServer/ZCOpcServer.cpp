// ZCOpcServer.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include "opcsvrda2/opcsvrda2.h"
#include "simutag/simutag.h"
#include "soap/stdsoap2.h"
#include "soap/soapH.h"
#include "soap/add.nsmap"
#include "ThreadPool.h"
#include "cjson/cJSON.h"
#include "CInitSocket.h"
// #include "HttpConnect.h"
#include <curl/curl.h>
#include <iostream>
#include <sstream>
#include <string>

#include "resource.h"
#include "config.h"
#pragma comment(lib,"wsock32.lib")
//#define IDI_ICON1                       101
ThreadPool g_threadsoap;
COpcSvrThread g_opcsvr; // 定义一个全局的OpcServerThread类

// ProgID就按照 Vendor.DriverName.Version格式生成。因此ProgID = tom.opcmfctst.1
const char* g_sClsID = "{604F6299-E551-4202-8686-CDD18C4ECDAF}";// CLSID 用GUID工具产生的Registry Format
const char* g_sVendor = "zc";									// 提供商,比如"tom"
const char* g_sDrivername = "zc_opc";							// 驱动名,比如"rdb4opcda"
const char* g_sVersion = "1";									// 版本名,比如"1" 
const char*	g_sOpcServerDes = "zc_opc server";		// OPC Server描述 ，<128字符
TCHAR tOpcKeyAll[1024] = { 0 };
TCHAR tOpcKeyValue[1024] = { 0 };
char cOpcKeyAll[1024] = { 0 };
char cOpcKeyValue[1024] = { 0 };
char cOpcKeyPath[1024] = { 0 };
TCHAR tOpcKeyPath[1024] = { 0 };
TCHAR tOpcLogCfg[1024] = { 0 };
TCHAR tOpcLogCfg2[1024] = { 0 };
char clogpath[1024] = { 0 };
char clogpath2[1024] = { 0 };

char cwebname[255] = {0};
CSimuTags	m_tags;		// 模拟数据标签表	
CInitSocket initSock; //初始化Winsock库
size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream);
bool LoadXmlKey();
LPCTSTR FindOneOf(LPCTSTR p1, LPCTSTR p2)
{
	while (p1 != NULL && *p1 != NULL)
	{
		LPCTSTR p = p2;
		while (p != NULL && *p != NULL)
		{
			if (*p1 == *p)
				return CharNext(p1);
			p = CharNext(p);
		}
		p1 = CharNext(p1);
	}
	return NULL;
}

bool ctrlhandler(DWORD fdwctrltype)
{
	switch (fdwctrltype)
	{
		// handle the ctrl-c signal.
	case CTRL_C_EVENT:
		printf("ctrl-c event\n\n");
		cout << "servicer exit...." << endl;
		g_opcsvr.Stop(); // 停止OPC服务器
//		OutputDebugString(L"exite.............................");
		
		return(true);
		break;
		// ctrl-close: confirm that the user wants to exit.
	case CTRL_CLOSE_EVENT:
		printf("ctrl-close event\n\n");
		cout << "servicer exit...." << endl;
		Sleep(3000);
		g_opcsvr.Stop(); // 停止OPC服务器
	//	OutputDebugString(L"exite.............................");
		Sleep(3000);
		cout << "servicer exit...." << endl;
		Sleep(3000);
		return(true);
		break;
		// pass other signals to the next handler.
	case CTRL_BREAK_EVENT:
		printf("ctrl-break event\n\n");
		cout << "servicer exit...." << endl;
		g_opcsvr.Stop(); // 停止OPC服务器
		//OutputDebugString(L"exite.............................");
		return false;
		break;
	case CTRL_LOGOFF_EVENT:
		printf("ctrl-logoff event\n\n");
		cout << "servicer exit...." << endl;
		g_opcsvr.Stop(); // 停止OPC服务器
	//	OutputDebugString(L"exite.............................");
		return false;
		break;
	case CTRL_SHUTDOWN_EVENT:
		printf("ctrl-shutdown event\n\n");
		cout << "servicer exit...." << endl;
		g_opcsvr.Stop(); // 停止OPC服务器
	//	OutputDebugString(L"exite.............................");
		return false;
		break;
	default:
		return false;
	}
}
DWORD WINAPI Show(LPVOID lp) {
	//创建套接字
	while (true)
	{
		m_tags.Show();
		Sleep(5000);
	}
	return 0;
}
DWORD WINAPI UiRun(LPVOID lp) {
	//int nTick = 0;
	//wchar_t cMsg[32] = { 0 };
	//while (true)
	//{	
	//	wsprintf(cMsg,_T("Tick=%d\n"), nTick);
	//	OutputDebugString(cMsg);
	//	Sleep(1000);
	//	nTick++;
	//}
	//PostQuitMessage(WM_QUIT);


	//创建套接字
	SOCKET sListen = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sListen == INVALID_SOCKET)
	{
		printf("Failed socket()\n");
		return 0;
	}
	//填充sockaddr_in结构
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(24567);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	//绑定这个套接字到一个本地地址
	if (::bind(sListen, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("Failed bind()\n");
		return 0;
	}
	//进入监听模式
	if (::listen(sListen, 2) == SOCKET_ERROR)
	{
		printf("Failed listen()\n");
		return 0;
	}
	//循环接受客户的连接请求
	sockaddr_in remoteAddr;
	int nAddrLen = sizeof(remoteAddr);
	//printf("Length of sockaddr_in is: %d", nAddrLen);
	SOCKET sClient;
	char szText[] = "{\"key\":\"connect\"}";
	while (TRUE)
	{
		sClient = ::accept(sListen, (SOCKADDR *)&remoteAddr, &nAddrLen);
		if (sClient == INVALID_SOCKET)
		{
			printf("Failed accapt()");
			continue;
		}
	//	printf("接受到一个连接:%s\r\n", inet_ntoa(remoteAddr.sin_addr));
		//向客户端发送数据
		::send(sClient, szText, strlen(szText), 0);
	}
	//关闭客户端的连接
	::closesocket(sListen);
	return 0;
}

int ns__GetInfo(struct soap *add_soap, std::string ConfigInfo, std::string &result)
{
	//LOGI(ConfigInfo);
	LOGI("更新数据");
	//cout << ConfigInfo.c_str() << endl;
	//result = ConfigInfo; IOPCGroupStateMgt
	//OutputDebugStringA(ConfigInfo.c_str());
	cJSON * root = NULL;
	cJSON * itemip = NULL;//cjson对象
	cJSON * itemdata = NULL;//cjson对象 值
	cJSON * itemtype = NULL;//cjson对象 值
	//char * sk = "{\"key\":\"setopctag\",\"data\":[{\"sname\":\"key_hello_world\",\"ucType\":\"11\",\"ucWriteable\":\"0\",\"cvalue\":\"helloworld\",\"sDes\":\"helloworld\",\"sUnit\":\"%\"},{\"sname\":\"key_jone\",\"ucType\":\"11\",\"cvalue\":\"aksdjflajdfjkjsldafj\"}]}";
	root = cJSON_Parse(ConfigInfo.c_str());

	cJSON * reRoot = cJSON_CreateObject(); // 结果集
	int nTotal = 0;
	int nOkCount = 0; 
	int nErrorCount = 0;
	if (!root)
	{
		cJSON_AddStringToObject(reRoot, "msg", "error json");
		cJSON_AddStringToObject(reRoot, "type", "error");
	}
	else
	{
		cJSON_AddStringToObject(reRoot, "msg", "json ok");
		cJSON_AddStringToObject(reRoot, "type", "ok");
		// json 值
		/* 
		{
			"key": "setopctag", //命令名称
			"data": [{
				"o_key": "key_hello_world", //<80字符 名称
				"o_type": "11", //<2字符 [1.char,5.int,9.float,11.string] 数据类型
				"o_writeable": "0", //<2字符[0.不可写,1.可写] 可写标志
				"o_val": "helloworld", //<32字节 值
				"o_descripe": "helloworld", //<80字符 描述
				"o_unit": "%" //<16字符 单位	
			}, {
				"o_key": "key_jone", //<80字符 名称
				"o_type": "11", //<2字符 [1.char,5.int,9.float,11.string] 数据类型
				"o_val": "aksdjflajdfjkjsldafj" //<32字节 值
			}]
		}
		*/
		itemtype = cJSON_GetObjectItem(root, "key");
		string strKey = itemtype->valuestring;
		if (strKey == "setopctag") {
			itemdata = cJSON_GetObjectItem(root, "data");
			int iSize = cJSON_GetArraySize(itemdata);
			nTotal = iSize;
			//cJSON  *pJsonArry, *pJsonsub;
			//pJsonArry = cJSON_CreateArray();   /*创建数组*/
			//cJSON_AddItemToArray(pJsonArry, pJsonsub = cJSON_CreateObject()); /* 给创建的数组增加对对象*/
			for (int iCnt = 0; iCnt < iSize; iCnt++)
			{
				cJSON * pSub = cJSON_GetArrayItem(itemdata, iCnt);
				if (NULL == pSub)
				{
					nErrorCount++;
					continue;
				}
				cJSON * jname = NULL;
				cJSON * jType = NULL;
				cJSON * jWriteabel = NULL;
				cJSON * jValue = NULL;
				cJSON * jDes = NULL;
				cJSON * jUnit = NULL;
				string strName="";
				string strType = "";
				string strWriteable = "";
				string strValue = "";
				string strDes = "";
				string strUnit = "";
				jname = cJSON_GetObjectItem(pSub, "o_key");
				jType = cJSON_GetObjectItem(pSub, "o_type");
				jWriteabel = cJSON_GetObjectItem(pSub, "o_writeable");
				jValue = cJSON_GetObjectItem(pSub, "o_val");
				jDes = cJSON_GetObjectItem(pSub, "o_descripe");
				jUnit = cJSON_GetObjectItem(pSub, "o_unit");
				int nWriteable = OTEM_NOWRITE;
				if (jname != NULL  && jValue!=NULL 
					&& jname->type != cJSON_NULL
					&& jValue->type != cJSON_NULL) {
					strName = jname->valuestring; // 标签名称
					strType = jType->valuestring; // 类型
					strValue = jValue->valuestring; // 数据
					//OutputDebugStringA(strName.c_str());
					if (jWriteabel != NULL &&jWriteabel->type != cJSON_NULL)
					{
						strWriteable = jWriteabel->valuestring; // 读写权限
						if (strWriteable == "1")
						{
							nWriteable = OTEM_WRITE;
						}
					}
					if (jDes&&jDes->type != cJSON_NULL)
					{
						strDes = jDes->valuestring; // 描述
					}
					if (jUnit&&jUnit->type != cJSON_NULL)
					{
						strUnit = jUnit->valuestring; //标签单位
					}
					int nType = ODT_STR;//<2字符 [1.char,5.int,9.float,11.string] 数据类型
					if (strType == "1")
					{
						nType = ODT_I8;
					}
					else if (strType == "3")
					{
						nType = ODT_I16;
					}
					else if (strType == "5")
					{
						nType = ODT_I32;
					}
					else if (strType == "9")
					{
						nType = ODT_F32;
					}
					else if (strType == "11")
					{
						nType = ODT_STR;
					}
				
				int nRet=m_tags.AddTag2(strName.c_str(), 
						strDes.c_str(),
						strUnit.c_str(),
						nType,
						nWriteable,
						"zcopc",
						strValue.c_str()
					);
				if (nRet ==1) {
					// 添加 tag 并 设置值成功
					//cJSON_AddStringToObject(pJsonsub, strName.c_str(), "1");      /* 给对象增加内容 */
					nOkCount++;
				}
				else if (nRet == 2) {
					// 设置值成功
					//cJSON_AddStringToObject(pJsonsub, strName.c_str(), "2");      /* 给对象增加内容 */
					nOkCount++;
				}
				}
				else {
					nErrorCount++;
				}
				
			}
		}
	}
	
	cJSON_AddNumberToObject(reRoot, "nTotal", nTotal);
	cJSON_AddNumberToObject(reRoot, "nOkCount", nOkCount);
	cJSON_AddNumberToObject(reRoot, "nErrorCount", nErrorCount);
	//char * js = cJSON_PrintUnformatted(reRoot);
	//if (js)
	//{
	//	result = string(js);
	//	free(js);
	//}
	//OutputDebugStringA(result.c_str());
	cJSON_Delete(reRoot);
	cJSON_Delete(root);
	return SOAP_OK;
}
void GetOpcUpdate()
{
	/*HTTP GET json data*/
	std::stringstream out;
	CURL* curl = curl_easy_init();
	// 设置URL
	curl_easy_setopt(curl, CURLOPT_URL, cOpcKeyValue);

	// 设置接收数据的处理函数和存放变量
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out);
	LOGD("请求获取最新OPC数据...");
	// 执行HTTP GET操作
	CURLcode res = curl_easy_perform(curl);
	if (res != CURLE_OK) {
		fprintf(stderr, "connect web failed: %s\n", curl_easy_strerror(res));
		LOGE("connect web failed: "<< curl_easy_strerror(res));
		LOGE("请求获取最新OPC数据失败！");
	}

	// 接受数据存放在out中，输出之
	curl_easy_cleanup(curl);
}
DWORD WINAPI SoapRun(LPVOID lpParam) {
	int m, s;
	int nPort = 17890;//这个是端口号,javaWeb端也要这个端口,与服务端连接
	struct soap platform_soap;
	soap_init(&platform_soap);
	platform_soap.accept_timeout = 4000;
	platform_soap.send_timeout = 4000;
	m = soap_bind(&platform_soap, NULL, nPort, 10);
	if (m < 0)
	{
		//ctcp.LogPrint("正晨服务", "gsoap_bind链接异常！");
		soap_print_fault(&platform_soap, stderr);
		fprintf(stderr, "gsoap_bind链接异常！err = %d\n", m);
		LOGE("gsoap_bind链接异常！err = "<< m);
		exit(-1);
	}
	//GetOpcUpdate();
	for (;;)
	{
		s = soap_accept(&platform_soap);
		if (s < 0)
		{
			//	ctcp.LogPrint("正晨服务", "gsoap_accept异常！");
			soap_print_fault(&platform_soap, stderr);
			LOGE(stderr);
			exit(-1);
		}
		//fprintf(stderr, "Socket connection successful: slave socket = %d\n", s);

		// 拷贝一份
		struct soap *psp2 = NULL;
		psp2 = soap_copy(&platform_soap); //make a safe copy
		if (psp2 != NULL)
		{
			// 针对不同连接
			psp2->socket = s;
			auto rtBuf = g_threadsoap.enqueue([psp2] {
				if (!soap_valid_socket(psp2->socket))
				{
					//OutputDebugString(_T("soap_valid_socket(psp2->socket)"));
					return 0;
				}
				//printf("当前运行线程是：%d\n", this_thread::get_id());
				// 消息处理和销毁
				soap_serve(psp2);  // 自动调用接口函数

				soap_destroy(psp2); // dealloc c++ data
				soap_end(psp2); // Dealloc data and clean up
				soap_done(psp2); //Detach soap Struct
				free(psp2); // 释放指针
				return 0;
			});
		}
		else {
		//	printf("platform_soap 拷贝失败！");
			soap_destroy(psp2); // dealloc c++ data
			soap_end(psp2); // Dealloc data and clean up
			soap_done(psp2); //Detach soap Struct
			free(psp2); // 释放指针
		}

	}
	return 0;
}

#include "Markup.h"
#include <comutil.h>  
//#include "D:/Windows Kits/10/Include/10.0.18362.0/um/WinUser.h"
#pragma comment(lib, "comsuppw.lib")
//将wstring转换成string  

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

void check()
{

		TCHAR MyDir[_MAX_PATH];

		SHGetSpecialFolderPath(NULL, MyDir, CSIDL_PERSONAL, 0);
		//	printf("路径2：%ws \n", MyDir);

		TCHAR MyDirtemp[_MAX_PATH];

		wsprintf(MyDirtemp, _T("%s%s"), MyDir, _T("\\conf.xml"));
		//	printf("路径3：%ws \n", MyDirtemp);
		//	FILE* fp = _tfopen(MyDirtemp, _T("wb"));

		//	if (fp != NULL)
		{

			//	if (dwSize == fwrite(pBuffer, sizeof(char), dwSize, fp))
			{
				CMarkup xml;
				bool brt = xml.Load(MyDirtemp);
				int nCount = 0;
				while (xml.FindElem(_T("data"))) {
					nCount++;
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

					//printf("单位:%s\t描述：%s\t指令：%s\t\n", it.c_str(),
					//	des.c_str(),
					//	key.c_str());

					if (!key.empty())
					{
						int nWriteable = OTEM_NOWRITE;

						int nType = ODT_STR;//<2字符 [1.char,5.int,9.float,11.string] 数据类型

						int nRet = m_tags.AddTag2(key.c_str(),
							des.c_str(),
							it.c_str(),
							nType,
							nWriteable,
							"zcopc",
							"0"
						);
					}
					else {

					}
					printf("%d,%s,%s,%s\n", nCount, it.c_str(), des.c_str(), key.c_str());
				}
				printf("kk3-- %d\n", nCount);
			}
			//		fclose(fp);
		}
}
void kk()
{
	HRSRC hRsrc = ::FindResource(NULL, MAKEINTRESOURCE(IDR_XML1), TEXT("xml"));
	if (NULL == hRsrc)
	{
		return;
	}
	//获取资源的大小
	DWORD dwSize = SizeofResource(NULL, hRsrc);
	if (0 == dwSize)
	{
		return;
	}
	//加载资源
	HGLOBAL hGlobal = LoadResource(NULL, hRsrc);
	if (NULL == hGlobal)
	{
		return;
	}
	//锁定资源
	LPVOID pBuffer = LockResource(hGlobal);
	if (NULL == pBuffer)
	{
		return;
	}
	BOOL bRt = FALSE;
	TCHAR MyDir[_MAX_PATH];
	SHGetSpecialFolderPath(NULL, MyDir, CSIDL_PERSONAL, 0);
	TCHAR MyDirtemp[_MAX_PATH];
	wsprintf(MyDirtemp,_T("%s%s"), MyDir, _T("\\conf.xml"));
	FILE* fp = _tfopen(MyDirtemp, _T("wb"));
	if (fp != NULL)
	{
		if (dwSize == fwrite(pBuffer, sizeof(char), dwSize, fp))
		{
			bRt = TRUE;
		}
		fclose(fp);
	}
	::FreeResource(hGlobal);
	check();
	//DeleteFile(MyDirtemp);
}

size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
	string data((const char*)ptr, (size_t)size * nmemb);

	*((stringstream*)stream) << data ;

	return size * nmemb;
}
string Utf8ToGbk(const char *src_str)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, src_str, -1, NULL, 0);
	wchar_t* wszGBK = new wchar_t[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, src_str, -1, wszGBK, len);
	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
	char* szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
	string strTemp(szGBK);
	if (wszGBK) delete[] wszGBK;
	if (szGBK) delete[] szGBK;
	return strTemp;
}

string UTF8ToGBK(const std::string& strUTF8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, NULL, 0);
	WCHAR* wszGBK = new WCHAR[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, (LPCCH)strUTF8.c_str(), -1, wszGBK, len);

	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
	char *szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
	std::string strTemp(szGBK);
	delete[]szGBK;
	delete[]wszGBK;
	return strTemp;
}

//UTF-8到GB2312的转换
char* U2G(const char* utf8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);
	len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
	if (wstr) delete[] wstr;
	return str;
}


char* U8ToUnicode(const char* szU8)
{
	//UTF8 to Unicode
	//预转换，得到所需空间的大小
	int wcsLen = ::MultiByteToWideChar(CP_UTF8, NULL, szU8, strlen(szU8), NULL, 0);
	//分配空间要给'\0'留个空间，MultiByteToWideChar不会给'\0'空间
	wchar_t* wszString = new wchar_t[wcsLen + 1];
	//转换
	::MultiByteToWideChar(CP_UTF8, NULL, szU8, strlen(szU8), wszString, wcsLen);
	//最后加上'\0'
	wszString[wcsLen] = '\0';

	char *m_char;
	int len = WideCharToMultiByte(CP_ACP, 0, wszString, wcslen(wszString), NULL, 0, NULL, NULL);
	m_char = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, wszString, wcslen(wszString), m_char, len, NULL, NULL);
	m_char[len] = '\0';
	return m_char;
}


void deleteAllMark(string &s, const string &mark)
{
	size_t nSize = mark.size();
	while (1)
	{
		size_t pos = s.find(mark);    //  尤其是这里
		if (pos == string::npos)
		{
			return;
		}
		s.erase(pos, nSize);
	}
}
void ParseOpc(string js)
{
	deleteAllMark(js, " ");
	deleteAllMark(js, "\r");
	deleteAllMark(js, "\n");
	cJSON* root = NULL;
	cJSON* itemip = NULL;//cjson对象
	cJSON* itemdata = NULL;//cjson对象 值
	cJSON* itemtype = NULL;//cjson对象 值
						   //char * sk = "{\"key\":\"setopctag\",\"data\":[{\"sname\":\"key_hello_world\",\"ucType\":\"11\",\"ucWriteable\":\"0\",\"cvalue\":\"helloworld\",\"sDes\":\"helloworld\",\"sUnit\":\"%\"},{\"sname\":\"key_jone\",\"ucType\":\"11\",\"cvalue\":\"aksdjflajdfjkjsldafj\"}]}";
	root = cJSON_Parse(js.c_str());
	if (root)
	{
		//printf("JSON OK!\n");
		LOGI("JSON OK!");
	}
	else {
	//	printf("JSON error !\n");
		LOGE("JSON error!");
	}
	itemdata = cJSON_GetObjectItem(root, "data");
	int iSize = cJSON_GetArraySize(itemdata);
	for (int iCnt = 0; iCnt < iSize; iCnt++)
	{
		cJSON* pSub = cJSON_GetArrayItem(itemdata, iCnt);
		if (NULL == pSub)
		{
			continue;
		}
		cJSON* jname = NULL;
		cJSON* jType = NULL;
		cJSON* jWriteabel = NULL;
		cJSON* jValue = NULL;
		cJSON* jDes = NULL;
		cJSON* jUnit = NULL;
		string strName = "";
		string strType = "";
		string strWriteable = "";
		string strValue = "";
		string strDes = "";
		string strUnit = "";
		jname = cJSON_GetObjectItem(pSub, "opcKey");
		//jType = cJSON_GetObjectItem(pSub, "o_type");
		//jWriteabel = cJSON_GetObjectItem(pSub, "o_writeable");
		//	jValue = cJSON_GetObjectItem(pSub, "o_val");
		jDes = cJSON_GetObjectItem(pSub, "describe");
		jUnit = cJSON_GetObjectItem(pSub, "unit");

		if (jname&&jname->type != cJSON_NULL) {
			strName = jname->valuestring; // 标签名称

			if (jDes && jDes->type != cJSON_NULL)
			{
				strDes = jDes->valuestring; // 描述
			}
			else
			{
				strDes = "";
			}
			if (jUnit&&jUnit->type != cJSON_NULL)
			{
				strUnit = jUnit->valuestring; //标签单位
			}

			
			int nWriteable = OTEM_NOWRITE;

			int nType = ODT_STR;//<2字符 [1.char,5.int,9.float,11.string] 数据类型

			int nRet = m_tags.AddTag2(strName.c_str(),
				strDes.c_str(),
				strUnit.c_str(),
				nType,
				nWriteable,
				"zcopc",
				"0"
			);

			//std::cout << iCnt << "\t" << strName.c_str() << "\t" << strUnit.c_str() << "\t" << strDes.c_str() << std::endl;
		}
	}
}

bool GetOpcFromWeb()
{
	// 基于当前系统的当前日期/时间
	/*HTTP GET json data*/
	std::stringstream out;
	CURL* curl = curl_easy_init();
	// 设置URL
	curl_easy_setopt(curl, CURLOPT_URL, cOpcKeyAll);
	//在数据头设置字符集为UTF-8，解决中文乱码问题
	struct curl_slist *head = NULL;
	head = curl_slist_append(head, "Content-Type:application/x-www-form-urlencoded;charset=UTF-8");
	head = curl_slist_append(head, "Accept-Language: zh-cn");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, head);

	// 设置接收数据的处理函数和存放变量
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out);
	LOGD("获取最新OPC点表... " );
	// 执行HTTP GET操作
	CURLcode res = curl_easy_perform(curl);
	if (res != CURLE_OK) {
		fprintf(stderr, "connect web failed: %s\n", curl_easy_strerror(res));	
		LOGE("connect web failed: " << curl_easy_strerror(res));
		LOGE("获取最新OPC点表失败！ ");
		return false;
	}

	// 接受数据存放在out中，输出之
	//cout << out.str() << endl;
	string str_json = out.str();
	string ss = Utf8ToGbk(str_json.c_str());
	ParseOpc(ss);
	curl_slist_free_all(head);//记得要释放
	curl_easy_cleanup(curl);
	return true;
}
void InitTags()
{

//	kk();//从资源加载

	bool bRt = GetOpcFromWeb();
	if (!bRt)
	{
		bool bxRt = LoadXmlKey(); // 从本地路径加载
		//printf("本地加载成功！\n");
		if(bxRt)
		LOGI("本地加载点表成功！");
		else LOGE("本地加载点表失败！");
	}
	else {
		//printf("在线加载成功! \n");
		LOGI("在线加载点表成功！");
	}
}
void SetUrl()
{
	wchar_t runPath[MAX_PATH];
	GetModuleFileName(NULL, runPath, MAX_PATH);
	//printf("GetModuleFileName当前路径:%ws\n", runPath);
	TCHAR *pChr = _tcsrchr(runPath, _T('\\'));
	pChr++;
	*pChr = _T('\0');

	lstrcpy(tOpcKeyPath, runPath);
	lstrcpy(tOpcLogCfg, runPath);
	lstrcpy(tOpcLogCfg2, runPath);

	lstrcat(tOpcKeyPath, _T("Config\\key.xml"));
	lstrcat(runPath, _T("Config\\Config.ini"));
	lstrcat(tOpcLogCfg, _T("Config\\log.cfg"));
	lstrcat(tOpcLogCfg2, _T("log\\"));

	TCHAR ips[MAX_PATH] = { 0 };
	TCHAR port[MAX_PATH] = { 0 };
	TCHAR webname[MAX_PATH] = { 0 };
	GetPrivateProfileString(_T("BESIP端口配置"), _T("IP"), _T(""), ips, MAX_PATH, runPath);
	GetPrivateProfileString(_T("BESIP端口配置"), _T("PORT"), _T(""), port, MAX_PATH, runPath);
	GetPrivateProfileString(_T("BESIP端口配置"), _T("WEBNAME"), _T(""), webname, MAX_PATH, runPath);


	lstrcat(tOpcKeyAll, _T("http://"));
	lstrcat(tOpcKeyAll, ips);
	lstrcat(tOpcKeyAll, _T(":"));
	lstrcat(tOpcKeyAll, port);
	lstrcat(tOpcKeyAll, _T("/"));
	lstrcat(tOpcKeyAll, webname);
	lstrcat(tOpcKeyAll, _T("/view/basedatamanage/eqmanage/getOpcKeyAll"));


	lstrcat(tOpcKeyValue, _T("http://"));
	lstrcat(tOpcKeyValue, ips);
	lstrcat(tOpcKeyValue, _T(":"));
	lstrcat(tOpcKeyValue, port);
	lstrcat(tOpcKeyValue, _T("/"));
	lstrcat(tOpcKeyValue, webname);
	lstrcat(tOpcKeyValue, _T("/view/basedatamanage/eqmanage/opcLoginSucceedCallback"));

	printf("%ws", tOpcKeyValue);

	sprintf(cOpcKeyAll, "%ws", tOpcKeyAll);
	sprintf(cOpcKeyValue, "%ws", tOpcKeyValue);

	sprintf(clogpath, "%ws", tOpcLogCfg);
	sprintf(clogpath2, "%ws", tOpcLogCfg2);
}
bool LoadXmlKey()
{
	// 从文件夹加载 key
	string path = string(cOpcKeyPath);
	CMarkup xml;
	bool brt = xml.Load(tOpcKeyPath);
	if (!brt)
	{
		//printf("XML OK !");
		LOGI("XML OK !");
	}
	else {
		//printf("XML ERROR !");
		LOGE("XML ERROR !");
		return false;
	}
	int nCount = 0;
	while (xml.FindElem(_T("data"))) {
		nCount++;
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
		if (!key.empty())
		{
			int nWriteable = OTEM_NOWRITE;

			int nType = ODT_STR;//<2字符 [1.char,5.int,9.float,11.string] 数据类型

			int nRet = m_tags.AddTag2(key.c_str(),
				des.c_str(),
				it.c_str(),
				nType,
				nWriteable,
				"zcopc",
				"0"
			);
		}
		else {

		}
		//printf("%d,%s,%s,%s\n", nCount, it.c_str(), des.c_str(), key.c_str());
	}
//	printf("kk3-- %d\n", nCount);
	LOGI("本地成功加载：" << nCount <<"个！");
	return true;
}
int main(int argc, char* argv[])
{
	LOGI("OPC Server 2.0!");
	SetUrl();
	ILog4zManager::getRef().config(clogpath);
	//start log4z
	ILog4zManager::getRef().start();
	ILog4zManager::getRef().setLoggerPath(LOG4Z_MAIN_LOGGER_ID, clogpath2);
	g_threadsoap.SetPoolSize(4);
	if (argc == 2)
	{
		// 命令行个数
		if (strcmp(argv[1], "-reg") ==0) {
			if (OE_OK != DA2_Reg(g_sClsID, g_sVendor, g_sDrivername, g_sVersion, g_sOpcServerDes, false))
			{
				LOGE("Register failed! Need Administrator power!注册失败,访问注册表失败,请用管理员权限运行!");
				MessageBox(NULL, _T("Register failed! Need Administrator power!\n注册失败,访问注册表失败,请用管理员权限运行!"), _T("提示"), MB_OK | MB_ICONHAND);
				return OE_ERR;
			};
			LOGI("Register Success!注册成功!");
			MessageBox(NULL, _T("Register Success!\n注册成功!"), _T("提示"), MB_OK | MB_ICONINFORMATION);
			return 0;
		}
		else if (strcmp(argv[1],"-unreg") == 0)
		{
			if (OE_OK != DA2_UnReg(g_sClsID, g_sVendor, g_sDrivername, g_sVersion, g_sOpcServerDes))
			{
				LOGE("UnRegister failed! Need Administrator power!卸载失败,访问注册表失败,请用管理员权限运行!");
				MessageBox(NULL, _T("UnRegister failed! Need Administrator power!\n卸载失败,访问注册表失败,请用管理员权限运行!"), _T("提示"), MB_OK | MB_ICONHAND);
				return OE_ERR;
			}
			LOGI("UnRegister Success!卸载成功!");
			MessageBox(NULL, _T("UnRegister Success!\n卸载成功!"), _T("提示"), MB_OK | MB_ICONINFORMATION);
			return 0;
		}
	}
	//m_tags.InitTags();	// 添加标签
	// 添加标签
	InitTags();

//	printf("初始化成功!\n");
	LOGI("初始化成功!");
	// 启动opc服务器
	if (!g_opcsvr.Start(g_sClsID, g_sVendor, g_sDrivername, g_sVersion, g_sOpcServerDes, CSimuTags::CB_SetTagVal, &m_tags, true))
	{
		LOGE("DA2_Initialize Error!\n初始化失败!");
		MessageBox(NULL, _T("DA2_Initialize Error!\n初始化失败!"), _T("提示"), MB_OK | MB_ICONINFORMATION);
		return FALSE;
	}

	DWORD threadID2;
	HANDLE hThread2;
	hThread2 = CreateThread(NULL, 0, SoapRun, NULL, 0, &threadID2);
	
	DWORD threadOpc;
	HANDLE hThreadOpc;
	hThreadOpc = CreateThread(NULL, 0, Show, NULL, 0, &threadOpc);

	GetOpcUpdate();
//	WaitForSingleObject(hThreadOpc, INFINITE); // 等待，直到线程被激发



	//if (SetConsoleCtrlHandler((PHANDLER_ROUTINE)ctrlhandler, TRUE))
	//{
	//	cout << "Control Handler is installed" << endl;
	//	cout << " Ctrl+C, Ctrl+Break, logging off or closing console NOW intercepted." << endl;
	//	cout << " ... into message loop.\n" << endl;
	//	while (1) {
	//		Sleep(100);
	//		cout << "---------" << endl;
	//	}
	//}
	//else
	//{
	//	cout << "Control handler setting failed...." << endl;
	//}
	//Sleep(20000);
	//g_opcsvr.Stop();

	WaitForSingleObject(hThreadOpc, INFINITE); // 等待，直到线程被激发

	CloseHandle(hThreadOpc);
	
	CloseHandle(hThread2);
	LOGI("退出！");
}
