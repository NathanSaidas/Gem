#include "Resource.h"
#include "ResourceDatabase.h"

namespace Gem
{
	RDEFINE_CLASS(Resource, Object)

	/**
	*
	*/
	Resource::Resource() : m_UniqueID(Guid::NewGuid())
	{
	}
	/**
	*
	*/
	Resource::~Resource()
	{

	}
	
	/**
	* Generates the Unique ID
	*/
	void Resource::GenerateID()
	{
		m_UniqueID = Guid::NewGuid();
	}
	/**
	* Gets the unique ID
	* @return Returns the unique ID
	*/
	Guid Resource::GetID()
	{
		return m_UniqueID;
	}
	/**
	* Saves the changes made to a resource
	* @param The path to the directory of the resource. Save uses ../Directory/GetName for the full path.
	*/
	void Resource::SaveMeta(IniFileStream & aFileStream)
	{
		if (!aFileStream.BindSection("Resource"))
		{
			aFileStream.AddSection("Resource");
			aFileStream.BindSection("Resource");
		}

		if (IniString::IsBadValue(aFileStream.GetString(ResourceDatabase::META_ID_TOKEN)))
		{
			aFileStream.AddString(ResourceDatabase::META_ID_TOKEN, m_UniqueID.ToString());
		}
		else
		{
			aFileStream.SetString(ResourceDatabase::META_ID_TOKEN, m_UniqueID.ToString());
		}
	}
	/**
	* Loads changes made from a resource
	@param The path to the directory of the resource. Load uses ../Directory/GetName for the full path.
	*/
	void Resource::LoadMeta(IniFileStream & aFileStream)
	{
		if (aFileStream.BindSection("Resource"))
		{
			IniString metaID = aFileStream.GetString(ResourceDatabase::META_ID_TOKEN);
			if (!IniString::IsBadValue(metaID))
			{
				m_UniqueID = Guid(metaID.GetValue());
			}
		}
	}
}