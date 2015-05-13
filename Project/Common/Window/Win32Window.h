#ifndef GEM_WIN_32_WINDOW_H
#define GEM_WIN_32_WINDOW_H

#include "../Core/GemAPI.h"
#include "../Core/BasicTypes.h"

#ifdef _WIN32
#include <Windows.h>
#endif

namespace Gem
{
	class Win32Window
	{
	public:
#ifdef _WIN32
		Win32Window(const std::string & aWindowName, HINSTANCE aHandleInstance);
#endif
		virtual ~Win32Window();

		virtual bool Open();
		virtual bool Close();

		virtual void SwapBuffer();
	private:
#ifdef _WIN32
		HINSTANCE m_HandleInstance;
		HWND m_WindowHandle;
		HDC m_WindowDeviceContext;
		HGLRC m_OpenGLContext;
		std::string m_WindowClassName;
#endif
		std::string m_WindowName;
		UInt32 m_WindowWidth;
		UInt32 m_WindowHeight;

		SInt32 m_RedBits;
		SInt32 m_GreenBits;
		SInt32 m_BlueBits;
		SInt32 m_AlphaBits;
		SInt32 m_DepthBits;
		SInt32 m_StencilBits;
		bool m_IsFullscreen;
		bool m_IsOpen;
		
	};

	TYPE_DEFINE(Win32Window)
}

#endif