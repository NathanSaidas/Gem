#ifndef GEM_RESOURCE_H
#define GEM_RESOURCE_H

//============================================================
// Date:			May,	13,		2015
// Description:		Defines the class Resource which is the base class for all resource types in the project.
//============================================================

#pragma  region CHANGE LOG
// -- Nathan Hanlan	- Refactored this class to fit in the Gem project from SchoolGameEngine
// -- Nathan Hanlan - Constructor now uses initialization list.
#pragma endregion

//Using for basic types for Object
#include "../Core/GemAPI.h"
#include "../Core/BasicTypes.h"
#include "../Utilities/Utilities.h"

#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4251)
#endif

namespace Gem
{

	//class Resource;
	//template class GEM_API Reflection::MetaObject<Resource>;

	/**
	* Resource represents the base class for all future resources.
	* Texture/Mesh/Shader
	*/
	class Resource : public Object
	{
		RDECLARE_CLASS(Resource)
	public:
		/**
		*
		*/
		Resource();
		/**
		*
		*/
		~Resource();

		
		/**
		* Generates the Unique ID
		*/
		void GenerateID();
		/**
		* Gets the unique ID
		* @return Returns the unique ID
		*/
		Guid GetID();

	protected:
		/**
		* Saves the changes made to a resource
		* @param The path to the directory of the resource. Save uses ../Directory/GetName for the full path.
		*/
		virtual void SaveMeta(IniFileStream & aDirectory);
		/**
		* Loads changes made from a resource
		@param The path to the directory of the resource. Load uses ../Directory/GetName for the full path.
		*/
		virtual void LoadMeta(IniFileStream & aDirectory);

	private:
		Guid m_UniqueID;
		friend class ResourceDatabase;
	};

	TYPE_DEFINE(Resource)
}

#ifdef _MSC_VER
#pragma warning (pop)
#endif

#endif