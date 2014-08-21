#include "G_Application.h"
#include "Reflection\G_Reflection.h"
#include "Memory\G_Memory.h"
#include "Window\G_WindowHook.h"
#include "Window\G_WindowManager.h"
#include "Input\G_Input.h"
#include "Utilities\G_Utilities.h"
#include "Utilities\G_Time.h"
#include <GLFW\glfw3.h>

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

    //called before run
    bool Application::init()
    {
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
        while(windowManager->hasWindows())
        {
            float currentTime = glfwGetTime();
            float lastTime = Time::s_CurrentTime;
            Time::s_DeltaTime = currentTime - lastTime;
            Time::s_CurrentTime = currentTime;

            input->update();
            windowManager->update();
            
            
        
        }
        
        windowManager->destroyWindow(mainWindowHandle);
        
        mainHook = Memory::destroy<WindowHook>(mainHook);





        return true;
    }
    //called after run
    bool Application::deinit()
    {
        Input::destroy();
        WindowManager::destroy();
        WindowManager::deinit();

        return true;
    }

    Type Application::getType()
    {
        return TypeFactory::create("Application",TypeID::APPLICATION,sizeof(Application));
    }
    Type Application::baseType()
    {
        return Object::getType();
    }
    Type * Application::instanceOf(int & aCount)
    {
        int prevCount = 0;
        Type * prevTypes = Object::instanceOf(prevCount);
        Type base = baseType();
        Type * types = TypeFactory::create(base,prevCount +1,prevTypes,prevCount);
        return types;
    }
}