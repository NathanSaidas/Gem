#ifndef GEM_BASE_WINDOW_H
#define GEM_BASE_WINDOW_H

//============================================================
// Date:			May,	13,		2015
// Description:		Contains the base class for all windows.
//============================================================

#pragma  region CHANGE LOG
// -- Nathan Hanlan	- 
#pragma endregion

#include "../Core/Core.h"
#include <string>


namespace Gem
{
    class BaseWindow : public object
    {
        RDECLARE_CLASS(BaseWindow)
    public:
        BaseWindow();
        BaseWindow(const std::string & aWindowName);
        ~BaseWindow();
        
        
        /**
        * Creates and opens the window. 
        * @return Returns true if the window opened successfully.
        */
        bool Open();
        /**
        * Destroys and closes the window.
        * @return Returns true if the window was closed successfully.
        */
        bool Close();
        /**
        * Swaps the windows front and back buffers.
        */
        void SwapBuffer();
        
        
        
        /**
        * Call to resize the window, or notify that it has been resized.
        */
        virtual void OnResize(UInt32 aWidth, UInt32 aHeight);
        
        /**
        * Call to notify the window about it being destroyed.
        */
        virtual void OnDestroy();
        
        
        
#pragma region ACCESSORS
        
        /**
        * Checks if the window is open. Check with the OS to see if the resources still exist.
        * @return Returns true if the window is open, false otherwise.
        */

        inline bool IsOpen() const
        {
            return m_Handle != nullptr;
        }
        /**
        * Check if the window is in fullscreen. Check with the OS to see if the window is actually
        * in a fullscreen.
        * @return Returns true if the window is fullscreen, false otherwise.
        */
        inline bool IsFullscreen() const
        {
            return m_IsFullscreen;
        }


        /**
        * Returns a handle to the window instance. This is used for comparing windows.
        * @return Returns a handle to the window instance.
        */
        inline void * GetHandle() const
        {
            return m_Handle;
        }

        bool IsResizeable() const
        {
            return m_IsResizeabe;
        }

        std::string GetName() const
        {
        	return m_Name;
        }
        
        UInt32 GetWidth() const
        {
        	return m_Width;
        }
        
        UInt32 GetHeight() const
        {
        	return m_Height;
        }
        
        UInt8 GetRedBits() const
        {
        	return m_RedBits;
        }
        
        UInt8 GetGreenBits() const
        {
        	return m_GreenBits;
        }
        
        UInt8 GetBlueBits() const
        {
        	return m_BlueBits;
        }
        
        UInt8 GetAlphaBits() const
        {
        	return m_AlphaBits;
        }
        
        UInt8 GetDepthBits() const
        {
        	return m_DepthBits;
        }
        
        UInt8 GetStencilBits() const
        {
        	return m_StencilBits;
        }
        
        
        
        void SetFullscreen(bool aFullscreen);
        //{
        //	if (aFullscreen != m_IsFullscreen)
        //	{
        //		m_IsFullscreen = aFullscreen;
        //		if (aFullscreen == true)
        //		{
        //			
        //		}
        //		else
        //		{
        //			
        //		}
        //	}
        //}

        void SetIsResizeable(bool aResizeable)
        {
            if (!IsOpen())
            {
                m_IsResizeabe = aResizeable;
            }
        }
        
        void SetName(const std::string & aName)
        {
            if (!IsOpen())
            {
                m_Name = aName;
            }
        }

        void SetWidth(UInt32 aWidth)
        {
        	m_Width = aWidth;
            if (IsOpen())
            {
                UpdateWidowSize();
            }
        }
        
        void SetHeight(UInt32 aHeight)
        {
        	m_Height = aHeight;
            if (IsOpen())
            {
                UpdateWidowSize();
            }
        }
        
        void SetRedBits(UInt8 aRedbits)
        {
        	m_RedBits = aRedbits;
        }
        
        void SetGreenBits(UInt8 aGreenBits)
        {
        	m_GreenBits = aGreenBits;
        }
        
        void SetBlueBits(UInt8 aBlueBits)
        {
        	m_BlueBits = aBlueBits;
        }
        
        void SetAlphaBits(UInt8 aAlphaBits)
        {
        	m_AlphaBits = aAlphaBits;
        }
        
        void SetDepthBits(UInt8 aDepthBits)
        {
        	m_DepthBits = aDepthBits;
        }
        
        void SetStencilBits(UInt8 aStencilBits)
        {
        	m_StencilBits = aStencilBits;
        }
        
#pragma endregion
    protected:
        RDECLARE_PROTECTED_MEMBER(BaseWindow, m_IsFullscreen)
        bool m_IsFullscreen;
        RDECLARE_PROTECTED_MEMBER(BaseWindow, m_IsResizeabe)
        bool m_IsResizeabe;
        
        RDECLARE_PROTECTED_MEMBER(BaseWindow, m_Name)
        std::string m_Name;
        RDECLARE_PROTECTED_MEMBER(BaseWindow, m_Width)
        UInt32 m_Width;
        RDECLARE_PROTECTED_MEMBER(BaseWindow, m_Height)
        UInt32 m_Height;
        
        RDECLARE_PROTECTED_MEMBER(BaseWindow, m_RedBits)
        UInt8 m_RedBits;
        RDECLARE_PROTECTED_MEMBER(BaseWindow, m_GreenBits)
        UInt8 m_GreenBits;
        RDECLARE_PROTECTED_MEMBER(BaseWindow, m_BlueBits)
        UInt8 m_BlueBits;
        RDECLARE_PROTECTED_MEMBER(BaseWindow, m_AlphaBits)
        UInt8 m_AlphaBits;
        RDECLARE_PROTECTED_MEMBER(BaseWindow, m_DepthBits)
        UInt8 m_DepthBits;
        RDECLARE_PROTECTED_MEMBER(BaseWindow, m_StencilBits)
        UInt8 m_StencilBits;

        RDECLARE_PROTECTED_MEMBER(BaseWindow, m_Handle)
        void* m_Handle;


        void UpdateWidowSize();
        
    };

    TYPE_DEFINE(BaseWindow)
}

#endif