#ifndef GEM_UTILITIES_H
#define GEM_UTILITIES_H

#include <vector>

#include "StringExtensions.h"
#include "CFileExtensions.h"
#include "../Core/Debug.h"
#include "Array.h"
#include "IniFileStream.h"
#include "IniSection.h"
#include "IniVariables.h"
#include "IniVector3.h"
#include "IniVector4.h"
#include "Stream.h"
#include "IFormatter.h"
#include "ISerializeable.h"
#include "BinaryFormatter.h"
#include "Guid.h"
#include "Directory.h"



namespace Gem
{
    

	namespace Utilities
	{
		template<typename T>
		bool Exists(std::vector<T> & aVector, T aElement)
		{
			for (std::vector<T>::iterator it = aVector.begin(); it != aVector.end(); it++)
			{
				if ((*it) == aElement)
				{
					return true;
				}
			}
			return false;
		}

		template<typename T>
		bool Remove(std::vector<T> & aVector, T aElement)
		{
			for (std::vector<T>::iterator it = aVector.begin(); it != aVector.end(); it++)
			{
				if ((*it) == aElement)
				{
					aVector.erase(it);
					return true;
				}
			}
			return false;
		}


		
		
	}
}

#endif