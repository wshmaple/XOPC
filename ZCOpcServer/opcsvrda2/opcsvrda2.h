// opcsvrda2.h 
// ����OPC Server SDK�������Ķ����ļ�����̬���ð�.
// copy right by ��������������޹�˾ 2009-2014
//
#ifndef _OPCSVRDA2_H_
#define _OPCSVRDA2_H_

#ifndef WINVER                  // ָ��Ҫ������ƽ̨�� Windows xp
#define WINVER 0x0501           // ����ֵ����Ϊ��Ӧ��ֵ���������� Windows �������汾��
#endif

#ifndef _WIN32_WINNT            // ָ��Ҫ������ƽ̨�� Windows xp
#define _WIN32_WINNT 0x0501     // ����ֵ����Ϊ��Ӧ��ֵ���������� Windows �������汾��
#endif

#define WIN32_LEAN_AND_MEAN     // �� Windows ͷ���ų�����ʹ�õ�����

#pragma warning (disable : 4996)

#include <windows.h>

#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <comutil.h>

#if _MSC_VER >= 1500  // vc9(vc2008)
	#ifdef _MT 
		#ifdef _DEBUG
			#ifdef _DLL
				#pragma comment(lib, "opcsvrda2/opcsvrda2_mdd.lib")
			#else
				#pragma comment(lib, "opcsvrda2/opcsvrda2_mtd.lib")
			#endif
		#else
			#ifdef _DLL
				#pragma comment(lib, "opcsvrda2/opcsvrda2_md.lib")
			#else
				#pragma comment(lib, "opcsvrda2/opcsvrda2_mt.lib")
			#endif			
		#endif
	#else
		#pragma message("��ʹ�ö��߳̿�")
	#endif	
#else
	#pragma message("��ʹ�ø��ڻ����VC9�ı�����")
#endif


// ��������
#define	ODT_VOID		0	// ������

#define	ODT_I8			1	// char
#define	ODT_U8			2	// unsigned char

#define	ODT_I16			3	// short
#define	ODT_U16			4	// unsigned short

#define	ODT_I32			5	// int
#define	ODT_U32			6	// unsigned int

#define	ODT_I64			7	// __int64
#define	ODT_U64			8	// unsigned __int64

#define	ODT_F32			9	// float
#define	ODT_F64			10	// double
	
#define ODT_STR			11  // char* �� BSTR
#define ODT_BLOB		12	// �������ֽ��� (VT_UI1 | VT_ARRAY)

// OPC�ͻ��������д��־
#define OTEM_NOWRITE	0	// ����д
#define OTEM_WRITE		1	// ��д

// ������
#define OE_OK			0	// �ɹ�
#define OE_ERR			1	// ʧ��
#define OE_ERRARG		2	// ����Ĳ���
#define OE_OUTMEM		3	// �ڴ治��
#define OE_CLSID		4	// CLSID����
#define OE_ERRCOM		5	// COM/DCOM����

#define OE_NOITEM		10	// �޴˱�ǩ
#define OE_ERRTYPE		11	// �������ʹ�����޷�ת������
#define OE_ERRDATA		12	// ���������
#define OE_ERRTIME		13	// �����ʱ��
#define OE_ERRQA		14	// �������������
#define OE_NODATA		15	// ������

//{{ OPC ��������,��OPC��׼���ƶ���
const WORD OPC_QUALITY_MASK						=	0xc0;
const WORD OPC_STATUS_MASK						=	0xfc;
const WORD OPC_LIMIT_MASK						=	0x3;
const WORD OPC_QUALITY_BAD						=	0;		// ������������
const WORD OPC_QUALITY_UNCERTAIN				=	0x40;
const WORD OPC_QUALITY_GOOD						=	0xc0;	// ������������
const WORD OPC_QUALITY_CONFIG_ERROR				=	0x4;
const WORD OPC_QUALITY_NOT_CONNECTED			=	0x8;
const WORD OPC_QUALITY_DEVICE_FAILURE			=	0xc;	// �豸����,������ϸ��
const WORD OPC_QUALITY_SENSOR_FAILURE			=	0x10;	// ����������,������ϸ��
const WORD OPC_QUALITY_LAST_KNOWN				=	0x14;
const WORD OPC_QUALITY_COMM_FAILURE				=	0x18;
const WORD OPC_QUALITY_OUT_OF_SERVICE			=	0x1c;
const WORD OPC_QUALITY_WAITING_FOR_INITIAL_DATA	=	0x20;
const WORD OPC_QUALITY_LAST_USABLE				=	0x44;
const WORD OPC_QUALITY_SENSOR_CAL				=	0x50;
const WORD OPC_QUALITY_EGU_EXCEEDED				=	0x54;
const WORD OPC_QUALITY_SUB_NORMAL				=	0x58;
const WORD OPC_QUALITY_LOCAL_OVERRIDE			=	0xd8;
const WORD OPC_LIMIT_OK							=	0;
const WORD OPC_LIMIT_LOW						=	0x1;
const WORD OPC_LIMIT_HIGH						=	0x2;
const WORD OPC_LIMIT_CONST						=	0x3;
//}}

// OPC �ͻ���д��ǩֵ�Ļص����������������д�����Բ�ʵ������ص�����
typedef bool (WINAPI *cb_SetItemVal)(const char* sTagName,FILETIME *ptime,WORD wqa,VARIANT* pv,void* pCallBackArg); // д��ǩֵ�ص�����

extern "C" int __stdcall DA2_Reg( // ֱ�ӿ��Ե��õ�ע�ắ��
			const char* sClsID,			// CLSID ����"{DEAFC4D7-1B56-4eff-B7DC-D06999F47ADB}"
			const char* sVendor,		// �ṩ��,����"tom"
			const char* sDrivername,	// ������,����"rdb4opcda"
			const char* sVersion,		// �汾��,����"1" 
			const char*	sOpcServerDes,	// OPC Server���� ��<128�ַ�
			bool	bService
			);

extern "C" int __stdcall DA2_UnReg( // ֱ�ӿ���ж�صĺ���
			const char* sClsID,			// CLSID ����"{DEAFC4D7-1B56-4eff-B7DC-D06999F47ADB}"
			const char* sVendor,		// �ṩ��,����"tom"
			const char* sDrivername,	// ������,����"rdb4opcda"
			const char* sVersion,		// �汾��,����"1" 
			const char*	sOpcServerDes	// OPC Server���� ��<128�ַ�			
			);

extern "C" int	__stdcall DA2_Initialize(
			const char* sClsID,			// CLSID ����"{DEAFC4D7-1B56-4eff-B7DC-D06999F47ADB}"
			const char* sVendor,		// �ṩ��,����"tom"
			const char* sDrivername,	// ������,����"rdb4opcda"
			const char* sVersion,		// �汾��,����"1" 
			const char*	sOpcServerDes,	// OPC Server���� ��<128�ַ�
			cb_SetItemVal pfcbSetTagVal,// �ص�����,NULL��ʾ����Ӧ�ͻ���д����
			void*	pcbSetTagValArg		// �ص�������pfcbSetTagValʹ�õĲ���
		); // ��ʼ��

 extern "C" int	__stdcall DA2_UnInitialize();
 extern "C" int	__stdcall DA2_Start(bool bLockServer);
 extern "C" int	__stdcall DA2_Stop();

 extern "C" int	__stdcall DA2_AddTag(
			const char* sName,		// ��ǩ��  <80�ַ�
			const char* sDes,		// ��ǩ���� < 80�ַ�
			const char*	sUnit,		// ��ǩ��λ < 16�ַ�
			unsigned char ucType,	// �������� ODT_I32��
			unsigned char ucWriteable // ��д��־ OTEM_NOWRITE ����OTEM_WRITE
		); // ��ӱ�ǩ

// �����������ύ��Ϊ�˷���ʹ�ã�ÿ���������͸���һ���ύ����
 extern "C" int	__stdcall DA2_SetData_I32(
		const char* sTagname,		// ��ǩ��
		int val,					// ֵ
		unsigned short wqa,			// ����
		FILETIME* pftime			// ʱ�꣬NULL��ʾʹ�ñ���ϵͳʱ��
		); // �ύ��ǩ����

extern "C"  int	__stdcall DA2_SetData_U32(
			const char* sTagname,	// ��ǩ��
			unsigned int val,		// ֵ
			unsigned short wqa,		// ����
			FILETIME* pftime		// ʱ�꣬NULL��ʾʹ�ñ���ϵͳʱ��
		); // �ύ��ǩ����

 extern "C" int	__stdcall DA2_SetData_I64(
			const char* sTagname,	// ��ǩ��
			__int64 val,			// ֵ
			unsigned short wqa,		// ����
			FILETIME* pftime		// ʱ�꣬NULL��ʾʹ�ñ���ϵͳʱ��
		); // �ύ��ǩ����

 extern "C" int	__stdcall DA2_SetData_U64(
			const char* sTagname,	// ��ǩ��
			unsigned __int64 val,	// ֵ
			unsigned short wqa,		// ����
			FILETIME* pftime		// ʱ�꣬NULL��ʾʹ�ñ���ϵͳʱ��
		); // �ύ��ǩ����

 extern "C" int	__stdcall DA2_SetData_F32(
			const char* sTagname,	// ��ǩ��
			float val,				// ֵ
			unsigned short wqa,		// ����
			FILETIME* pftime		// ʱ�꣬NULL��ʾʹ�ñ���ϵͳʱ��
		); // �ύ��ǩ����

 extern "C" int	__stdcall DA2_SetData_F64(
			const char* sTagname,	// ��ǩ��
			double val,				// ֵ
			unsigned short wqa,		// ����
			FILETIME* pftime		// ʱ�꣬NULL��ʾʹ�ñ���ϵͳʱ��
		); // �ύ��ǩ����

 extern "C" int	__stdcall DA2_SetData_Str(
			const char* sTagname,	// ��ǩ��
			const char* val,				// ֵ
			unsigned short wqa,		// ����
			FILETIME* pftime		// ʱ�꣬NULL��ʾʹ�ñ���ϵͳʱ��
		); // �ύ��ǩ����

 extern "C" int	__stdcall DA2_SetData_Blob(
			const char* sTagname,	// ��ǩ��
			unsigned char* val,		// ֵ
			int		nsize,			// �ֽ���
			unsigned short wqa,		// ����
			FILETIME* pftime		// ʱ�꣬NULL��ʾʹ�ñ���ϵͳʱ��
		); // �ύ��ǩ����

 #include <process.h>
// opc server�ļ��װ�װ������������һ���߳��ڣ�ʹ��COM���߳��׼䣬�����MFC�ĵ��߳��׼�AfxOleInit��ͻ
 class COpcSvrThread
 {
 public:
	 COpcSvrThread(){
		_sClsID = NULL;
		_sVendor = NULL;
		_sDrivername = NULL;
		_sVersion = NULL;
		_sOpcServerDes = NULL;
		_pfcbSetTagVal = NULL;
		_pcbSetTagValArg = NULL;
		_bLockServer = true;
		_bIsRuning = false;

		_lThreadRun = 0;
		_lKillTread = 0;
		
		_hEvtOk = ::CreateEvent(NULL, true, false, NULL);
	 };
	 virtual ~COpcSvrThread(){
		 ::CloseHandle(_hEvtOk);
	 };
protected:
	LONG			_lThreadRun;			// �Ƿ�����
	LONG			_lKillTread;			// ��ֹ�߳�
	HANDLE			_hEvtOk;				// ��ʼ���ɹ��¼�

	bool			_bLockServer;
	const char*		_sClsID;
	const char*		_sVendor;
	const char*		_sDrivername;
	const char*		_sVersion;
	const char*		_sOpcServerDes;
	cb_SetItemVal	_pfcbSetTagVal;// �ص�����,NULL��ʾ����Ӧ�ͻ���д����
	void*			_pcbSetTagValArg;		// �ص�������pfcbSetTagValʹ�õĲ���

	bool            _bIsRuning;// �Ƿ���������
	static void ThreadProcess(void* pargs)
	{
		COpcSvrThread* pcls = (COpcSvrThread*)pargs;
		pcls->mainloop();
	}

 public:	
	 void mainloop() // �߳���ѭ������ThreadProcess����
	 {
		if( OE_OK != DA2_Initialize(_sClsID,_sVendor,_sDrivername,_sVersion,_sOpcServerDes,_pfcbSetTagVal,&_pcbSetTagValArg) || 		
			OE_OK != DA2_Start(_bLockServer))	
			return;
		::SetEvent(_hEvtOk);
	    HANDLE  hObject = ::CreateEvent(NULL, true, false, NULL);
		InterlockedExchange(&_lKillTread,0); //����Ϊ0
		InterlockedExchange(&_lThreadRun,1); //����Ϊ0
		while(!InterlockedExchangeAdd(&_lKillTread,0))
			WaitForSingleObject(hObject,100); // ����ʱSleep��
		InterlockedExchange(&_lThreadRun,0); //����Ϊ0
		InterlockedExchange(&_lKillTread,0);
		
		DA2_Stop();
		DA2_UnInitialize(); // ж��
		::CloseHandle(hObject);
	 }
 public:
	 bool	Start(const char* sClsID,			// CLSID ����"{DEAFC4D7-1B56-4eff-B7DC-D06999F47ADB}"
			const char* sVendor,		// �ṩ��,����"tom"
			const char* sDrivername,	// ������,����"rdb4opcda"
			const char* sVersion,		// �汾��,����"1" 
			const char*	sOpcServerDes,	// OPC Server���� ��<128�ַ�
			cb_SetItemVal pfcbSetTagVal,// �ص�����,NULL��ʾ����Ӧ�ͻ���д����
			void*	pcbSetTagValArg,
			bool	bLockServer
			)		// �ص�������pfcbSetTagValʹ�õĲ���
	{
		if(InterlockedExchangeAdd(&_lThreadRun,0))
			return true;

		_sClsID = sClsID;
		_sVendor = sVendor;
		_sDrivername = sDrivername;
		_sVersion = sVersion;
		_sOpcServerDes = sOpcServerDes;
		_pfcbSetTagVal = pfcbSetTagVal;
		_pcbSetTagValArg = pcbSetTagValArg;
		_bLockServer = bLockServer;

		ResetEvent(_hEvtOk);
		_beginthread(ThreadProcess,0,this);		
		_bIsRuning = true;
		if(WAIT_OBJECT_0 == WaitForSingleObject(_hEvtOk,2000))			
			return true;
		return false;
	}
	void Stop()
	{
		_bIsRuning = false;
		InterlockedExchange(&_lKillTread,1);        
		while(InterlockedExchangeAdd(&_lThreadRun,0) > 0)
			Sleep(100);
	}
	bool IsRuning() {
		return _bIsRuning;
	}
 };
#endif // _OPCSVRDA2_H_
