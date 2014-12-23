#ifndef G_UNIQUE_NUMBER_GENERATOR_H
#define G_UNIQUE_NUMBER_GENERATOR_H

#pragma region CHANGE LOG
/* November,18,2014 - Nathan, Added and implemented UniqueNumberGenerator Class
*
*/
#pragma endregion

#include "Collections\G_List.h"
#include "../G_Object.h"

namespace Gem
{
	///A class to generate unique numbers
	///See Get and Free methods.
	class UniqueNumberGenerator : public object
	{
		G_CLASS_DEF(UniqueNumberGenerator)
	public:
		UniqueNumberGenerator();
		UniqueNumberGenerator(int aStartID);
		~UniqueNumberGenerator();

		///Retrieves the next available ID and increments the ID
		int Get();
		///Reserves an ID, Returns true if successful, false if the ID cannot be reserved
		bool Reserve(int aID);
		///Frees a ID back so it can be reused.
		void Free(int aID);
		///Resets the generator to the specified start ID.
		void Reset(int aStart = 0);
		///Returns true if the ID is on the free list.
		bool IsFree(int aID);
		///Accessor to the m_NextNumber member. This is not the same as Get it does not increment the next ID.
		int NextNumber();
		///Accessor to the m_LargestNumber member.
		int LargestNumber();
	private:
		///A list of IDs that are currently available
		Collections::List<int> m_FreeList;
		///A list of reserved IDs that cannot be taken
		Collections::List<int> m_ReservedList;
		///The next number to be chosen
		int m_NextNumber;
		///The largest number generated so far
		int m_LargestNumber;
	};
}
#endif