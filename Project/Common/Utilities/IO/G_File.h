#ifndef G_FILE_H
#define G_FILE_H

#include "G_FileMode.h" //For FileMode enum
#include "G_FileAccess.h" //Fore FileAccess enum
#include "../../G_Object.h"

namespace Gem
{
	namespace FileIO
	{
#pragma region Change Log
		/* October,20,2014 - Nathan Hanlan - Moved File to its own File.
		*  October,20,2014 - Nathan Hanlan - Removing support for std::string. Adding support for Gem::string and const char *
		*  
		*/
#pragma endregion ChangeLog
		/// <summary>
		/// A data structure used to handle file operations. Open, Close, Create, Exists etc.
		/// </summary>
		class File : public object
		{
			G_CLASS_DEF(File)
		public:
			File();
			/// <summary>
			/// Construct the file with the following path.
			/// </summary>
			/// <param name="aPath">The location of the file.</param>
			File(std::string & aPath);
			/// <summary>
			/// Construct the file with the following path.
			/// </summary>
			/// <param name="aPath">The location of the file.</param>
			File(const char * aPath);
			/// <summary>
			/// Destroys the file.
			/// </summary>
			~File();

			/// <summary>
			/// Opens the file with the current filepath and reads and/or writes to the file with the current read / write streams
			/// </summary>
			/// <param name="aFileMode">The filemode to use.</param>
			/// <returns>Returns true if successful. False if not.</returns>
			bool Open(FileMode aFileMode);
			/// <summary>
			/// Opens the file and reads and/or writes to the file with the current read / write streams
			/// </summary>
			/// <param name="aPath">The file path to open.</param>
			/// <param name="aFileMode">The filemode to use.</param>
			/// <returns>Returns true if successful. False if not.</returns>
			bool Open(std::string & aPath, FileMode aFileMode);
			/// <summary>
			/// Opens the file and reads and/or writes to the file with the current read / write streams
			/// </summary>
			/// <param name="aPath">The file path to open.</param>
			/// <param name="aFileMode">The filemode to use.</param>
			/// <returns>Returns true if successful. False if not.</returns>
			bool Open(const char * aPath, FileMode aFileMode);

			/// <summary>
			/// Closes the file. Read and Write streams become empty.
			/// </summary>
			void Close();


			/// <summary>
			/// Creates a file with the given filename and extension at the given path.
			/// </summary>
			/// <param name="aPath">The file path to create the file at.</param>
			/// <param name="aFilename">The name of the file to create.</param>
			/// <param name="aPath">The extension to give the file,".txt" by default.</param>
			/// <returns>Returns true if successful. False if not.</returns>
			bool Create(std::string & aPath, std::string & aFilename, std::string & aExtension = std::string(".txt"));

			/// <summary>
			/// Determines if the file exists or not.
			/// </summary>
			/// <param name="aFilename">The file to check for.</param>
			/// <returns>Returns true if exists. False if not.</returns>
			static bool Exists(std::string & aFilename);
			/// <summary>
			/// Determines if the file exists or not with the given extension
			/// </summary>
			/// <param name="aFilename">The file to check for.</param>
			/// <param name="aExtension">The extension to check for.</param>
			/// <returns>Returns true if exists. False if not.</returns>
			static bool Exists(std::string & aFilename, std::string & aExtension);
			static bool Exists(File * aFile);

			static std::string Extension(std::string & aFilename);
			static std::string Extension(File * aFile);

			static std::string ReplaceExtension(std::string & aFilename, std::string & aNewExtension);
			static std::string ReplaceExtension(File * aFile, std::string & aNewExtension);
			/// <summary>
			///October,20,2014 std::string no longer supported.
			/// </summary>
			//inline std::string filename()
			//{
			//	return m_Path;
			//}



			std::string ReadStream();
			std::string WriteStream();
			void SetWriteStream(std::string & aData);
			void AddWriteStream(std::string & aData);
			void ClearWriteStream();
			std::string Path();

			bool Verify();
		private:

			void Read(FILE * aFile);
			void Write(FILE * aFile);
			void ReadWrite(FILE * aFile);

			std::string m_Path;
			std::string m_ReadStream;
			std::string m_WriteStream;
		};
	}
}

#endif