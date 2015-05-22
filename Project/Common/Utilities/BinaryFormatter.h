#ifndef GEM_BINARY_FORMATTER_H
#define GEM_BINARY_FORMATTER_H

#include "IFormatter.h"

namespace Gem
{

	class BinaryFormatter;
	template class GEM_API Reflection::MetaObject<BinaryFormatter>;

	/**
	* BinaryFormatter formats the primitive data types into a stream in a binary format.
	*/
	class GEM_API BinaryFormatter : public object , public IFormatter
	{
		RDECLARE_CLASS(BinaryFormatter)
        RDECLARE_ATTRIBUTE_INTERFACE(BinaryFormatter,IFormatter)
	public:

		void Serialize(SInt32 aValue, Stream & aStream);
		void Serialize(SInt16 aValue, Stream & aStream);
		void Serialize(SInt8 aValue, Stream & aStream);
		void Serialize(UInt32 aValue, Stream & aStream);
		void Serialize(UInt16 aValue, Stream & aStream);
		void Serialize(UInt8 aValue, Stream & aStream);
		void Serialize(Float32 aValue, Stream & aStream);
		void Serialize(bool aValue, Stream & aStream);
		void Serialize(Vector2 aValue, Stream & aStream);
		void Serialize(Vector3 aValue, Stream & aStream);
		void Serialize(Vector4 aValue, Stream & aStream);
		void Serialize(Quaternion aValue, Stream & aStream);
		void Serialize(Matrix4x4 aValue, Stream & aStream);
		void Serialize(std::string & aValue, Stream & aStream);


		bool Deserialize(SInt32 & aValue, Stream & aStream);
		bool Deserialize(SInt16 & aValue, Stream & aStream);
		bool Deserialize(SInt8 & aValue, Stream & aStream);
		bool Deserialize(UInt32 & aValue, Stream & aStream);
		bool Deserialize(UInt16 & aValue, Stream & aStream);
		bool Deserialize(UInt8 & aValue, Stream & aStream);
		bool Deserialize(Float32 & aValue, Stream & aStream);
		bool Deserialize(bool & aValue, Stream & aStream);
		bool Deserialize(Vector2 & aValue, Stream & aStream);
		bool Deserialize(Vector3 & aValue, Stream & aStream);
		bool Deserialize(Vector4 & aValue, Stream & aStream);
		bool Deserialize(Quaternion & aValue, Stream & aStream);
		bool Deserialize(Matrix4x4 & aValue, Stream & aStream);
		bool Deserialize(std::string & aValue, Stream & aStream);
	};

	TYPE_DEFINE(BinaryFormatter)
}


#endif //GEM_BINARY_FORMATTER