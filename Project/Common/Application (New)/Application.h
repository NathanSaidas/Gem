#ifndef GEM_APPLICATION_H
#define GEM_APPLICATION_H

//============================================================
// Date:			July,	21,		2015
// Description:		The root file for the Application Class. Contains the Application Class as well as typedefs and explicit exporting/importing.
//============================================================

#pragma  region CHANGE LOG
// -- Nathan Hanlan	- Implemented base features of class Application.
#pragma endregion

#include <vector>
#include <string>

#include "../Core/Core.h"

namespace Gem
{
    class BaseWindow;

    //FORCE_EXPORT_META(Application);

    /**
    * This class manages...
    * - Windows
    * - Application State (Running/Not Running)
    * - Threads
    * - Scenes
    */
    class Application : public object
    {
        RDECLARE_CLASS(Application)
    public:

        /**
        * This is the main function that will start the execution for the entire application.
        * Start Subsystems (Memory, Reflection)
        * Create Main Window ( Initialize Graphics, Initialize Entity Component, Initialize Input, Initialize Time )
        * Game Loop (Update Time, Clear Screen, Update Objects, Render Objects, Swap Buffers, Clean Up Memory)
        */
        static SInt32 Execute();
        static bool ShouldWindowClose(BaseWindow* aWindow);

        static Application* GetInstance();
        static void Terminate();


    private:
        static Application* s_Instance;
        Application();
        ~Application();

        /** The main window of the application. If this window is closed then the application quits.*/
        BaseWindow* m_MainWindow;
        /** A collection of other windows created.*/
        std::vector<BaseWindow*> m_Windows;

        //Methods for BaseWindow to invoke specifically.
        //TODO(Nathan): For thread safety create a command / processor design to create tasks for the Application to process on the main thread
        // to handle window calls as they should all be called from the main thread. If the call is made from the main thread do not create a process.
        // execute the method as normally.

        /**
        * This method takes a window object and creates a window with the operating system setting the m_Handle member.
        * @param aWindow The window to open.
        * @return Returns true if the window was opened, returns false if there was a problem opening the window.
        */
        bool OpenWindow(BaseWindow* aWindow);
        /**
        * This method takes a window object and destroys a window associated with the m_Handle member.
        * @param aWindow The widow to close.
        * @return Returns true if the window was closed, returns false if there was a problem closing the window.
        */
        bool CloseWindow(BaseWindow* aWindow);
        /**
        * This method will set the size of the window based on the width and height of the window.
        * Width and height are retrieved from the window object.
        * @param aWindow The window whose size to update.
        * @return Returns true if the window had their size changed.
        */
        bool SetWindowSize(BaseWindow* aWindow);
        /**
        * This method will set the window to be a fullscreen window. This only applies to the window which is equal to the m_MainWindow.
        * The fullscreen flag is retrieved from the window object.
        * @param aWindow The window to change 
        * @return Returns true if the window could successfully change fullscreen mode.
        */
        bool ChangeFullscreenWindow(BaseWindow* aWindow);

        /**
        * This method will swap the buffers of the window.
        * @param aWindow The window whose buffers to swap.
        */
        void SwapBuffer(BaseWindow* aWindow);

        friend BaseWindow;
        friend Reflection::MetaObject<Application>;
    };

    TYPE_DEFINE(Application)

}

#endif