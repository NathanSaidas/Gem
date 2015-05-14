#ifndef GAME_ENGINE_STREAM_H
#define GAME_ENGINE_STREAM_H

#include <vector>

#include "../Core/GemAPI.h"
#include "../Core/BasicTypes.h"
#include "Array.h"

namespace Gem
{
	class Stream;
	template class GEM_API Reflection::MetaObject<Stream>;

	template class GEM_API std::vector<UInt8>;

	/**
	* Stream represents an array of data.
	*/
	class GEM_API Stream : public object
	{
		RDECLARE_CLASS(Stream)
	public:

		Stream();
		~Stream();

		/**
		* Writes a single byte to the stream.
		* @param aByte The byte to be written
		*/
		void WriteByte(UInt8 aByte);
		/**
		* Writes an array of bytes to the stream.
		* @param aBytes The bytes to be written
		* @param aLength The amount of bytes to be written
		*/
		void Write(const UInt8 aBytes[], const UInt32 aLength);
		/**
		* Writes an array of bytes to the stream
		* @param aBytes An array of bytes to write
		*/
		void Write(const Array<UInt8> & aBytes);
		/**
		* Reads x bytes from the stream. If the size of the array returned does not match aSize there was an error reading bytes.
		* @param aSize The amount of bytes to read
		* @return An array of bytes read from the stream.
		*/
		Array<UInt8> Read(UInt32 aSize);
		/**
		* Reads a single byte from the stream
		* @param aByte The byte to be read.
		* @return Returns true if read successful or false if bad read.
		*/
		bool ReadByte(UInt8 & aByte);

		/**
		* Writes the stream to a file.
		* @param aFilename The full path of the file to write to
		*/
		void WriteFile(const std::string & aFilename);
		/**
		* Reads a files data into the stream.
		* @param aFilename The full path of the file to read from.
		*/
		void ReadFile(const std::string & aFilename);

		/**
		* Converts the stream to an array of bytes.
		* @return Returns an array of bytes.
		*/
		Array<UInt8> ToBytes();

	private:
		/**
		* The bytes of data for the stream.
		*/
		std::vector<UInt8> m_Bytes;
	};

	TYPE_DEFINE(Stream)
}

#endif // GEM_STREAM_H