#ifndef G_UTILITIES_H
#define G_UTILITIES_H

#include "../Primitives/G_PrimitiveObjects.h"

namespace Gem
{
	class Utilities : public Object
	{
	public:
		///Converts a int to a string
		static string I2S(int & aInt);
		///Converts a string to an int
		static int S2I(string & aString);
		///Converts float to string
		static string F2S(float & aFloat);
		///Converts a string to a float
		static float S2F(string & aString);

		///TODO: Consider other platforms (Compilers, CPU Architecture) and 
		//how they layout their memory for such classes and data types
		template<class T>
		static sbyte * Serialize(T * aObject)
		{
			unsigned int SIZE = sizeof(T);
			sbyte * bytes = new sbyte[SIZE + 4];
			memcpy(bytes, &SIZE, 4);

			void * indexA = bytes;
			void * indexB = &bytes[3];

			memcpy(&bytes[4] , aObject, SIZE);
			return bytes;
		}

		template<class T>
		static T * Deserialize(sbyte * aBytes)
		{
#ifdef _DEBUG
			ASSERT(aBytes != nullptr);
#endif
			int size = 0;
			memcpy(&size, aBytes, 4);
			if (sizeof(T) != size)
			{
#ifdef _DEBUG
				Debug::Error("Deserialization failed, size miss match.");
#endif
				return nullptr;
			}
			T * buffer = Memory::Instantiate<T>();
			memcpy(buffer, &aBytes[4], size);
			return buffer;
		}

		template<class T>
		static T Construct(T & aMemory)
		{
			return new(aMemory)T();
		}
	};
}

GEM_CLASS(Utilities,Object)

#endif