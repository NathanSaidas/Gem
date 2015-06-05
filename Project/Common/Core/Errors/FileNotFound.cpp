#include "FileNotFound.h"
#include "../Debug.h"

namespace Gem
{
	namespace Debugging
	{
		RDEFINE_CLASS(FileNotFound,Error)

		void FileNotFound::Log(CString aHeader)
		{
			Debug::LogFormat(aHeader, nullptr, "Error[%d]: %s %s at %s\nFile: %s\nLine%d",
				GetErrorCode(),
				GetErrorString(),
				GetArgumentName(),
				GetMethodFullName(),
				GetErrorTrace().filename,
				GetErrorTrace().line);
		}
	}
}