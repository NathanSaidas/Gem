#ifndef GEM_THREAD_H
#define GEM_THREAD_H

//============================================================
// Date:			May,	12,		2015
// Description:		The root file for the Application Class. Contains the Application Class as well as typedefs and explicit exporting/importing.
//============================================================

#pragma  region CHANGE LOG
// -- Nathan Hanlan	- Implemented base features of class Application.
#pragma endregion

#include <thread>
#include "../Core/GemAPI.h"
#include "../Core/BasicTypes.h"
#include "../Core/Func.h"

#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4251)
#endif

namespace Gem
{

	class Thread;
	template class GEM_API Reflection::MetaObject<Thread>;
	template class GEM_API Func<void, Thread&>;

	typedef Func<void, Thread&> ThreadEntryCallback;

	class GEM_API Thread : public object
	{
		RDECLARE_CLASS(Thread)
	public:
		Thread();
		Thread(ThreadEntryCallback aEntryPoint);
		~Thread();
		void Start();
		void Start(ThreadEntryCallback aEntryPoint);
		void Wait();
		
		inline bool IsExecuting() const
		{
			return m_StartedExecution;
		}

		inline size_t GetID() const
		{
			if (IsExecuting())
			{
				return m_Thread.get_id().hash();
			}
			return 0;
		}
	private:
		/**
		* The function where the thread starts at.
		*/
		ThreadEntryCallback m_EntryPoint;

		/**
		* A std::thread object which allows for the thread to execute.
		*/
		std::thread m_Thread;

		/**
		* A flag for telling whether or not the thread is executing.
		*/
		bool m_StartedExecution;

		void EntryFunction();
	};
}

#ifdef _MSC_VER
#pragma warning (pop)
#endif

#endif // GEM_THREAD_H