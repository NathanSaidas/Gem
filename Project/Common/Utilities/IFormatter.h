#ifndef GEM_I_FORMATTER_H
#define GEM_I_FORMATTER_H


#include "../Core/GemAPI.h"
#include "../Core/BasicTypes.h"
#include "../Math/Math.h"
#include "Stream.h"


namespace Gem
{
	//class IFormatter;
	//template class GEM_API Reflection::MetaObject<IFormatter>;

	/**
	*	IFormatter represents the interface class for marshalling data in and out of streams.
	*/
	class GEM_API IFormatter
	{
		RDECLARE_INTERFACE(IFormatter);
	public:
		/**
		* Serialize methods format the data from an object into raw data then adding that data to the stream.
		*/

		virtual void Serialize(SInt32 aValue, Stream & aStream) = 0;
		virtual void Serialize(SInt16 aValue, Stream & aStream) = 0;
		virtual void Serialize(SInt8 aValue, Stream & aStream) = 0;
		virtual void Serialize(UInt32 aValue, Stream & aStream) = 0;
		virtual void Serialize(UInt16 aValue, Stream & aStream) = 0;
		virtual void Serialize(UInt8 aValue, Stream & aStream) = 0;
		virtual void Serialize(Float32 aValue, Stream & aStream) = 0;
		virtual void Serialize(bool aValue, Stream & aStream) = 0;
		virtual void Serialize(Vector2 aValue, Stream & aStream) = 0;
		virtual void Serialize(Vector3 aValue, Stream & aStream) = 0;
		virtual void Serialize(Vector4 aValue, Stream & aStream) = 0;
		virtual void Serialize(Quaternion aValue, Stream & aStream) = 0;
		virtual void Serialize(Matrix4x4 aValue, Stream & aStream) = 0;
		virtual void Serialize(std::string & aValue, Stream & aStream) = 0;

		/**
		* Deserialize methods format the data from a stream into an object.
		*/
		virtual bool Deserialize(SInt32 & aValue, Stream & aStream) = 0;
		virtual bool Deserialize(SInt16 & aValue, Stream & aStream) = 0;
		virtual bool Deserialize(SInt8 & aValue, Stream & aStream) = 0;
		virtual bool Deserialize(UInt32 & aValue, Stream & aStream) = 0;
		virtual bool Deserialize(UInt16 & aValue, Stream & aStream) = 0;
		virtual bool Deserialize(UInt8 & aValue, Stream & aStream) = 0;
		virtual bool Deserialize(Float32 & aValue, Stream & aStream) = 0;
		virtual bool Deserialize(bool & aValue, Stream & aStream) = 0;
		virtual bool Deserialize(Vector2 & aValue, Stream & aStream) = 0;
		virtual bool Deserialize(Vector3 & aValue, Stream & aStream) = 0;
		virtual bool Deserialize(Vector4 & aValue, Stream & aStream) = 0;
		virtual bool Deserialize(Quaternion & aValue, Stream & aStream) = 0;
		virtual bool Deserialize(Matrix4x4 & aValue, Stream & aStream) = 0;
		virtual bool Deserialize(std::string & aValue, Stream & aStream) = 0;
	};

	TYPE_DEFINE(IFormatter)
}

#endif //GEM_I_FORMATTER_H