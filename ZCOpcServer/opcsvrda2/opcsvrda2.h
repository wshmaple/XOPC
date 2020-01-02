// opcsvrda2.h 
// 这是OPC Server SDK开发包的定义文件，静态调用版.
// copy right by 重庆唐码软件有限公司 2009-2014
//
#ifndef _OPCSVRDA2_H_
#define _OPCSVRDA2_H_

#ifndef WINVER                  // 指定要求的最低平台是 Windows xp
#define WINVER 0x0501           // 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif

#ifndef _WIN32_WINNT            // 指定要求的最低平台是 Windows xp
#define _WIN32_WINNT 0x0501     // 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif

#define WIN32_LEAN_AND_MEAN     // 从 Windows 头中排除极少使用的资料

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
		#pragma message("请使用多线程库")
	#endif	
#else
	#pragma message("请使用高于或等于VC9的编译器")
#endif


// 数据类型
#define	ODT_VOID		0	// 空类型

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
	
#define ODT_STR			11  // char* 或 BSTR
#define ODT_BLOB		12	// 二进制字节流 (VT_UI1 | VT_ARRAY)

// OPC客户端容许读写标志
#define OTEM_NOWRITE	0	// 不可写
#define OTEM_WRITE		1	// 可写

// 错误码
#define OE_OK			0	// 成功
#define OE_ERR			1	// 失败
#define OE_ERRARG		2	// 错误的参数
#define OE_OUTMEM		3	// 内存不够
#define OE_CLSID		4	// CLSID错误
#define OE_ERRCOM		5	// COM/DCOM错误

#define OE_NOITEM		10	// 无此标签
#define OE_ERRTYPE		11	// 数据类型错误或无法转换数据
#define OE_ERRDATA		12	// 错误的数据
#define OE_ERRTIME		13	// 错误的时标
#define OE_ERRQA		14	// 错误的数据质量
#define OE_NODATA		15	// 无数据

//{{ OPC 数据质量,从OPC标准复制而来
const WORD OPC_QUALITY_MASK						=	0xc0;
const WORD OPC_STATUS_MASK						=	0xfc;
const WORD OPC_LIMIT_MASK						=	0x3;
const WORD OPC_QUALITY_BAD						=	0;		// 坏质量，常用
const WORD OPC_QUALITY_UNCERTAIN				=	0x40;
const WORD OPC_QUALITY_GOOD						=	0xc0;	// 好质量，常用
const WORD OPC_QUALITY_CONFIG_ERROR				=	0x4;
const WORD OPC_QUALITY_NOT_CONNECTED			=	0x8;
const WORD OPC_QUALITY_DEVICE_FAILURE			=	0xc;	// 设备错误,坏质量细分
const WORD OPC_QUALITY_SENSOR_FAILURE			=	0x10;	// 传感器错误,坏质量细分
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

// OPC 客户端写标签值的回调函数，如果不允许写，可以不实现这个回调函数
typedef bool (WINAPI *cb_SetItemVal)(const char* sTagName,FILETIME *ptime,WORD wqa,VARIANT* pv,void* pCallBackArg); // 写标签值回调函数

extern "C" int __stdcall DA2_Reg( // 直接可以调用的注册函数
			const char* sClsID,			// CLSID 比如"{DEAFC4D7-1B56-4eff-B7DC-D06999F47ADB}"
			const char* sVendor,		// 提供商,比如"tom"
			const char* sDrivername,	// 驱动名,比如"rdb4opcda"
			const char* sVersion,		// 版本名,比如"1" 
			const char*	sOpcServerDes,	// OPC Server描述 ，<128字符
			bool	bService
			);

extern "C" int __stdcall DA2_UnReg( // 直接可以卸载的函数
			const char* sClsID,			// CLSID 比如"{DEAFC4D7-1B56-4eff-B7DC-D06999F47ADB}"
			const char* sVendor,		// 提供商,比如"tom"
			const char* sDrivername,	// 驱动名,比如"rdb4opcda"
			const char* sVersion,		// 版本名,比如"1" 
			const char*	sOpcServerDes	// OPC Server描述 ，<128字符			
			);

extern "C" int	__stdcall DA2_Initialize(
			const char* sClsID,			// CLSID 比如"{DEAFC4D7-1B56-4eff-B7DC-D06999F47ADB}"
			const char* sVendor,		// 提供商,比如"tom"
			const char* sDrivername,	// 驱动名,比如"rdb4opcda"
			const char* sVersion,		// 版本名,比如"1" 
			const char*	sOpcServerDes,	// OPC Server描述 ，<128字符
			cb_SetItemVal pfcbSetTagVal,// 回调函数,NULL表示不响应客户端写操作
			void*	pcbSetTagValArg		// 回调参数，pfcbSetTagVal使用的参数
		); // 初始化

 extern "C" int	__stdcall DA2_UnInitialize();
 extern "C" int	__stdcall DA2_Start(bool bLockServer);
 extern "C" int	__stdcall DA2_Stop();

 extern "C" int	__stdcall DA2_AddTag(
			const char* sName,		// 标签名  <80字符
			const char* sDes,		// 标签表述 < 80字符
			const char*	sUnit,		// 标签单位 < 16字符
			unsigned char ucType,	// 数据类型 ODT_I32等
			unsigned char ucWriteable // 可写标志 OTEM_NOWRITE 或者OTEM_WRITE
		); // 添加标签

// 下面是数据提交，为了方便使用，每种数据类型给出一个提交方法
 extern "C" int	__stdcall DA2_SetData_I32(
		const char* sTagname,		// 标签名
		int val,					// 值
		unsigned short wqa,			// 质量
		FILETIME* pftime			// 时标，NULL表示使用本机系统时间
		); // 提交标签数据

extern "C"  int	__stdcall DA2_SetData_U32(
			const char* sTagname,	// 标签名
			unsigned int val,		// 值
			unsigned short wqa,		// 质量
			FILETIME* pftime		// 时标，NULL表示使用本机系统时间
		); // 提交标签数据

 extern "C" int	__stdcall DA2_SetData_I64(
			const char* sTagname,	// 标签名
			__int64 val,			// 值
			unsigned short wqa,		// 质量
			FILETIME* pftime		// 时标，NULL表示使用本机系统时间
		); // 提交标签数据

 extern "C" int	__stdcall DA2_SetData_U64(
			const char* sTagname,	// 标签名
			unsigned __int64 val,	// 值
			unsigned short wqa,		// 质量
			FILETIME* pftime		// 时标，NULL表示使用本机系统时间
		); // 提交标签数据

 extern "C" int	__stdcall DA2_SetData_F32(
			const char* sTagname,	// 标签名
			float val,				// 值
			unsigned short wqa,		// 质量
			FILETIME* pftime		// 时标，NULL表示使用本机系统时间
		); // 提交标签数据

 extern "C" int	__stdcall DA2_SetData_F64(
			const char* sTagname,	// 标签名
			double val,				// 值
			unsigned short wqa,		// 质量
			FILETIME* pftime		// 时标，NULL表示使用本机系统时间
		); // 提交标签数据

 extern "C" int	__stdcall DA2_SetData_Str(
			const char* sTagname,	// 标签名
			const char* val,				// 值
			unsigned short wqa,		// 质量
			FILETIME* pftime		// 时标，NULL表示使用本机系统时间
		); // 提交标签数据

 extern "C" int	__stdcall DA2_SetData_Blob(
			const char* sTagname,	// 标签名
			unsigned char* val,		// 值
			int		nsize,			// 字节数
			unsigned short wqa,		// 质量
			FILETIME* pftime		// 时标，NULL表示使用本机系统时间
		); // 提交标签数据

 #include <process.h>
// opc server的简易包装，让它运行在一个线程内，使用COM多线程套间，避免和MFC的单线程套间AfxOleInit冲突
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
	LONG			_lThreadRun;			// 是否运行
	LONG			_lKillTread;			// 终止线程
	HANDLE			_hEvtOk;				// 初始化成功事件

	bool			_bLockServer;
	const char*		_sClsID;
	const char*		_sVendor;
	const char*		_sDrivername;
	const char*		_sVersion;
	const char*		_sOpcServerDes;
	cb_SetItemVal	_pfcbSetTagVal;// 回调函数,NULL表示不响应客户端写操作
	void*			_pcbSetTagValArg;		// 回调参数，pfcbSetTagVal使用的参数

	bool            _bIsRuning;// 是否正在运行
	static void ThreadProcess(void* pargs)
	{
		COpcSvrThread* pcls = (COpcSvrThread*)pargs;
		pcls->mainloop();
	}

 public:	
	 void mainloop() // 线程主循环，被ThreadProcess调用
	 {
		if( OE_OK != DA2_Initialize(_sClsID,_sVendor,_sDrivername,_sVersion,_sOpcServerDes,_pfcbSetTagVal,&_pcbSetTagValArg) || 		
			OE_OK != DA2_Start(_bLockServer))	
			return;
		::SetEvent(_hEvtOk);
	    HANDLE  hObject = ::CreateEvent(NULL, true, false, NULL);
		InterlockedExchange(&_lKillTread,0); //设置为0
		InterlockedExchange(&_lThreadRun,1); //设置为0
		while(!InterlockedExchangeAdd(&_lKillTread,0))
			WaitForSingleObject(hObject,100); // 当延时Sleep用
		InterlockedExchange(&_lThreadRun,0); //设置为0
		InterlockedExchange(&_lKillTread,0);
		
		DA2_Stop();
		DA2_UnInitialize(); // 卸载
		::CloseHandle(hObject);
	 }
 public:
	 bool	Start(const char* sClsID,			// CLSID 比如"{DEAFC4D7-1B56-4eff-B7DC-D06999F47ADB}"
			const char* sVendor,		// 提供商,比如"tom"
			const char* sDrivername,	// 驱动名,比如"rdb4opcda"
			const char* sVersion,		// 版本名,比如"1" 
			const char*	sOpcServerDes,	// OPC Server描述 ，<128字符
			cb_SetItemVal pfcbSetTagVal,// 回调函数,NULL表示不响应客户端写操作
			void*	pcbSetTagValArg,
			bool	bLockServer
			)		// 回调参数，pfcbSetTagVal使用的参数
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
