#include "ArgumentNull.h"
#include "../Debug.h"

namespace Gem
{
	namespace Debugging
	{
		RDEFINE_CLASS(ArgumentNull, Error)

		void ArgumentNull::Log(CString aHeader)
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