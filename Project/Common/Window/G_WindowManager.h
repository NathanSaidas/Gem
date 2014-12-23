#ifndef G_WINDOW_MANAGER_H
#define G_WINDOW_MANAGER_H

#include "../Math/G_Math.h"
#include "../G_Object.h"
//#include "../Utilities/G_UniqueNumberGenerator.h"
#include <vector>


struct GLFWwindow;

namespace Gem
{
    const extern int INVALID_WINDOW_HANDLE;

    class Window;
    class WindowHook;

    class WindowManager : public object
    {
		G_CLASS_DEF(WindowManager)
    public:
        WindowManager();
        ~WindowManager();
        static bool Init();
        static void DeInit();
        static WindowManager * Instance();
        static void Destroy();

        int CreateWindow(char * aTitle, int aWidth, int aHeight);
        void DestroyWindow(int aHandle);
        void AttachHook(WindowHook * aHook, int aHandle);
        void DetachHook(WindowHook * aHook, int aHandle);

        int GetFocusedWindow();
        inline int GetMainWindowID()
        {
            return 0;
        }

		Vector2 GetWindowSize(int aHandle);
    private:
        static WindowManager * s_Instance;
        

        GLFWwindow * GetCurrentWindow();
        bool HasWindows();
        void Update();

        std::vector<Window*> m_Windows;

        static void ErrorCallback(int aError, const char * aDescription);
        static void KeyEvent(GLFWwindow * aWindow, int aKey, int aScanCode, int aAction, int aMods);
        static void MouseEvent(GLFWwindow * aWindow, int aButton, int aAction, int aMods);
        static void MouseMove(GLFWwindow* aWindow, double aX, double aY);
        static void MouseScroll(GLFWwindow * aWindow, double aX, double aY);
		static void WindowResize(GLFWwindow * aWindow, int aWidth, int aHeight);

        void ProcessKeyEvent(GLFWwindow * aWindow, int aKey, int aScanCode, int aAction, int aMods);
        void ProcessMouseEvent(GLFWwindow * aWindow, int aButton, int aAction, int aMods);
        void ProcessMouseMove(GLFWwindow* aWindow, double aX, double aY);
        void ProcessMouseScroll(GLFWwindow * aWindow, double aX, double aY);
		void ProcessWindowResize(GLFWwindow * aWindow, int aWidth, int aHeight);
        bool m_HasInitGlew;
        //UNG m_WindowHandleGen;

        friend class PoolAllocator;
        friend class Application;
        friend class Input;

    };


}
#endif