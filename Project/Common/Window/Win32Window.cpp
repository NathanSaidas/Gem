#include "Win32Window.h"
#include "../Core/ErrorConstants.h"
#include "../Core/Debug.h"

using namespace Gem::Debugging;

namespace Gem
{



#ifdef _WIN32

	LRESULT CALLBACK WndProc(HWND aHandle, UINT aMessage, WPARAM aWParam, LPARAM aLParam)
	{
		switch (aMessage)
		{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
			
		}

		return DefWindowProc(aHandle, aMessage, aWParam, aLParam);
	}

	Win32Window::Win32Window(const std::string & aWindowName, HINSTANCE aHandleInstance)
		: m_WindowName(aWindowName),
		m_HandleInstance(m_HandleInstance),
		m_WindowHandle(NULL),
		m_WindowDeviceContext(NULL),
		m_OpenGLContext(NULL),
		m_WindowWidth(1024),
		m_WindowHeight(768),
		m_RedBits(8),
		m_GreenBits(8),
		m_BlueBits(8),
		m_AlphaBits(8),
		m_DepthBits(8),
		m_StencilBits(8),
		m_IsFullscreen(false),
		m_IsOpen(false)
	{

	}

#endif
	Win32Window::~Win32Window()
	{
		Close();
	}

	bool Win32Window::Open()
	{
		if (m_IsOpen)
		{
			return true;
		}
		//Register Window Class

#ifdef _WIN32
		m_WindowClassName = m_WindowName;
		WNDCLASS wc;
		ZeroMemory(&wc, sizeof(WNDCLASS));
		wc.style = 0;
		wc.lpfnWndProc = WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = GetModuleHandle(NULL);
		wc.hIcon = NULL;
		wc.hCursor = 0;
		wc.hbrBackground = 0;
		wc.lpszMenuName = NULL;
		wc.lpszClassName = m_WindowClassName.c_str();

		if (!RegisterClass(&wc))
		{
			Error error = Error("Failed to register window class", ErrorConstants::FAILED_WINDOW_CREATION, GET_TRACE(2), "Win32Window::Open");
			Debug::Error("Window",error);
			return false;
		}

		RECT windowRect = { 0, 0, m_WindowWidth, m_WindowHeight };

		DWORD exStyle;
		DWORD style;

		ShowCursor(TRUE);

		exStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		style = WS_CAPTION | WS_SYSMENU | WS_VISIBLE | WS_MINIMIZEBOX;

		AdjustWindowRectEx(&windowRect, style, FALSE, exStyle);

		UINT width = windowRect.right - windowRect.left;
		UINT height = windowRect.bottom - windowRect.top;

		UINT x = GetSystemMetrics(SM_CXSCREEN) / 2 - width / 2;
		UINT y = GetSystemMetrics(SM_CYSCREEN) / 2 - height / 2;

		m_WindowHandle = CreateWindowEx(exStyle,
			m_WindowClassName.c_str(),
			m_WindowClassName.c_str(),
			WS_CLIPSIBLINGS | WS_CLIPCHILDREN | style,
			x,
			y,
			width,
			height,
			NULL,
			NULL,
			GetModuleHandle(NULL),
			this);
	
		if (m_WindowHandle == NULL)
		{
			Error error = Error("Failed to create window", ErrorConstants::FAILED_WINDOW_CREATION, GET_TRACE(2), "Win32Window::Open");
			Debug::Error("Window", error);
			return false;
		}

		

		PIXELFORMATDESCRIPTOR pixelFormatDescriptor;
		ZeroMemory(&pixelFormatDescriptor, sizeof(PIXELFORMATDESCRIPTOR));
		pixelFormatDescriptor.nSize				= sizeof(PIXELFORMATDESCRIPTOR);
		pixelFormatDescriptor.nVersion			= 1;
		pixelFormatDescriptor.dwFlags			= PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pixelFormatDescriptor.cColorBits		= 32;
		pixelFormatDescriptor.cRedBits			= static_cast<BYTE>(m_RedBits);
		pixelFormatDescriptor.cGreenBits		= static_cast<BYTE>(m_GreenBits);
		pixelFormatDescriptor.cBlueBits			= static_cast<BYTE>(m_BlueBits);
		pixelFormatDescriptor.cAlphaBits		= static_cast<BYTE>(m_AlphaBits);
		pixelFormatDescriptor.cDepthBits		= static_cast<BYTE>(m_DepthBits);
		pixelFormatDescriptor.cStencilBits		= static_cast<BYTE>(m_StencilBits);
		pixelFormatDescriptor.iPixelType		= PFD_TYPE_RGBA;

		m_WindowDeviceContext = GetDC(m_WindowHandle);
		if (m_WindowDeviceContext == NULL)
		{
			Error error = Error("Failed to get device context", ErrorConstants::FAILED_WINDOW_CREATION, GET_TRACE(3), "Win32Window::Open");
			Debug::Error("Window", error);
			DestroyWindow(m_WindowHandle);
			UnregisterClass(m_WindowClassName.c_str(), GetModuleHandle(NULL));
			return false;
		}

		int pixelFormat = ChoosePixelFormat(m_WindowDeviceContext, &pixelFormatDescriptor);
		if (pixelFormat == 0)
		{
			Error error = Error("Failed to choose pixel format.", ErrorConstants::FAILED_WINDOW_CREATION, GET_TRACE(3), "Win32Window::Open");
			Debug::Error("Window", error);

			ReleaseDC(m_WindowHandle, m_WindowDeviceContext);
			m_WindowDeviceContext = NULL;

			DestroyWindow(m_WindowHandle);
			UnregisterClass(m_WindowClassName.c_str(), GetModuleHandle(NULL));
			return false;
		}

		if(SetPixelFormat(m_WindowDeviceContext,pixelFormat,&pixelFormatDescriptor) == FALSE)
		{
			Error error = Error("Failed to set pixel format.", ErrorConstants::FAILED_WINDOW_CREATION, GET_TRACE(2), "Win32Window::Open");
			Debug::Error("Window", error);
			ReleaseDC(m_WindowHandle, m_WindowDeviceContext);
			m_WindowDeviceContext = NULL;

			DestroyWindow(m_WindowHandle);
			UnregisterClass(m_WindowClassName.c_str(), GetModuleHandle(NULL));
			return false;
		}

		m_OpenGLContext = wglCreateContext(m_WindowDeviceContext);
		if(m_OpenGLContext == NULL)
		{
			Error error = Error("Failed to create OpenGL Context", ErrorConstants::FAILED_WINDOW_CREATION, GET_TRACE(3), "Win32Window::Open");
			Debug::Error("Window", error);

			ReleaseDC(m_WindowHandle, m_WindowDeviceContext);
			m_WindowDeviceContext = NULL;

			DestroyWindow(m_WindowHandle);
			UnregisterClass(m_WindowClassName.c_str(), GetModuleHandle(NULL));
			return false;
		}

		if (wglMakeCurrent(m_WindowDeviceContext, m_OpenGLContext) == FALSE)
		{
			Error error = Error("Failed to make this windows context the current.", ErrorConstants::FAILED_WINDOW_CREATION, GET_TRACE(2), "Win32Window::Open");
			Debug::Error("Window", error);

			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(m_OpenGLContext);
			m_OpenGLContext = NULL;

			ReleaseDC(m_WindowHandle, m_WindowDeviceContext);
			m_WindowDeviceContext = NULL;

			DestroyWindow(m_WindowHandle);
			UnregisterClass(m_WindowClassName.c_str(), GetModuleHandle(NULL));

			return false;
		}

		ShowWindow(m_WindowHandle, SW_SHOW);


		m_IsOpen = true;
		return true;
#else
		return false;
#endif
	}

	bool Win32Window::Close()
	{
		if (!m_IsOpen)
		{
			return true;
		}
#ifdef _WIN32

		if (!IsWindow(m_WindowHandle))
		{
			return false;
		}

		if (m_IsFullscreen)
		{
			ChangeDisplaySettings(NULL, 0);
			ShowCursor(TRUE);
		}

		if(m_OpenGLContext != NULL)
		{
			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(m_OpenGLContext);
			m_OpenGLContext = NULL;
		}

		if (m_WindowDeviceContext != NULL)
		{
			ReleaseDC(m_WindowHandle, m_WindowDeviceContext);
			m_WindowDeviceContext = NULL;
		}

		DestroyWindow(m_WindowHandle);
		UnregisterClass(m_WindowClassName.c_str(), GetModuleHandle(NULL));

		m_IsOpen = false;

		return true;
#else
		return false;
#endif

		
	}

	void Win32Window::SwapBuffer()
	{
#ifdef _WIN32
		SwapBuffers(m_WindowDeviceContext);
#else

#endif
	}
}