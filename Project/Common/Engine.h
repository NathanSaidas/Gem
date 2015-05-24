#ifndef GEM_ENGINE_H
#define GEM_ENGINE_H


//============================================================
// Date:			May,	11,		2015
// Description:		This is the root file to include which includes the whole Gem Framework/Engine.
//============================================================
#pragma warning ( disable : 4018)
#pragma warning ( disable : 4244)

#include "Graphics\Graphics.h"
#include "Core\GemAPI.h"
#include "Core\Debug.h"
#include "Core\Time.h"
#include "Core\ErrorConstants.h"
#include "Memory\Memory.h"
#include "Reflection\Reflection.h"
#include "Math\Math.h"
#include "Application\Application.h"
#include "Application\Input.h"
//Window
#include "Window\Window.h"
#include "Window\Win32Window.h"
#include "Window\Win32Message.h"
//Utils
#include "Utilities\Utilities.h"
#include "EntityComponent\EntityComponent.h"
#include "Resource\ResourceDatabase.h"


//Namespaces Used.



namespace Gem
{
	namespace Debugging
	{

	}
	namespace Memory
	{

	}
}

#endif