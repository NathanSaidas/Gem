#include "BinaryFormatter.h"

namespace Gem
{
	RDEFINE_CLASS(BinaryFormatter, object)
    RDEFINE_ATTRIBUTE_INTERFACE(BinaryFormatter,IFormatter)

	void BinaryFormatter::Serialize(SInt32 aValue, Stream & aStream)
	{
		Serialize(*(UInt32*)&aValue, aStream);
	}
	void BinaryFormatter::Serialize(SInt16 aValue, Stream & aStream)
	{
		Serialize(*(UInt16*)&aValue, aStream);
	}
	void BinaryFormatter::Serialize(SInt8 aValue, Stream & aStream)
	{
		Serialize(*(UInt8*)&aValue, aStream);
	}

	void BinaryFormatter::Serialize(UInt32 aValue, Stream & aStream)
	{
		Array<UInt8> bytes(4);
		bytes[0] = (aValue >> 24) & 0xFF;
		bytes[1] = (aValue >> 16) & 0xFF;
		bytes[2] = (aValue >> 8) & 0xFF;
		bytes[3] = aValue & 0xFF;

		aStream.Write(bytes);

	}
	void BinaryFormatter::Serialize(UInt16 aValue, Stream & aStream)
	{
		Array<UInt8> bytes(2);
		bytes[0] = (aValue >> 8) & 0xFF;
		bytes[1] = aValue & 0xFF;

		aStream.Write(bytes);
	}
	void BinaryFormatter::Serialize(UInt8 aValue, Stream & aStream)
	{
		aStream.WriteByte(aValue);
	}
	void BinaryFormatter::Serialize(Float32 aValue, Stream & aStream)
	{
		
		UInt32 value = *(UInt32*)&aValue;
		Array<UInt8> bytes(4);
		bytes[0] = (value >> 24) & 0xFF;
		bytes[1] = (value >> 16) & 0xFF;
		bytes[2] = (value >> 8) & 0xFF;
		bytes[3] = value & 0xFF;

		aStream.Write(bytes);
	}
	void BinaryFormatter::Serialize(bool aValue, Stream & aStream)
	{
		aStream.WriteByte((UInt8)aValue);
	}
	void BinaryFormatter::Serialize(Vector2 aValue, Stream & aStream)
	{
		Serialize(aValue.x, aStream);
		Serialize(aValue.y, aStream);
	}
	void BinaryFormatter::Serialize(Vector3 aValue, Stream & aStream)
	{
		Serialize(aValue.x, aStream);
		Serialize(aValue.y, aStream);
		Serialize(aValue.z, aStream);
	}
	void BinaryFormatter::Serialize(Vector4 aValue, Stream & aStream)
	{
		Serialize(aValue.x, aStream);
		Serialize(aValue.y, aStream);
		Serialize(aValue.z, aStream);
		Serialize(aValue.w, aStream);
	}
	void BinaryFormatter::Serialize(Quaternion aValue, Stream & aStream)
	{
		Serialize(aValue.x, aStream);
		Serialize(aValue.y, aStream);
		Serialize(aValue.z, aStream);
		Serialize(aValue.w, aStream);
	}
	void BinaryFormatter::Serialize(Matrix4x4 aValue, Stream & aStream)
	{
		Serialize(aValue[0][0], aStream);
		Serialize(aValue[1][0], aStream);
		Serialize(aValue[2][0], aStream);
		Serialize(aValue[3][0], aStream);

		Serialize(aValue[0][1], aStream);
		Serialize(aValue[1][1], aStream);
		Serialize(aValue[2][1], aStream);
		Serialize(aValue[3][1], aStream);

		Serialize(aValue[0][2], aStream);
		Serialize(aValue[1][2], aStream);
		Serialize(aValue[2][2], aStream);
		Serialize(aValue[3][2], aStream);

		Serialize(aValue[0][3], aStream);
		Serialize(aValue[1][3], aStream);
		Serialize(aValue[2][3], aStream);
		Serialize(aValue[3][3], aStream);
	}

	void BinaryFormatter::Serialize(std::string & aValue, Stream & aStream)
	{
		UInt32 length = aValue.size();
		Serialize(length, aStream);

		for (unsigned int i = 0; i < length; i++)
		{
			Serialize((SInt8)aValue[i], aStream);
		}
	}

	bool BinaryFormatter::Deserialize(SInt32 & aValue, Stream & aStream)
	{
		UInt32 value;
		if (Deserialize(value, aStream))
		{
			aValue = *(SInt32*)&value;
			return true;
		}
		return false;
	}
	bool BinaryFormatter::Deserialize(SInt16 & aValue, Stream & aStream)
	{
		UInt16 value;
		if (Deserialize(value, aStream))
		{
			aValue = *(SInt16*)&value;
			return true;
		}
		return false;
	}
	bool BinaryFormatter::Deserialize(SInt8 & aValue, Stream & aStream)
	{
		UInt8 value;
		if (Deserialize(value, aStream))
		{
			aValue = *(SInt8*)&value;
			return true;
		}
		return false;
	}

	bool BinaryFormatter::Deserialize(UInt32 & aValue, Stream & aStream)
	{
		Array<UInt8> bytes = aStream.Read(4);

		if (bytes.GetCount() != 4)
		{
			return false;
		}

		aValue = 0;

		aValue = bytes[0];
		aValue = aValue << 8;
		aValue |= bytes[1];
		aValue = aValue << 8;
		aValue |= bytes[2];
		aValue = aValue << 8;
		aValue |= bytes[3];

		return true;
	}
	bool BinaryFormatter::Deserialize(UInt16 & aValue, Stream & aStream)
	{
		Array<UInt8> bytes = aStream.Read(2);

		if (bytes.GetCount() != 4)
		{
			return false;
		}

		aValue = 0;

		aValue = bytes[0];
		aValue = aValue << 8;
		aValue |= bytes[1];

		return true;
	}
	bool BinaryFormatter::Deserialize(UInt8 & aValue, Stream & aStream)
	{
		if (aStream.ReadByte(aValue))
		{
			return true;
		}
		return false;
	}
	bool BinaryFormatter::Deserialize(Float32 & aValue, Stream & aStream)
	{
		Array<UInt8> bytes = aStream.Read(4);

		if (bytes.GetCount() != 4)
		{
			return false;
		}

		UInt32 value = 0;

		value = bytes[0];
		value = value << 8;
		value |= bytes[1];
		value = value << 8;
		value |= bytes[2];
		value = value << 8;
		value |= bytes[3];


		aValue = *(Float32*)&value;

		return true;
	}
	bool BinaryFormatter::Deserialize(bool & aValue, Stream & aStream)
	{
		UInt8 value;
		if (aStream.ReadByte(value))
		{
			aValue = (bool)value;
			return true;
		}

		return false;
	}
	bool BinaryFormatter::Deserialize(Vector2 & aValue, Stream & aStream)
	{
		bool noError = 
			Deserialize(aValue.x, aStream) && 
			Deserialize(aValue.y, aStream);
		if (noError)
		{
			return true;
		}
		return false;
	}
	bool BinaryFormatter::Deserialize(Vector3 & aValue, Stream & aStream)
	{
		bool noError =
			Deserialize(aValue.x, aStream) &&
			Deserialize(aValue.y, aStream) &&
			Deserialize(aValue.z, aStream);
		if (noError)
		{
			return true;
		}
		return false;
	}
	bool BinaryFormatter::Deserialize(Vector4 & aValue, Stream & aStream)
	{
		bool noError =
			Deserialize(aValue.x, aStream) &&
			Deserialize(aValue.y, aStream) &&
			Deserialize(aValue.z, aStream) &&
			Deserialize(aValue.w, aStream);
		if (noError)
		{
			return true;
		}
		return false;
	}
	bool BinaryFormatter::Deserialize(Quaternion & aValue, Stream & aStream)
	{
		bool noError =
			Deserialize(aValue.x, aStream) &&
			Deserialize(aValue.y, aStream) &&
			Deserialize(aValue.z, aStream) &&
			Deserialize(aValue.w, aStream);
		if (noError)
		{
			return true;
		}
		return false;
	}
	bool BinaryFormatter::Deserialize(Matrix4x4 & aValue, Stream & aStream)
	{
		bool noError =
			Deserialize(aValue[0][0], aStream) &&
			Deserialize(aValue[1][0], aStream) &&
			Deserialize(aValue[2][0], aStream) &&
			Deserialize(aValue[3][0], aStream) &&

			Deserialize(aValue[0][1], aStream) &&
			Deserialize(aValue[1][1], aStream) &&
			Deserialize(aValue[2][1], aStream) &&
			Deserialize(aValue[3][1], aStream) &&

			Deserialize(aValue[0][2], aStream) &&
			Deserialize(aValue[1][2], aStream) &&
			Deserialize(aValue[2][2], aStream) &&
			Deserialize(aValue[3][2], aStream) &&

			Deserialize(aValue[0][3], aStream) &&
			Deserialize(aValue[1][3], aStream) &&
			Deserialize(aValue[2][3], aStream) &&
			Deserialize(aValue[3][3], aStream);
		if (noError)
		{
			return true;
		}
		return false;
	}

	bool BinaryFormatter::Deserialize(std::string & aValue, Stream & aStream)
	{
		UInt32 length = 0;
		if (!Deserialize(length, aStream))
		{
			return false;
		}

		aValue.resize(length);

		for (UInt32 i = 0; i < length; i++)
		{
			SInt8 byte = 0;
			if (!Deserialize(byte, aStream))
			{
				return false;
			}
			aValue[i] = byte;
		}

		return true;
	}
}