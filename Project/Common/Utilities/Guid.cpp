#include "Guid.h"

#ifdef _WIN32
#include <rpc.h>
#else
#include <uuid/uid.h>
#endif

namespace Gem
{
	const int GUID_STRING_LENGTH = 36;
	RDEFINE_CLASS(Guid,object)

	Guid::Guid() : object()
	{
		//m_Data = new UInt8[GUID_STRING_LENGTH + 1];
		memset(m_Data, 0, sizeof(UInt8)* GUID_STRING_LENGTH);
		m_Data[GUID_STRING_LENGTH] = '\0';
	}
	Guid::Guid(const std::string & aString) : object()
	{
		//m_Data = new UInt8[GUID_STRING_LENGTH + 1];
		memset(m_Data, 0, sizeof(UInt8)* GUID_STRING_LENGTH);
		m_Data[GUID_STRING_LENGTH] = '\0';
		for (unsigned int i = 0; i < aString.length() && i < GUID_STRING_LENGTH; i++)
		{
			m_Data[i] = (UInt8)aString[i];
		}
	}
	Guid::Guid(const Array<UInt8> & aBytes) : object()
	{
		//m_Data = new UInt8[GUID_STRING_LENGTH + 1];
		memset(m_Data, 0, sizeof(UInt8)* GUID_STRING_LENGTH);
		m_Data[GUID_STRING_LENGTH] = '\0';
		for (unsigned int i = 0; i < aBytes.GetCount() && i < GUID_STRING_LENGTH; i++)
		{
			m_Data[i] = aBytes[i];
		}
	}
	Guid::~Guid()
	{
		//delete m_Data;
		//m_Data = nullptr;
	}

	std::string Guid::ToString()
	{
		return std::string((char*)m_Data);
	}

	Guid Guid::NewGuid()
	{
#ifdef WIN32
		UUID uuid;
		//This requires Rpcrt4.lib to be added to linker options.
		::UuidCreate(&uuid);

		unsigned char * str;
		UuidToStringA(&uuid, &str);

		std::string s = ((char*)str);
		RpcStringFreeA(&str);
#else
		//TODO(Nathan): Linux Implementation
#endif
		return Guid(s);
	}

	void Guid::OnSerialize(IFormatter * aFormatter, Stream & aStream)
	{
		for (int i = 0; i < GUID_STRING_LENGTH; i++)
		{
			aFormatter->Serialize(m_Data[i], aStream);
		}
	}
	void Guid::OnDeserialize(IFormatter * aFormatter, Stream & aStream)
	{
		for (int i = 0; i < GUID_STRING_LENGTH; i++)
		{
			aFormatter->Deserialize(m_Data[i], aStream);
		}
	}
}