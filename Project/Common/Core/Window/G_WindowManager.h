#ifndef G_WINDOW_MANAGER_H
#define G_WINDOW_MANAGER_H

#include "../Primitives/G_PrimitiveObjects.h"
#include "../Utilities/G_UniqueNumberGenerator.h"
#include <vector>

struct GLFWwindow;

namespace Gem
{
    class Window;
    class WindowHook;

    class WindowManager : public Object
    {
    public:
        WindowManager();
        ~WindowManager();
        static bool init();
        static void deinit();
        static WindowManager * instance();
        static void destroy();

        int createWindow(char * aTitle, int aWidth, int aHeight);
        void destroyWindow(int aHandle);
        void attachHook(WindowHook * aHook, int aHandle);
        void detachHook(WindowHook * aHook, int aHandle);

        int getFocusedWindow();
        inline int getMainWindowID()
        {
            return 0;
        }

        virtual Pointer<Reflection::Type> getType() override;
    private:
        static WindowManager * s_Instance;
        

        GLFWwindow * getCurrentWindow();
        bool hasWindows();
        void update();

        std::vector<Window*> m_Windows;

        static void errorCallback(int aError, const char * aDescription);
        static void keyEvent(GLFWwindow * aWindow, int aKey, int aScanCode, int aAction, int aMods);
        static void mouseEvent(GLFWwindow * aWindow, int aButton, int aAction, int aMods);
        static void mouseMove(GLFWwindow* aWindow, double aX, double aY);
        static void mouseScroll(GLFWwindow * aWindow, double aX, double aY);

        void processKeyEvent(GLFWwindow * aWindow, int aKey, int aScanCode, int aAction, int aMods);
        void processMouseEvent(GLFWwindow * aWindow, int aButton, int aAction, int aMods);
        void processMouseMove(GLFWwindow* aWindow, double aX, double aY);
        void processMouseScroll(GLFWwindow * aWindow, double aX, double aY);

        bool m_HasInitGlew;
        UNG m_WindowHandleGen;

        friend class PoolAllocator;
        friend class Application;
        friend class Input;

    };


}
GEM_CLASS(WindowManager,Object)
#endif