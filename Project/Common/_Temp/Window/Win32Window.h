#ifndef GEM_WIN_32_WINDOW_H
#define GEM_WIN_32_WINDOW_H

#include "../Core/GemAPI.h"
#include "../Core/BasicTypes.h"
#include "Window.h"

#ifdef _WIN32
//#include <Windows.h>

//#define DECLARE_HANDLE(name) struct name##__{int unused;}; typedef struct name##__ *name

struct HINSTANCE__;
typedef struct HINSTANCE__ *HINSTANCE;

struct HWND__;
typedef struct HWND__ *HWND;

struct HDC__;
typedef struct HDC__ *HDC;

struct HGLRC__;
typedef struct HGLRC__ *HGLRC;

#endif

namespace Gem
{
	class Win32Window;
	template class GEM_API Reflection::MetaObject<Win32Window>;

	class Win32Window : public Window
	{
		RDECLARE_CLASS(Win32Window)
	public:
		Win32Window();
#ifdef _WIN32
		Win32Window(const std::string & aWindowName, HINSTANCE aHandleInstance);
#endif
		virtual ~Win32Window();

		bool Open() override;
		bool Close() override;

		void SwapBuffer() override;
		bool IsOpen() override;
		bool IsFullscreen() override;
		bool IsShowing() override;

		void Show() override;
		void Hide() override;

		void * GetHandle() override;

		void OnResize(UInt32 aWidth, UInt32 aHeight) override;
		void OnDestroy() override;
	protected:
		void OnSetFullscreen() override;
		void OnSetWindowMode() override;
		void CheckState();

	private:
#ifdef _WIN32
		HINSTANCE m_HandleInstance;
		HWND m_WindowHandle;
		HDC m_WindowDeviceContext;
		HGLRC m_OpenGLContext;
		std::string m_WindowClassName;
#endif
	};

	TYPE_DEFINE(Win32Window)
}

#endif