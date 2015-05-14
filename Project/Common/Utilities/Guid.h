#ifndef GEM_GUID_H
#define GEM_GUID_H

#pragma region CHANGE LOG
/// -- April	7, 2015 - Nathan Hanlan - Added in Guid class for generating unique numbers cross platform.
/// -- May	   12, 2015 - Nathan Hanlan - Added Guid to Gem Project.
#pragma endregion

#include "../Core/BasicTypes.h"
#include "ISerializeable.h"
#include "Array.h"

namespace Gem
{
	//class Guid;
	//template class GEM_API Reflection::MetaObject<Guid>;

	/**
	* This class is responsible for using the platform available functions to generate unique numbers.
	*
	*/
	class Guid : public object , public ISerializeable
	{
		//Declare class for reflection
		RDECLARE_CLASS(Guid)
	public:
		/**
		* Default constructor initializes the Guid to a value of 0.
		*/
		Guid();
		/**
		* Constructor initializes the Guid to a value represented by a string
		* If string length is less than 16 the remaining values are defaulted to zero.
		* @param aString The string containing a unique value
		*/
		Guid(const std::string & aString);
		/**
		* Constructor initializes the Guid to a value represented by an array of bytes
		* If the array length is less than 16 the remaining values are defaulted to zero.
		* @param aBytes The bytes containing a unique value. 
		*/
		Guid(const Array<UInt8> & aBytes);
		/**
		* Destructor frees up allocated resources.
		*/
		~Guid();

		/**
		* Converts the GUID to a string
		* @return Returns the string version of the GUID
		*/
		std::string ToString();
		
		/**
		* Generates a new guid
		* @return A new guid generated.
		*/
		static Guid NewGuid();


		/**
		* Compares a Guid with another to determine if they are equal.
		* @param aGuid The guid to compare to.
		* @return Returns if they are equal or not.
		*/
		bool operator==(const Guid & aGuid) const
		{
			for (int i = 0; i < 16; i++)
			{
				if (aGuid.m_Data[i] != m_Data[i])
				{
					return false;
				}
			}
			return true;
		}

		/**
		* Compares a Guid with another to determine if they are not equal.
		* @param aGuid The guid to compare to.
		* @return Returns if they are not equal or not.
		*/
		bool operator!=(const Guid & aGuid) const
		{
			return !(aGuid == *this);
		}

		void OnSerialize(IFormatter * aFormatter, Stream & aStream);
		void OnDeserialize(IFormatter * aFormatter, Stream & aStream);

	private:
		/**
		* The array of bytes allocated for the GUID. (Size == 16).
		*/
		//std::string m_Data;
		UInt8 m_Data[37];
	};

	TYPE_DEFINE(Guid)
}

#endif // GEM_GUID_H