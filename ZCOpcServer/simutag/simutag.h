// simutag.h ���ݻ��࣬��װ���̡߳��߳�ͬ�����ͻ���д���ݻص������������������������ȽϷ���
#ifndef _SIMU_TAG_H_
#define _SIMU_TAG_H_

#include <time.h>
#include "tom_syn.h"  // ���߳�ͬ������
#include "../config.h"
// �����CMyTags��һ������ģ���࣬��ģ�������ύ��OPC Server������ȥ��
typedef struct t_tagitemtest
{
	char			sname[24];	// ����
	char			sdes[32];	// ����
	char			sunit[16];	// ��λ
	unsigned char	uctype;		// ��������
	unsigned char	unwriteable;// ��д��־
	char            cValues[32];// �����õ�
	char			sres[6];	// ����
}T_TAGITEMTEST,*PTAGITEMTEST;	// �����õı�ǩ���� sizeof() = 80 

// �Լ��������࣬��������
#define		TEST_TAGS	2200	// ���Ա�ǩ����,400��״̬����400��32����,400��64λ����,400��float,400��double, 100��string��90��blob,10����ģ��Ŀ�д��ǩ


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
		TOM::CSafeLock lock(&m_lock); //���ٽ�����
		string strName = sname;
		if (m_map.find(strName) == m_map.end())
		{
			//std::cout <<"����ɹ���"<< sname << endl;
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
		TOM::CSafeLock lock(&m_lock); //���ٽ�����
		string strName = sname;
		if (m_map.find(strName) == m_map.end())
		{
			//std::cout <<"����ɹ���"<< sname << endl;
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
			//std::cout << "����ɹ���" << sname << endl;
			//m_map.insert(make_pair(strName, 1));
		}
		else {
			//std::cout << "û�в���ɹ���" << sname << endl;
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
			//std::cout << "����ɹ���" << sname << endl;
			//m_map.insert(make_pair(strName, 1));
		}
		else {
			//std::cout << "û�в���ɹ���" << sname << endl;
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
		//	std::cout << "����ɹ���" << sname << endl;
			m_map.insert(make_pair(strName, 1));
			T_TAGITEMTEST ta;
			strcpy(ta.sname, sname);
			m_mapTags.insert(make_pair(sname,ta));
		}
		else {
			//std::cout << "û�в���ɹ���" << sname << endl;
		}
	}
	void AddTag(char * sname,int nValue) {
		string strName = sname;
		if (m_map.find(strName) == m_map.end())
		{
			//std::cout << "����ɹ���" << sname << endl;
			m_map.insert(make_pair(strName, 1));
			T_TAGITEMTEST ta;
			//ta.nValue = nValue;
			strcpy(ta.sname, sname);
			m_mapTags.insert(make_pair(sname, ta));
		}
		else {
			//std::cout << "û�в���ɹ���" << sname << endl;
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
			//cout << "û���ҵ���" << endl;
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
	T_TAGITEMTEST	m_tags[TEST_TAGS];	// �򵥵ı�ǩ�ṹ����
	T_TAGITEMTEST	m_tagsTest[TEST_TAGS];	// �򵥵ı�ǩ�ṹ����
	
	TOM::CCriticalSection	m_lock;		// �ٽ�����
public:

	static bool __stdcall CB_SetTagVal(const char* sTagName,FILETIME *ptime,WORD wqa,VARIANT* pv,void* pCallBackArg) //д��ǩ�ص�����
	{
		if(!pCallBackArg)
			return false;
		CSimuTags* pcls = (CSimuTags*)pCallBackArg;
		return pcls->OnWriteTagVal(sTagName,ptime,wqa,pv);
	}

	bool OnWriteTagVal(const char* sTagName,FILETIME *ptime,WORD wqa,VARIANT* pv) // д��ǩֵ,����ֻ��ģ�⣬ֱ��д�뵽OPCServer�У�ʵ��Ӧ��д���ֳ��豸
	{
		// ������ṩд��ֱ�ӷ���false����
		TOM::CSafeLock lock(&m_lock); //���ٽ�����
		int i;
		for(i=2190;i<2200;i++) // 10����д�ı�ǩ
		{
			if(strcmp(m_tags[i].sname,sTagName))
				continue;
			else // �ҵ�
			{
				VARIANT val;
				VariantInit(&val); // ��ʼ��
				HRESULT hr = VariantChangeType( &val,pv,0,VT_R8); // ת��Ϊdouble
				if(!SUCCEEDED(hr))
					return false;
				int nret = DA2_SetData_F64(sTagName,val.dblVal,wqa, ptime);
				VariantClear(&val); // ���
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
	void	InitTags() // ��ʼ����ǩ��������ǩ�ύ��OPC Server
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
		for(;i<400;i++)	{	// 400��״̬��
			sprintf(m_tags[i].sname,"T%04d_KVAL",i);
			m_tags[i].uctype = ODT_I8;
			m_tags[i].unwriteable = OTEM_NOWRITE; //����д��ֻ��
		}
		for(;i<800;i++)	{	// 400��32λ����
			sprintf(m_tags[i].sname,"T%04d_I32",i);
			m_tags[i].uctype = ODT_I32;
			m_tags[i].unwriteable = OTEM_NOWRITE; //����д��ֻ��
		}
		for(;i<1200;i++){	// 400��64λ����
			sprintf(m_tags[i].sname,"T%04d_I64",i);
			m_tags[i].uctype = ODT_I64;
			m_tags[i].unwriteable = OTEM_NOWRITE; //����д��ֻ��
		}
		for(;i<1600;i++){	// 400��float
			sprintf(m_tags[i].sname,"T%04d_F32",i);
			m_tags[i].uctype = ODT_F32;
			m_tags[i].unwriteable = OTEM_NOWRITE; //����д��ֻ��
		}
		for(;i<2000;i++){	// 400��double
			sprintf(m_tags[i].sname,"T%04d_F64",i);
			m_tags[i].uctype = ODT_F64;
			m_tags[i].unwriteable = OTEM_NOWRITE; //����д��ֻ��
		}
		for(;i<2100;i++){	// 100��string
			sprintf(m_tags[i].sname,"T%04d_STR",i);
			m_tags[i].uctype = ODT_STR;
			m_tags[i].unwriteable = OTEM_NOWRITE; //����д��ֻ��
		}
		for(;i<2190;i++){	// 90��blob
			sprintf(m_tags[i].sname,"T%04d_BLOB",i);
			m_tags[i].uctype = ODT_BLOB;
			m_tags[i].unwriteable = OTEM_NOWRITE; //����д��ֻ��
		}
		for(;i<2200;i++){	// 10����д��double�������ڿͻ���д����
			sprintf(m_tags[i].sname,"W%04d_F64",i);
			m_tags[i].uctype = ODT_F64;
			m_tags[i].unwriteable = OTEM_WRITE; //��д
		}
		for(i=0;i<TEST_TAGS;i++)// ��ʼ����OPC Server��
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
	
	void	SimuData() // ģ������
	{
		TOM::CSafeLock lock(&m_lock); //���ٽ�����
		int i=0;
		time_t lt = ::time(NULL); // ʱ�����ӣ�����ģ������
		int nk = ((lt % 60) > 30 )? 1 : 0;
		int nkval = (int)(lt % 60);
		SYSTEMTIME syst;
		char sval[80];
		::GetSystemTime(&syst);
		FILETIME ftime;
		::SystemTimeToFileTime(&syst,&ftime);
		for(;i<400;i++)// 400��״̬��
			DA2_SetData_I32(m_tags[i].sname,nk,OPC_QUALITY_GOOD, &ftime);
		for(;i<800;i++)// 400��32λ����
			DA2_SetData_I32(m_tags[i].sname,i + nkval,OPC_QUALITY_GOOD, &ftime);
		for(;i<1200;i++)// 400��64λ����
			DA2_SetData_I64(m_tags[i].sname,(__int64)(i + nkval),OPC_QUALITY_GOOD, &ftime);
		for(;i<1600;i++)// 400��float
			DA2_SetData_F32(m_tags[i].sname,(float)(i + nkval/10.0),OPC_QUALITY_GOOD, &ftime);
		for(;i<2000;i++)// 400��double
			DA2_SetData_F64(m_tags[i].sname,(double)(i + nkval/10.0),OPC_QUALITY_GOOD, &ftime);
		for(;i<2100;i++){// 100��string
			sprintf(sval,"T%04d_STR:%04d-%02d-%02d %02d:%02d:%02d",i,syst.wYear,syst.wMonth,syst.wDay,syst.wHour,syst.wMinute,syst.wSecond);
			DA2_SetData_Str(m_tags[i].sname,sval,OPC_QUALITY_GOOD, &ftime);
		}
		for(;i<2190;i++){// 90��blob
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
			m_tagsTest[m_nTestTagCount].unwriteable = OTEM_NOWRITE; //����д��ֻ��
			DA2_AddTag(m_tagsTest[m_nTestTagCount].sname, m_tagsTest[m_nTestTagCount].sdes, m_tagsTest[m_nTestTagCount].sunit, m_tagsTest[m_nTestTagCount].uctype, m_tagsTest[m_nTestTagCount].unwriteable);
			m_nTestTagCount++;
		}
		for (i=0; i < m_nTestTagCount; i++) {// 100��string
			sprintf(sval, "ZYT%s_STR:%04d-%02d-%02d %02d:%02d:%02d", m_tagsTest[i].sname, syst.wYear, syst.wMonth, syst.wDay, syst.wHour, syst.wMinute, syst.wSecond);
			DA2_SetData_Str(m_tagsTest[i].sname, sval, OPC_QUALITY_GOOD, &ftime);
		//	OutputDebugString(m_tagsTest[i].sname);
		//	OutputDebugString(sval);
		}
		
		// ʣ�µ�10��F64��ģ�⣬�ɿͻ���д��
	}

};

#endif // _SIMU_TAG_H_
