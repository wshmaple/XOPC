// testXml.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "Markup.h"
#include <string>
using namespace std;
#include <shlobj.h>   

#include <comutil.h>  
#pragma comment(lib, "comsuppw.lib")
string wstring2string(const wstring& ws)
{
	_bstr_t t = ws.c_str();
	char* pchar = (char*)t;
	string result = pchar;
	return result;
}
void kk()
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

int main()
{
	kk();
	getchar();
    return 0;
}

