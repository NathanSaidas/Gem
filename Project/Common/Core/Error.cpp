#include "Error.h"
#include "Debug.h"


namespace Gem
{
	namespace Debugging
	{
		RDEFINE_CLASS(Error, object)

		void Error::Log(CString aHeader)
		{
			Debug::LogFormat(aHeader, nullptr, "Error[%d]: %s at %s\nFile: %s\nLine%d",
			GetErrorCode(),
			GetErrorString(),
			GetMethodFullName(),
			GetErrorTrace().filename,
			GetErrorTrace().line);
		}
	}
}