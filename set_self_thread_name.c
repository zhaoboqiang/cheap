#include "set_self_thread_name.h"
 
#if defined(_WIN32) && defined(_MSC_VER)
#include <Windows.h>

#ifndef THREADNAME_INFO
#pragma pack(push, 8)  
typedef struct tagTHREADNAME_INFO  
{  
    DWORD dwType;       // Must be 0x1000.  
    LPCSTR pszName;     // Pointer to name (in user addr space).  
    DWORD dwThreadID;   // Thread ID (-1=caller thread).  
    DWORD dwFlags;      // Reserved for future use, must be zero.  
 } THREADNAME_INFO;  
#pragma pack(pop)  
#endif

errno_t set_self_thread_name(char const* name)
{
	const DWORD MS_VC_EXCEPTION = 0x406D1388;  
    
	THREADNAME_INFO info;  
    info.dwType = 0x1000;  
    info.pszName = name;  
    info.dwThreadID = (DWORD)-1;
    info.dwFlags = 0;  
#pragma warning(push)  
#pragma warning(disable: 6320 6322)  
    __try {  
        RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);  
    }  
    __except (EXCEPTION_EXECUTE_HANDLER){  
    }  
#pragma warning(pop)  

    return 0;
}

#endif

#ifdef __linux__
#include <pthread.h>

errno_t set_self_thread_name(char const* name)
{
    return pthread_setname_np(pthread_self(), name);
}
#endif
