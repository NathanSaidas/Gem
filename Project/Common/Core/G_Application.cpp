#include "G_Application.h"
#include "Reflection\G_Reflection.h"
#include "Utilities\G_Debug.h"
#include "Memory\G_Memory.h"
#include "Window\G_WindowHook.h"
#include "Window\G_WindowManager.h"
#include "Input\G_Input.h"
#include "Utilities\G_Utilities.h"
#include "Utilities\G_Time.h"
#include <GLFW\glfw3.h>
#include "Entity Component\G_GameObjectManager.h"
#include "Window\G_SceneManager.h"

namespace Gem
{
    using namespace Reflection;

    Application * Application::s_Instance = nullptr;
    Application * Application::instance()
    {
        if(s_Instance == nullptr)
        {
            s_Instance = Memory::instantiate<Application>();
        }
        return s_Instance;
    }
    void Application::destroy()
    {
        if(s_Instance != nullptr)
        {
            s_Instance = Memory::destroy<Application>(s_Instance);
        }
    }
    Application::Application()
    {
		m_ShouldQuit = false;
    }
    Application::~Application()
    {

    }
    void Application::execute()
    {
        init();
        run();
        deinit();
    }
	void Application::exit()
	{
		if (s_Instance != nullptr)
		{
			s_Instance->m_ShouldQuit = true;
		}
	}
		
    //called before run
    bool Application::init()
    {
        Runtime::instance();
		Debug::instance();
        //Initialize things
        if(WindowManager::init() == false)
        {
            return false;
        }


        return true;
    }
    //called after init
    bool Application::run()
    {
        log("Application running");
        WindowManager * windowManager = WindowManager::instance();
        Input * input = Input::instance();
        GameObjectManager * gom = GameObjectManager::instance();
        
        
        int mainWindowHandle = windowManager->createWindow("Main Window",1280,768);
        //int inspectorWindow = windowManager->createWindow("Inspector",400,800);
        WindowHook * mainHook = Memory::instantiate<WindowHook>();
        windowManager->attachHook(mainHook,mainWindowHandle);
        
        
        if(mainWindowHandle == -1)
        {
            return false;
        }
        
        //While there is windows update
        //Update them
        while(windowManager->hasWindows() && m_ShouldQuit == false)
        {
            //Calculate delta time
            float currentTime = glfwGetTime();
            float lastTime = Time::s_CurrentTime;
            Time::s_DeltaTime = currentTime - lastTime;
            Time::s_CurrentTime = currentTime;

            //Update inputs
            input->update();
            //Update the windows (Poll Events and swap buffers)
            gom->update();
            windowManager->update();
            //Update all the game objects
            //Make Render Calls on each window
            gom->processDestroyRequests();
            gom->processSceneUnload();
            

			
            
        
        }
		gom->onApplicationQuit();

        windowManager->destroyWindow(mainWindowHandle);
        
        mainHook = Memory::destroy<WindowHook>(mainHook);





        return true;
    }
    //called after run
    bool Application::deinit()
    {
        SceneManager::destroy();
        GameObjectManager::destroy();
        Input::destroy();
        WindowManager::destroy();
        WindowManager::deinit();
		Debug::destroy();
        Runtime::destroy();
        return true;
    }

    Pointer<Reflection::Type> Application::getType()
    {
        return typeOf("Application");
    }
    
}