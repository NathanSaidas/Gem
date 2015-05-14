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

	RDEFINE_CLASS(Win32Window,Window)

	Win32Window::Win32Window()
		: Window(),
		m_HandleInstance(NULL),
		m_WindowHandle(NULL),
		m_WindowDeviceContext(NULL),
		m_OpenGLContext(NULL)
	{

	}

	Win32Window::Win32Window(const std::string & aWindowName, HINSTANCE aHandleInstance)
		: Window(aWindowName),
		m_HandleInstance(m_HandleInstance),
		m_WindowHandle(NULL),
		m_WindowDeviceContext(NULL),
		m_OpenGLContext(NULL)
	{

	}

#else
	Win32Window::Win32Window() : Window()
	{

	}
#endif
	Win32Window::~Win32Window()
	{
		Close();
	}

	bool Win32Window::Open()
	{
		//Checks the state, any incorrect settings will get fixed.
		CheckState();

		//If the window is already open, ignore this call.
		if (m_IsOpen)
		{
			return true;
		}
		//Register Window Class
#ifdef _WIN32
		m_WindowClassName = m_Name;
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

		//Create the Window
		RECT windowRect = { 0, 0, m_Width, m_Height };

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

		
		//Setup the OpenGL context
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
		CheckState();
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

	bool Win32Window::IsOpen()
	{
		CheckState();
		return m_IsOpen;
	}

	bool Win32Window::IsFullscreen()
	{
		return m_IsFullscreen;
	}

	bool Win32Window::IsShowing()
	{
		CheckState();
		return m_IsShowing;
	}

	void Win32Window::Show()
	{
		CheckState();
#ifdef _WIN32
		if (m_WindowHandle != NULL)
		{
			if (!m_IsShowing)
			{
				if (ShowWindow(m_WindowHandle, SW_RESTORE) == TRUE)
				{
					m_IsShowing = true;
				}
			}
		}
#endif
	}

	void Win32Window::Hide()
	{
		CheckState();
#ifdef _WIN32
		if (m_WindowHandle != NULL)
		{
			if (m_IsShowing)
			{
				if (ShowWindow(m_WindowHandle, SW_MINIMIZE) == TRUE)
				{
					m_IsShowing = false;
				}
			}
		}
#endif
	}

	void Win32Window::OnSetFullscreen()
	{
		if (!m_IsFullscreen)
		{
#ifdef _WIN32
			DEVMODE newSettings;
			EnumDisplaySettings(NULL, 0, &newSettings);
			newSettings.dmPelsWidth = m_Width;
			newSettings.dmPelsHeight = m_Height;
			newSettings.dmBitsPerPel = 32;
			newSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

			if (ChangeDisplaySettings(&newSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
			{
				m_IsFullscreen = false;
			}
			else
			{
				m_IsFullscreen = true;
			}
#endif
		}
	}

	void Win32Window::OnSetWindowMode()
	{
		if (m_IsFullscreen)
		{
#ifdef _WIN32
			ChangeDisplaySettings(NULL, 0);
			ShowCursor(TRUE);
#endif
		}
	}


	void Win32Window::CheckState()
	{
#ifdef _WIN32
		//If one of the handles is null, everything is null. This could result in leaked memory.
		if (m_WindowHandle == NULL
			|| m_WindowDeviceContext == NULL
			|| m_OpenGLContext == NULL)
		{
			bool leakedMemory = false;

			if (m_WindowHandle != NULL)
			{
				leakedMemory = true;
			}
			m_WindowHandle = NULL;
			if (m_WindowDeviceContext != NULL)
			{
				leakedMemory = true;
			}
			m_WindowDeviceContext = NULL;
			if (m_OpenGLContext != NULL)
			{
				leakedMemory = true;
			}
			m_OpenGLContext = NULL;

			m_WindowClassName = "";

			m_IsOpen = false;
			m_IsShowing = false;

			if (leakedMemory)
			{
				Error error = Error("Error checking window state, possible memory leaked.", ErrorConstants::LEAKED_WINDOW_MEMORY, GET_TRACE(2), "Win32Window::CheckState");
				Debug::Error("Window",error);
			}
		}
		
		//Check if the window exists. (m_IsOpen)
		if (m_WindowHandle != NULL)
		{
			BOOL result = IsWindow(m_WindowHandle);
			if (result == FALSE)
			{
				m_WindowHandle = NULL;
				CheckState();
			}
			else
			{
				//Check if minimized or not.
				result = IsIconic(m_WindowHandle);
				if (result == FALSE && m_IsShowing)
				{
					m_IsShowing = false;
				}
				else if (result == TRUE && !m_IsShowing)
				{
					m_IsShowing = true;
				}
			}
		}
#endif
	}
}