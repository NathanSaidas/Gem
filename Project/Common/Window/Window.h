#ifndef GEM_WINDOW_H
#define GEM_WINDOW_H

//============================================================
// Date:			May,	13,		2015
// Description:		Contains the base class for all windows.
//============================================================

#pragma  region CHANGE LOG
// -- Nathan Hanlan	- 
#pragma endregion

#include "../Core/GemAPI.h"
#include "../Core/BasicTypes.h"
#include <string>

namespace Gem
{
	class Window;
	template class GEM_API Reflection::MetaObject<Window>;

	class GEM_API Window : public object
	{
		RDECLARE_CLASS(Window)
	public:
		Window();
		Window(const std::string & aWindowName);
		~Window();


		/**
		* Creates and opens the window. 
		* @return Returns true if the window opened successfully.
		*/
		virtual bool Open();
		/**
		* Destroys and closes the window.
		* @return Returns true if the window was closed successfully.
		*/
		virtual bool Close();
		/**
		* Swaps the windows front and back buffers.
		*/
		virtual void SwapBuffer();

		/**
		* Checks if the window is open. Check with the OS to see if the resources still exist.
		* @return Returns true if the window is open, false otherwise.
		*/

		virtual bool IsOpen();
		/**
		* Check if the window is in fullscreen. Check with the OS to see if the window is actually
		* in a fullscreen.
		* @return Returns true if the window is fullscreen, false otherwise.
		*/
		virtual bool IsFullscreen();

		/**
		* Check to see if the window is showing. Check with the OS to see if the window is showing.
		* @return Returns true if the window is showing, false otherwise.
		*/
		virtual bool IsShowing();
		
		/**
		* Makes the window visible.
		*/
		virtual void Show();
		
		/**
		* Hides the window making it not visible.
		*/
		virtual void Hide();

		/**
		* Returns a handle to the window instance. This is used for comparing windows.
		* @return Returns a handle to the window instance.
		*/
		virtual void * GetHandle();

		/**
		* Call to resize the window, or notify that it has been resized.
		*/
		virtual void OnResize(UInt32 aWidth, UInt32 aHeight);

		/**
		* Call to notify the window about it being destroyed.
		*/
		virtual void OnDestroy();

		
		
#pragma region ACCESSORS

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

		

		void SetFullscreen(bool aFullscreen)
		{
			if (aFullscreen != m_IsFullscreen)
			{
				m_IsFullscreen = aFullscreen;
				if (aFullscreen == true)
				{
					OnSetFullscreen();
				}
				else
				{
					OnSetWindowMode();
				}
			}
		}

		void SetName(std::string aName)
		{
			m_Name = aName;
		}

		void SetName(const char * aName)
		{
			m_Name = aName;
		}

		void SetWidth(UInt32 aWidth)
		{
			m_Width = aWidth;
		}

		void SetHeight(UInt32 aHeight)
		{
			m_Height = aHeight;
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
		bool m_IsFullscreen;
		bool m_IsOpen;
		bool m_IsShowing;

		std::string m_Name;
		UInt32 m_Width;
		UInt32 m_Height;

		UInt8 m_RedBits;
		UInt8 m_GreenBits;
		UInt8 m_BlueBits;
		UInt8 m_AlphaBits;
		UInt8 m_DepthBits;
		UInt8 m_StencilBits;

		virtual void OnSetFullscreen();
		virtual void OnSetWindowMode();


	};

	TYPE_DEFINE(Window)
}


#endif