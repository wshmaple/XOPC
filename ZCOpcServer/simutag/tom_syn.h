// tom_syn.h ���߳�ͬ��������,ͬʱ֧��windows��LINUX GCC
#ifndef _TOM_SYN_H_
#define _TOM_SYN_H_

#ifndef _WIN32
#include <pthread.h>
#else
#include <process.h>
#endif

namespace TOM{	// �������߳�ͬ��������
#ifdef LINUX_GCC
	class CCriticalSection  //�ٽ���, �� MFC�÷���ͬ
	{
	public:
		CCriticalSection(){
			pthread_mutex_init(&m_mtx,NULL);
		};
		~CCriticalSection(){
			pthread_mutex_destroy(&m_mtx);
		};
	public:
		void Lock() {pthread_mutex_lock(&m_mtx);}
		void Unlock() {pthread_mutex_unlock(&m_mtx);}
		pthread_mutex_t* GetMutex(){return &m_mtx;};
	protected:
		pthread_mutex_t m_mtx;
	};

#else  // windows
	class CEvent			//�¼�,�� MFC�÷���ͬ
	{
	public:
		CEvent(bool bInitiallyOwn = FALSE, bool bManualReset = FALSE, const TCHAR* pstrName = NULL,
			LPSECURITY_ATTRIBUTES lpsaAttribute = NULL){
				m_hObject = ::CreateEvent(lpsaAttribute, bManualReset, bInitiallyOwn, pstrName);
		};

		~CEvent(){
			if (m_hObject != NULL)	{
				::CloseHandle(m_hObject);
				m_hObject = NULL;
			}
		};
		HANDLE  m_hObject;
		// Operations
	public:
		BOOL SetEvent(){
			if(m_hObject)
				return ::SetEvent(m_hObject);
			return FALSE;
		};
		BOOL PulseEvent(){
			if(m_hObject)
				return ::PulseEvent(m_hObject);
			return FALSE;
		};
		BOOL ResetEvent(){
			if(m_hObject)
				return ::ResetEvent(m_hObject);
			return FALSE;
		};
		operator HANDLE() const{
			return m_hObject;
		};
	};
	class CCriticalSection  //�ٽ���, �� MFC�÷���ͬ
	{
	public:
		CCriticalSection(){
			InitializeCriticalSection(&m_sec);
		};
		~CCriticalSection(){
			DeleteCriticalSection(&m_sec);
		};
	public:
		void Lock() {EnterCriticalSection(&m_sec);}
		void Unlock() {LeaveCriticalSection(&m_sec);}
	protected:
		CRITICAL_SECTION m_sec;
	};
#endif //define LINUX_GCC

	class CSafeLock  //��װ���Զ��ٽ���
	{
	public:
		CSafeLock(CCriticalSection *pLock){
			m_pLock = pLock;
			if(m_pLock != NULL)
				m_pLock->Lock();
		};
		virtual ~CSafeLock(){
			if(m_pLock != NULL)
				m_pLock->Unlock();
		};
	private:
		CCriticalSection *m_pLock;
	};
}; // namespace TOM

#endif // _TOM_SYN_H_