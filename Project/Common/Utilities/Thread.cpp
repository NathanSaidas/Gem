#include "Thread.h"
#include "../Core/Debug.h"

using namespace Gem::Debugging;


namespace Gem
{
	RDEFINE_CLASS(Thread,object)

	Thread::Thread() : m_EntryPoint(nullptr),
		m_StartedExecution(false)
	{

	}
	Thread::Thread(ThreadEntryCallback aEntryPoint)
		: m_EntryPoint(aEntryPoint),
		m_StartedExecution(false)
	{

	}
	Thread::~Thread()
	{
		if (m_Thread.joinable())
		{
			m_Thread.join();
		}
	}
	void Thread::Start()
	{
		//Cannot execute the thread. It's already executing.
		if (IsExecuting())
		{
			return;
		}
		m_Thread = std::thread(&Thread::EntryFunction, this); //Start executing.
	}
	void Thread::Start(ThreadEntryCallback aEntryPoint)
	{
		if (IsExecuting())
		{
			return;
		}
		m_EntryPoint = aEntryPoint;
		Start();
	}
	void Thread::Wait()
	{
		m_Thread.join();
	}

	void Thread::EntryFunction()
	{
		Debug::Log("Thread", "Thread Started");
		m_StartedExecution = true;
		m_EntryPoint.Invoke(*this);
		m_StartedExecution = false;
	}


}