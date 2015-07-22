#include "BaseWindow.h"
#include "Application.h"

//Error handling.
#include "../Core/Debug.h"
#include "../Core/Errors/CoreErrors.h"

using namespace Gem::Debugging;

namespace Gem
{
    RDEFINE_CLASS(BaseWindow, object)
    RDEFINE_PROTECTED_MEMBER(BaseWindow, m_IsFullscreen, bool)
    RDEFINE_PROTECTED_MEMBER(BaseWindow, m_IsResizeabe, bool)
    RDEFINE_PROTECTED_MEMBER(BaseWindow, m_Name, std::string)
    RDEFINE_PROTECTED_MEMBER(BaseWindow, m_Width, UInt32)
    RDEFINE_PROTECTED_MEMBER(BaseWindow, m_Height, UInt32)
    RDEFINE_PROTECTED_MEMBER(BaseWindow, m_RedBits, UInt8)
    RDEFINE_PROTECTED_MEMBER(BaseWindow, m_GreenBits, UInt8)
    RDEFINE_PROTECTED_MEMBER(BaseWindow, m_BlueBits, UInt8)
    RDEFINE_PROTECTED_MEMBER(BaseWindow, m_AlphaBits, UInt8)
    RDEFINE_PROTECTED_MEMBER(BaseWindow, m_DepthBits, UInt8)
    RDEFINE_PROTECTED_MEMBER(BaseWindow, m_StencilBits, UInt8)
    RDEFINE_PROTECTED_MEMBER(BaseWindow, m_Handle, void*)

    BaseWindow::BaseWindow()
    : m_IsFullscreen(false),
    m_IsResizeabe(false),
    m_Width(1024),
    m_Height(768),
    m_RedBits(8),
    m_GreenBits(8),
    m_BlueBits(8),
    m_AlphaBits(8),
    m_DepthBits(24),
    m_StencilBits(8),
    m_Handle(nullptr)
    {
        
    }
    BaseWindow::BaseWindow(const std::string & aBaseWindowName)
    : m_IsFullscreen(false),
    m_IsResizeabe(false),
    m_Name(aBaseWindowName),
    m_Width(1024),
    m_Height(768),
    m_RedBits(8),
    m_GreenBits(8),
    m_BlueBits(8),
    m_AlphaBits(8),
    m_DepthBits(8),
    m_StencilBits(8),
    m_Handle(nullptr)
    {
    
    }
    BaseWindow::~BaseWindow()
    {
        if (IsOpen())
        {
            Close();
        }
    	
    }
    
    
    
    bool BaseWindow::Open()
    {
        //Ensure that width and height are valid.
        
        if (m_Width == 0)
        {
            InvalidArgument error = InvalidArgument("m_Width", GET_TRACE(0), "BaseWindow::Open");
            error.Log("Gem");
            m_Width = 640;
        }

        if (m_Height == 0)
        {
            InvalidArgument error = InvalidArgument("m_Height", GET_TRACE(0), "BaseWindow::Open");
            error.Log("Gem");
            m_Height = 480;
        }

        //Create the window.
        Application* app = Application::GetInstance();
        if (app != nullptr)
        {
            return app->OpenWindow(this);
        }
        return false;
    }
    bool BaseWindow::Close()
    {
        //Close the window.
        Application* app = Application::GetInstance();
        if (app != nullptr)
        {
            return app->CloseWindow(this);
        }
    	return false;
    }
    void BaseWindow::SwapBuffer()
    {
        //Ignore this call if the window is not open.
        if (!IsOpen())
        {
            return;
        }

        Application* app = Application::GetInstance();
        if (app != nullptr)
        {
            return app->SwapBuffer(this);
        }
    }
   
    
    void BaseWindow::OnResize(UInt32 aWidth, UInt32 aHeight)
    {
    	m_Width = aWidth;
    	m_Height = aHeight;
    }
    
    void BaseWindow::OnDestroy()
    {
    
    }

    void BaseWindow::SetFullscreen(bool aFullscreen)
    {
        if (aFullscreen != m_IsFullscreen)
        {
            m_IsFullscreen = aFullscreen;
            if (IsOpen())
            {
                Application* app = Application::GetInstance();
                if (app != nullptr)
                {
                    app->ChangeFullscreenWindow(this);
                }
            }
        }
    }
    
    void BaseWindow::UpdateWidowSize()
    {
        //Ignore this call if the window is not open.
        if (!IsOpen())
        {
            return;
        }
        Application* app = Application::GetInstance();
        if (app != nullptr)
        {
            app->SetWindowSize(this);
        }
    }
}