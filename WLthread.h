#ifdef __linux__ 
#include <pthread.h>
#elif __cplusplus > 199711L
#include <chrono>
#include <thread>


#else
#include <Windows.h>
#include <process.h>
#endif

///
/**
* Crossplatform Windows Linux run thread function and join
* Use pointers in ParameterObject for thread control like exit flags
*/
class WLthread
{
	void* userDefinedFunction = 0;


#ifdef __linux__ 

	pthread_t hThread;
	unsigned threadId = 0;
	void* param = 0;

	WLthread()
	{
		threadId = 0;
		param = 0;
	}

	static void* wrappedCaller(void* pArguments)
	{
		((void(*)(void*))(((WLthread*)(pArguments))->userDefinedFunction))(((WLthread*)(pArguments))->param);
		pthread_exit(NULL);
	}

public:

	WLthread(void(*thread)(void*), void* parameterObject)
	{
		userDefinedFunction = (void*)(thread);
		param = parameterObject;
		threadId = pthread_create(&hThread, NULL, wrappedCaller, (void*)parameterObject);
	}

	void join()
	{
		pthread_join(hThread, 0);
	}

	unsigned int get_id()
	{
		return threadId;
	}

	void* native_handle()
	{
		return &hThread;
	}

	WLthread(const WLthread& other)
	{
		*this = other;
	}

	void operator=(const WLthread& other)
	{
		hThread = other.hThread;
		threadId = other.threadId;
		param = other.param;
	}

#elif __cplusplus > 199711L
	std::thread _thread;
public:

	WLthread()
	{

	}

	WLthread(void(*thread)(void*), void* parameterObject)
	{
		_thread = std::thread(thread, parameterObject);
		userDefinedFunction = (void*)(thread);
		param = parameterObject;
		hThread = (HANDLE)_beginthreadex(NULL, 0, &wrappedCaller, (void*)this, 0, &threadId);
	}

	void join()
	{
		_thread.join();
	}

	std::thread::id get_id()
	{
		return _thread.get_id();
	}

	void* native_handle()
	{
		return _thread.native_handle();
	}

	WLthread(const WLthread& other)
	{
		*this = other;
	}

	void operator=(const WLthread& other)
	{
		_thread = (std::thread&&)(other._thread);
	}

#else
	HANDLE hThread = 0;
	unsigned threadId = 0;
	void* param = 0;

	static unsigned __stdcall wrappedCaller(void* pArguments)
	{
		((void(*)(void*))(((WLthread*)(pArguments))->userDefinedFunction))(((WLthread*)(pArguments))->param);
		_endthreadex(0);
		return 0;
	}

public:

	WLthread()
	{
		hThread = 0;
		threadId = 0;
		param = 0;
	}

	WLthread(void(*thread)(void*), void* parameterObject)
	{
		userDefinedFunction = (void*)(thread);
		param = parameterObject;
		hThread = (HANDLE)_beginthreadex(NULL, 0, &wrappedCaller, (void*)this, 0, &threadId);
	}

	void join()
	{
		WaitForSingleObject(hThread, INFINITE);
		CloseHandle(hThread);
	}

	unsigned int get_id()
	{
		return threadId;
	}

	void* native_handle()
	{
		return hThread;
	}

	WLthread(const WLthread& other)
	{
		*this = other;
	}

	void operator=(const WLthread& other)
	{
		hThread = other.hThread;
		threadId = other.threadId;
		param = other.param;
	}

#endif



};

//...
