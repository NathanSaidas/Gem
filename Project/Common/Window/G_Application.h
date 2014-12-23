#ifndef G_APPLICATION_H
#define G_APPLICATION_H

#include "../G_Object.h"

namespace Gem
{
    class Application : public object
    {
		G_CLASS_DEF(Application)
    public:
        Application();
        ~Application();
        static Application * Instance();
        static void Destroy();


        //called once on main thread
        int Execute();
		static void Exit(int aStatus = 0);
		static void PauseOnExit();

    private:

		void OnApplicationStart();
		void OnExecute();
		void OnApplicationClose();

		//Gets called when the main window resizes.
		void OnWindowResize(float aWidth, float aHeight);

        static Application * s_Instance;
        
		bool m_PauseOnExit;
		bool m_ShouldQuit;
		int m_ExitStatus;
        friend class PoolAllocator;
		friend class WindowManager;
    };
}


#endif