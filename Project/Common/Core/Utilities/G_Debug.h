#ifndef G_DEBUG_H
#define G_DEBUG_H

#include "../Primitives/G_PrimitiveObjects.h"




namespace Gem
{
	namespace FileIO
	{
		class File;
	}
	

	class Debug : public Object
	{
	public:
		static Debug * instance();
		static void destroy();


		static void log(string aMessage, bool aWriteFile = false);
		static void log(Object aMessage, bool aWriteFile = false);
		static void log(Object * aMessage, bool aWriteFile = false);
		static void warning(string aMessage, bool aWriteFile = false);
		static void warning(Object aMessage, bool aWriteFile = false);
		static void warning(Object * aMessage, bool aWriteFile = false);
		static void error(string aMessage, bool aWriteFile = false);
		static void error(Object aMessage, bool aWriteFile = false);
		static void error(Object * aMessage, bool aWriteFile = false);
		static void saveLogFile();
		static void clearLogFile();

		Pointer<Reflection::Type> getType() override;

	private:
		static Debug * s_Instance;
		Debug();
		~Debug();

		void debugLog(string aMessage, bool aWriteFile);
		void debugWarning(string aMessage, bool aWriteFile);
		void debugError(string aMessage, bool aWriteFile);
		void debugSaveLogFile();
		void debugClearLogFile();
		

		FileIO::File * m_File;


		friend class Reflection::ReflectionFactory;

	};
	
}
GEM_CLASS(Debug,Object)
#endif