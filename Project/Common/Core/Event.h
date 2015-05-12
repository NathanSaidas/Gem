#ifndef GEM_EVENT_H
#define GEM_EVENT_H

//============================================================
// Date:			May,	12,		2015
// Description:		A class used for sending events to many different users.
//============================================================

#pragma  region CHANGE LOG
// -- Nathan Hanlan	- Implemented base features of class Event.
#pragma endregion

#include <vector>
#include "Func.h"

namespace Gem
{
	/**
	* Export Event Type
	*/
	template class GEM_API std::vector < Func<void>>;

	/**
	* A class used for sending events.
	*/
	template<typename ... ARGS>
	class Event
	{
	public:
		/**
		* Define the function type targets;
		*/
		typedef std::vector<Func<void, ARGS...>> Targets;

		Event()
		{

		}

		virtual ~Event()
		{

		}

		/**
		* Adds a listener to the event listener list. If they are on this list this does nothing.
		*/
		void AddListener(const Func<void, ARGS...> & aListener)
		{
			Targets::const_iterator it = std::find(m_Listeners.begin(), m_Listeners.end(), aListener);
			 if (it == m_Listeners.end())
			 {
				 m_Listeners.push_back(aListener);
			 }
		}

		/**
		* Removes a listener from the event listener list. If they are not on this list, this does nothing.
		*/
		void RemoveListener(const Func<void, ARGS...> & aListener)
		{
			Targets::const_iterator it = std::find(m_Listeners.begin(), m_Listeners.end(), aListener);
			if (it != m_Listeners.end())
			{
				m_Listeners.erase(it);
			}
		}

		/**
		* Invokes the event on all the listeners.
		*/
		void Invoke(ARGS ... args)
		{
			for (Targets::iterator it = m_Listeners.begin(); it != m_Listeners.end(); it++)
			{
				(*it)(args...);
			}
		}

	private:
		/**
		* List of listeners.
		*/
		Targets m_Listeners;

	};
}
	

#endif