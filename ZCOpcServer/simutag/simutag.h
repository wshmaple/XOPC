// simutag.h 数据基类，封装了线程、线程同步、客户端写数据回调函数，从这个类派生数据类比较方便
#ifndef _SIMU_TAG_H_
#define _SIMU_TAG_H_

#include <time.h>
#include "tom_syn.h"  // 多线程同步工具
#include "../config.h"
// 下面的CMyTags是一个数据模拟类，将模拟数据提交给OPC Server发布出去。
typedef struct t_tagitemtest
{
	char			sname[24];	// 名称
	char			sdes[32];	// 描述
	char			sunit[16];	// 单位
	unsigned char	uctype;		// 数据类型
	unsigned char	unwriteable;// 可写标志
	char            cValues[32];// 测试用的
	char			sres[6];	// 保留
}T_TAGITEMTEST,*PTAGITEMTEST;	// 测试用的标签定义 sizeof() = 80 

// 自己的数据类，定义数据
#define		TEST_TAGS	2200	// 测试标签个数,400个状态量，400个32整数,400个64位整数,400个float,400个double, 100个string，90个blob,10个不模拟的可写标签


#include <unordered_map>
#include <string>
#include <map>
using namespace std;

struct compare_str {
	bool operator()(const char* p1, const char*p2) const {
		return strcmp(p1, p2) == 0;
	}
};
typedef unordered_map<const char*, string, hash<const char*>, compare_str> StrIntMap;
typedef unordered_map<string, int> MyMap;
typedef unordered_map<string, int>::iterator MyIte;
typedef map<string, T_TAGITEMTEST> MyTagMap;
typedef map<string, T_TAGITEMTEST>::iterator MyMapIte;
StrIntMap ssk;
#define COUNT_MAX 11
#define COUNT_MAX_LEN COUNT_MAX-1
class CSimuTags
{
public:
	MyMap m_map;
	MyTagMap m_mapTags;
	int m_nCount[COUNT_MAX];
	void Show()
	{
		int nSize = m_map.size();
		//cout << "[count:"<<nSize <<"] update:";
		LOGD("[count:" << nSize << "] update:");
		for (int i = COUNT_MAX_LEN; i >-1; i--)
		{
			if (m_nCount[i] > 0)
			{
				//cout <<"["<<i<<"->"<< m_nCount[i] <<"]\t";
				LOGD("[" << i << "->" << m_nCount[i] << "]");
			}
		}
		cout << endl;
	}
	void Check(int i)
	{
		if (m_nCount[i] > 2047483647)
		{
			m_nCount[i] = 0;
			m_nCount[i+1]++;
		}
	
		if (m_nCount[COUNT_MAX_LEN] > 0)
		{
			for (int i = 0; i < COUNT_MAX_LEN; i++)
			{
				m_nCount[i] = 0;
			}
		}
	}
	void Count()
	{
		m_nCount[0]++;
		for (int i = 0; i < COUNT_MAX_LEN; i++)
		{
			Check(i);
		}
	}
	int AddTag(const char * sname,
		const	char *sdes, 
		const char *sunit,
		unsigned char uctype,
		unsigned char unwriteable, 
		const char * sres,
		const char *cvalue)
	{
		TOM::CSafeLock lock(&m_lock); //加临界区锁
		string strName = sname;
		if (m_map.find(strName) == m_map.end())
		{
			//std::cout <<"插入成功："<< sname << endl;
			m_map.insert(make_pair(strName, 1));
			T_TAGITEMTEST ta;
			strcpy(ta.sname, sname);
			strcpy(ta.sdes, sdes);
			strcpy(ta.sunit, sunit);
			ta.uctype = uctype;
			ta.unwriteable = unwriteable;
			strcpy(ta.cValues, cvalue);
			strcpy(ta.sres, sres);
			m_mapTags.insert(make_pair(strName, ta));
			DA2_AddTag(sname, sdes, sunit, uctype, unwriteable);
			
			SetTagValue(sname, cvalue, uctype);
			return 1;
		}
		else {
			strcpy(m_mapTags[strName].cValues, cvalue);
			SetTagValue(sname, cvalue, uctype);
			return 2;
		}
	}
	int AddTag2(const char* sname,
		const	char* sdes,
		const char* sunit,
		unsigned char uctype,
		unsigned char unwriteable,
		const char* sres,
		const char* cvalue)
	{
		TOM::CSafeLock lock(&m_lock); //加临界区锁
		string strName = sname;
		if (m_map.find(strName) == m_map.end())
		{
			//std::cout <<"插入成功："<< sname << endl;
			m_map.insert(make_pair(strName, 1));
			T_TAGITEMTEST ta;
			strcpy(ta.sname, sname);
			strcpy(ta.sdes, sdes);
			strcpy(ta.sunit, sunit);
			ta.uctype = uctype;
			ta.unwriteable = unwriteable;
			strcpy(ta.cValues, cvalue);
			strcpy(ta.sres, sres);
			m_mapTags.insert(make_pair(strName, ta));
			DA2_AddTag(sname, sdes, sunit, uctype, unwriteable);

			SetTagValue2(sname, cvalue, uctype,NULL);
			return 1;
		}
		else {
			strcpy(m_mapTags[strName].cValues, cvalue);
			SetTagValue2(sname, cvalue, uctype, NULL);
			return 2;
		}



		//string strName = sname;
		//m_map.insert(make_pair(strName, 1));

		//T_TAGITEMTEST ta;
		//strcpy(ta.sname, sname);
		//strcpy(ta.sdes, sdes);
		//strcpy(ta.sunit, sunit);
		//ta.uctype = uctype;
		//ta.unwriteable = unwriteable;
		//strcpy(ta.cValues, cvalue);
		//strcpy(ta.sres, sres);
		//m_mapTags.insert(make_pair(strName, ta));
		//DA2_AddTag(sname, sdes, sunit, uctype, unwriteable);
		//SetTagValue2(sname, cvalue, uctype, NULL);
	}
	void SetTagValue(const char * sname, const char *value, unsigned char uctype)
	{
		//SYSTEMTIME syst;
		//::GetSystemTime(&syst);
		//FILETIME ftime;
		//::SystemTimeToFileTime(&syst, &ftime);
		SetTagValue(sname, value, uctype,NULL);
	}

	inline std::string ToUTF8(const std::string str)
	{
		int nw_len = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

		wchar_t* pw_buf = new wchar_t[nw_len + 1];
		memset(pw_buf, 0, nw_len * 2 + 2);

		::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pw_buf, nw_len);

		int len = WideCharToMultiByte(CP_UTF8, 0, pw_buf, -1, NULL, NULL, NULL, NULL);

		char* utf8_buf = (char*)malloc(len + 1);
		memset(utf8_buf, 0, len + 1);

		::WideCharToMultiByte(CP_UTF8, 0, pw_buf, nw_len, utf8_buf, len, NULL, NULL);

		std::string outstr(utf8_buf);

		delete[] pw_buf;
		//delete[] utf8_buf;
		free(utf8_buf);
		utf8_buf = NULL;

		return outstr;
	}
	void SetTagValue2(const char* sname, const char* value, unsigned char uctype, FILETIME* pftime)
	{
		string strName = sname;
		if (m_map.find(strName) == m_map.end())
		{
			//std::cout << "插入成功：" << sname << endl;
			//m_map.insert(make_pair(strName, 1));
		}
		else {
			//std::cout << "没有插入成功！" << sname << endl;
			//char sval[80];
			//strcpy(sval, value);

			string strUt = ToUTF8(sname);
			string strvalue = ToUTF8(value);

			//std::cout << strUt << "\t" << strvalue << std::endl;
			Count();
			strcpy(m_mapTags[sname].cValues, value);

			switch (uctype) {
			case ODT_I8:
			{

			}
			break;
			case ODT_I16:
			{
			}
			break;
			case ODT_I32:
			{
				DA2_SetData_I32(sname, atoi(value), OPC_QUALITY_GOOD, pftime);
			}
			break;
			case ODT_I64:
			{
			}
			break;
			case ODT_F32:
			{
				DA2_SetData_F32(sname, atof(value), OPC_QUALITY_GOOD, pftime);
			}
			break;
			case ODT_STR:
			{
				DA2_SetData_Str(sname, value, OPC_QUALITY_GOOD, pftime);
			}
			break;
			case ODT_BLOB:
			{
				//DA2_SetData_Blob(sname, value, OPC_QUALITY_GOOD, &ftime);
			}
			break;
			}
		}
	}
	void SetTagValue(const char * sname, const char *value,unsigned char uctype, FILETIME *pftime)
	{
		string strName = sname;
		if (m_map.find(strName) == m_map.end())
		{
			//std::cout << "插入成功：" << sname << endl;
			//m_map.insert(make_pair(strName, 1));
		}
		else {
			//std::cout << "没有插入成功！" << sname << endl;
			//char sval[80];
			//strcpy(sval, value);
			
			string strUt = ToUTF8(sname);
			string strvalue = ToUTF8(value);

			std::cout << strUt << "\t" << strvalue << std::endl;

			strcpy(m_mapTags[sname].cValues, value);

			switch (uctype) {
			case ODT_I8:
			{
				
			}
			break;
			case ODT_I16:
			{
			}
			break;
			case ODT_I32:
			{
				DA2_SetData_I32(sname, atoi(value), OPC_QUALITY_GOOD, pftime);
			}
			break;
			case ODT_I64:
			{
			}
			break;
			case ODT_F32:
			{
				DA2_SetData_F32(sname, atof(value), OPC_QUALITY_GOOD, pftime);
			}
			break;
			case ODT_STR:
			{
				DA2_SetData_Str(sname, value, OPC_QUALITY_GOOD, pftime);
			}
			break;
			case ODT_BLOB:
			{
				//DA2_SetData_Blob(sname, value, OPC_QUALITY_GOOD, &ftime);
			}
			break;
			}
		}
	}
	void AddTag(char * sname) {
		string strName = sname;
		if (m_map.find(strName) == m_map.end())
		{
		//	std::cout << "插入成功：" << sname << endl;
			m_map.insert(make_pair(strName, 1));
			T_TAGITEMTEST ta;
			strcpy(ta.sname, sname);
			m_mapTags.insert(make_pair(sname,ta));
		}
		else {
			//std::cout << "没有插入成功！" << sname << endl;
		}
	}
	void AddTag(char * sname,int nValue) {
		string strName = sname;
		if (m_map.find(strName) == m_map.end())
		{
			//std::cout << "插入成功：" << sname << endl;
			m_map.insert(make_pair(strName, 1));
			T_TAGITEMTEST ta;
			//ta.nValue = nValue;
			strcpy(ta.sname, sname);
			m_mapTags.insert(make_pair(sname, ta));
		}
		else {
			//std::cout << "没有插入成功！" << sname << endl;
			//m_mapTags[sname].nValue = nValue;
		}
	}
	void ShowTag() {
		//std::cout << "========ShowTag========" << endl;
		MyMapIte iter=m_mapTags.begin();
		for (; iter != m_mapTags.end(); iter++) {
			//std::cout << iter->first << "\t" << iter->second.sname << "\t"<< iter->second.nValue<< endl;
		}

	}
	void AddTag(T_TAGITEMTEST* ptag)
	{
		if (m_map.find(ptag->sname) != m_map.end())
		{
			//cout << "没有找到！" << endl;
		}
		else {
			//T_TAGITEMTEST ta;
			//strcpy(ta.sname, sname);
			//strcpy(ta.sdes, sdes);
			//strcpy(ta.sunit, sunit);
			//ta.unwriteable = unwriteable;
			//ta.uctype = uctype;
			//strcpy(ta.sres, sres);
			m_map.insert(make_pair(ptag->sname, 1));
		}
	}
	bool FindTag(PTAGITEMTEST pTag) {
		return	m_map.find(pTag->sname) != m_map.end();
	}
	CSimuTags()
	{
		m_nTestTagCount = 0;
	};
	virtual ~CSimuTags(){
	}
protected:
	T_TAGITEMTEST	m_tags[TEST_TAGS];	// 简单的标签结构数组
	T_TAGITEMTEST	m_tagsTest[TEST_TAGS];	// 简单的标签结构数组
	
	TOM::CCriticalSection	m_lock;		// 临界区锁
public:

	static bool __stdcall CB_SetTagVal(const char* sTagName,FILETIME *ptime,WORD wqa,VARIANT* pv,void* pCallBackArg) //写标签回调函数
	{
		if(!pCallBackArg)
			return false;
		CSimuTags* pcls = (CSimuTags*)pCallBackArg;
		return pcls->OnWriteTagVal(sTagName,ptime,wqa,pv);
	}

	bool OnWriteTagVal(const char* sTagName,FILETIME *ptime,WORD wqa,VARIANT* pv) // 写标签值,这里只是模拟，直接写入到OPCServer中，实际应该写入现场设备
	{
		// 如果不提供写，直接返回false即可
		TOM::CSafeLock lock(&m_lock); //加临界区锁
		int i;
		for(i=2190;i<2200;i++) // 10个可写的标签
		{
			if(strcmp(m_tags[i].sname,sTagName))
				continue;
			else // 找到
			{
				VARIANT val;
				VariantInit(&val); // 初始化
				HRESULT hr = VariantChangeType( &val,pv,0,VT_R8); // 转换为double
				if(!SUCCEEDED(hr))
					return false;
				int nret = DA2_SetData_F64(sTagName,val.dblVal,wqa, ptime);
				VariantClear(&val); // 清除
				if(nret != OE_OK)
					return false;
				return true;
			}
		}
		return false;
	}

public:
	int m_nTestTagCount;
	void InitTagsFromConfig()
	{
		
	}
	void	InitTags() // 初始化标签，并将标签提交给OPC Server
	{
		//IOPCItemProperties sk;
		int i=0;
		m_nTestTagCount = 0;
		for(i=0;i<TEST_TAGS;i++){
			//sprintf(m_tags[i].sdes,"DES_T%04d",i);
			//sprintf(m_tags[i].sunit,"U_T%04d",i);
			memset(m_tags[i].sdes, 0,32 );
			memset(m_tags[i].sunit, 0,16 );
		}
		i = 0;
		for(;i<400;i++)	{	// 400个状态量
			sprintf(m_tags[i].sname,"T%04d_KVAL",i);
			m_tags[i].uctype = ODT_I8;
			m_tags[i].unwriteable = OTEM_NOWRITE; //不可写，只读
		}
		for(;i<800;i++)	{	// 400个32位整数
			sprintf(m_tags[i].sname,"T%04d_I32",i);
			m_tags[i].uctype = ODT_I32;
			m_tags[i].unwriteable = OTEM_NOWRITE; //不可写，只读
		}
		for(;i<1200;i++){	// 400个64位整数
			sprintf(m_tags[i].sname,"T%04d_I64",i);
			m_tags[i].uctype = ODT_I64;
			m_tags[i].unwriteable = OTEM_NOWRITE; //不可写，只读
		}
		for(;i<1600;i++){	// 400个float
			sprintf(m_tags[i].sname,"T%04d_F32",i);
			m_tags[i].uctype = ODT_F32;
			m_tags[i].unwriteable = OTEM_NOWRITE; //不可写，只读
		}
		for(;i<2000;i++){	// 400个double
			sprintf(m_tags[i].sname,"T%04d_F64",i);
			m_tags[i].uctype = ODT_F64;
			m_tags[i].unwriteable = OTEM_NOWRITE; //不可写，只读
		}
		for(;i<2100;i++){	// 100个string
			sprintf(m_tags[i].sname,"T%04d_STR",i);
			m_tags[i].uctype = ODT_STR;
			m_tags[i].unwriteable = OTEM_NOWRITE; //不可写，只读
		}
		for(;i<2190;i++){	// 90个blob
			sprintf(m_tags[i].sname,"T%04d_BLOB",i);
			m_tags[i].uctype = ODT_BLOB;
			m_tags[i].unwriteable = OTEM_NOWRITE; //不可写，只读
		}
		for(;i<2200;i++){	// 10个可写的double类型用于客户端写测试
			sprintf(m_tags[i].sname,"W%04d_F64",i);
			m_tags[i].uctype = ODT_F64;
			m_tags[i].unwriteable = OTEM_WRITE; //可写
		}
		for(i=0;i<TEST_TAGS;i++)// 初始化到OPC Server中
			DA2_AddTag(m_tags[i].sname,m_tags[i].sdes,m_tags[i].sunit,m_tags[i].uctype,m_tags[i].unwriteable);
		//char cName[48] = {0};
		//for (int k = 0; k < 10; k++)
		//{
		//	memset(cName, 0, 48);
		//	sprintf(cName, "sname_%04d_STR", k);
		//	AddTag(cName,11);	
		//}
		//ShowTag();
		//std::cout << "=====================2================" << endl;
		//for (int k = 0; k < 10; k++)
		//{
		//	memset(cName, 0, 48);
		//	sprintf(cName, "sname_%04d_STR", k);
		//	AddTag(cName,33);
		//}
		//ShowTag();
	}
	
	void	SimuData() // 模拟数据
	{
		TOM::CSafeLock lock(&m_lock); //加临界区锁
		int i=0;
		time_t lt = ::time(NULL); // 时间因子，用于模拟数据
		int nk = ((lt % 60) > 30 )? 1 : 0;
		int nkval = (int)(lt % 60);
		SYSTEMTIME syst;
		char sval[80];
		::GetSystemTime(&syst);
		FILETIME ftime;
		::SystemTimeToFileTime(&syst,&ftime);
		for(;i<400;i++)// 400个状态量
			DA2_SetData_I32(m_tags[i].sname,nk,OPC_QUALITY_GOOD, &ftime);
		for(;i<800;i++)// 400个32位整数
			DA2_SetData_I32(m_tags[i].sname,i + nkval,OPC_QUALITY_GOOD, &ftime);
		for(;i<1200;i++)// 400个64位整数
			DA2_SetData_I64(m_tags[i].sname,(__int64)(i + nkval),OPC_QUALITY_GOOD, &ftime);
		for(;i<1600;i++)// 400个float
			DA2_SetData_F32(m_tags[i].sname,(float)(i + nkval/10.0),OPC_QUALITY_GOOD, &ftime);
		for(;i<2000;i++)// 400个double
			DA2_SetData_F64(m_tags[i].sname,(double)(i + nkval/10.0),OPC_QUALITY_GOOD, &ftime);
		for(;i<2100;i++){// 100个string
			sprintf(sval,"T%04d_STR:%04d-%02d-%02d %02d:%02d:%02d",i,syst.wYear,syst.wMonth,syst.wDay,syst.wHour,syst.wMinute,syst.wSecond);
			DA2_SetData_Str(m_tags[i].sname,sval,OPC_QUALITY_GOOD, &ftime);
		}
		for(;i<2190;i++){// 90个blob
			sprintf(sval,"T%04d_BLOB:%04d-%02d-%02d %02d:%02d:%02d",i,syst.wYear,syst.wMonth,syst.wDay,syst.wHour,syst.wMinute,syst.wSecond);
			DA2_SetData_Blob(m_tags[i].sname,(unsigned char*)sval,strlen(sval),OPC_QUALITY_GOOD, &ftime);
		}
		char cDebug[255] = {0};
		sprintf(cDebug, ".................%d..................", m_nTestTagCount);
//		OutputDebugString(cDebug);
		if (m_nTestTagCount< TEST_TAGS)
		{
			sprintf(m_tagsTest[m_nTestTagCount].sname, "ZYT%04d_STR", m_nTestTagCount);
		//	OutputDebugString(m_tagsTest[m_nTestTagCount].sname);
			m_tagsTest[m_nTestTagCount].uctype = ODT_STR;
			m_tagsTest[m_nTestTagCount].unwriteable = OTEM_NOWRITE; //不可写，只读
			DA2_AddTag(m_tagsTest[m_nTestTagCount].sname, m_tagsTest[m_nTestTagCount].sdes, m_tagsTest[m_nTestTagCount].sunit, m_tagsTest[m_nTestTagCount].uctype, m_tagsTest[m_nTestTagCount].unwriteable);
			m_nTestTagCount++;
		}
		for (i=0; i < m_nTestTagCount; i++) {// 100个string
			sprintf(sval, "ZYT%s_STR:%04d-%02d-%02d %02d:%02d:%02d", m_tagsTest[i].sname, syst.wYear, syst.wMonth, syst.wDay, syst.wHour, syst.wMinute, syst.wSecond);
			DA2_SetData_Str(m_tagsTest[i].sname, sval, OPC_QUALITY_GOOD, &ftime);
		//	OutputDebugString(m_tagsTest[i].sname);
		//	OutputDebugString(sval);
		}
		
		// 剩下的10个F64不模拟，由客户端写入
	}

};

#endif // _SIMU_TAG_H_
