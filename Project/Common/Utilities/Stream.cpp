#include "Stream.h"
#include <stdio.h>
#include <stdlib.h>
#include "../Core/Debug.h"
#include "../Utilities/Utilities.h"

using namespace Gem::Debugging;

namespace Gem
{
	RDEFINE_CLASS(Stream, object)

	Stream::Stream()
	{

	}
	Stream::Stream(std::string & aBytes)
	{
		m_Bytes.clear();
		int startIndex = 0;
		int currentIndex = 0;

		while (true)
		{
			for (; currentIndex < aBytes.size(); currentIndex++)
			{
				if (aBytes[currentIndex] == ',')
				{
					int length = currentIndex - startIndex;
					std::string substring = aBytes.substr(startIndex, length);
					if (substring.size() > 0)
					{
						m_Bytes.push_back(Utilities::StringToByte(substring));
					}
					startIndex = currentIndex + 1;
				}
			}

			if (currentIndex >= aBytes.size())
			{
				break;
			}
		}
		
	}
	Stream::~Stream()
	{

	}
	void Stream::WriteByte(UInt8 aByte)
	{
		m_Bytes.push_back(aByte);
	}

	void Stream::Write(const UInt8 aBytes[], const UInt32 aLength)
	{
		for (unsigned int i = 0; i < aLength; i++)
		{
			m_Bytes.push_back(aBytes[i]);
		}
	}

	void Stream::Write(const Array<UInt8> & aBytes)
	{
		UInt8 * bytes = aBytes.GetElements();
		for (unsigned int i = 0; i < aBytes.GetCount(); i++)
		{
			m_Bytes.push_back(bytes[i]);
		}
	}



	Array<UInt8> Stream::Read(UInt32 aSize)
	{
		Array<UInt8> bytes(aSize);

		std::vector<UInt8>::iterator it;
		for (unsigned int i = 0; i < aSize && m_Bytes.size() > 0; i++)
		{
			it = m_Bytes.begin();
			bytes[i] = *it;
			m_Bytes.erase(it);
		}
		return bytes;
	}

	bool Stream::ReadByte(UInt8 & aByte)
	{
		if (m_Bytes.size() > 0)
		{
			std::vector<UInt8>::iterator it = m_Bytes.begin();
			aByte = *it;
			m_Bytes.erase(it);
		}
		else
		{
			return false;
		}
		return true;
	}

	void Stream::WriteFile(const std::string & aFilename)
	{
		FILE * file = fopen(aFilename.c_str(), "w");
		if (file != nullptr)
		{
			UInt32 length = std::fwrite(&m_Bytes[0], sizeof(UInt8), m_Bytes.size(), file);
			if (length != m_Bytes.size())
			{
				//DEBUG_LOG("Error writing file");

			}
			fclose(file);
		}

	}
	void Stream::ReadFile(const std::string & aFilename)
	{
		m_Bytes.clear();
		FILE * file = fopen(aFilename.c_str(), "r");
		if (file != nullptr)
		{
			if (fseek(file, 0, SEEK_END) == 0)
			{
				SInt32 size = ftell(file);
				if (size == -1)
				{
					Debug::Log("Gem", "Error reading file. Buffer size is -1");
				}


				if (fseek(file, 0, SEEK_SET) != 0)
				{
					Debug::Log("Gem", "Error reading file.");
				}

				UInt32 length = std::fread(&m_Bytes[0], sizeof(UInt8), size, file);
				if (length == 0)
				{
					Debug::Log("Gem", "Error reading file.");
				}
			}
			fclose(file);
		}
		
	}

	Array<UInt8> Stream::ToBytes()
	{
		Array<UInt8> bytes;
		Array<UInt8>::Copy(m_Bytes, bytes);
		return bytes;
	}

	std::string Stream::ToString()
	{
		std::string result;
		for (int i = 0; i < m_Bytes.size(); i++)
		{
			std::string stringValue = Utilities::ByteToString(m_Bytes[i]);
			result.append(stringValue);
			if (i != m_Bytes.size() - 1)
			{
				result.append(",");
			}
		}
		return result;
	}
}