#include "../../Common/Engine.h"
#include <glew-1.10.0\include\GL\glew.h>
#include <GLFW_3.0.4\include\GLFW\glfw3.h>
#include <Windows.h>


using namespace Gem;

int WINAPI WinMain(HINSTANCE aCurrentInstance, HINSTANCE aPreviousInstance, LPSTR aCommandLineArgs, int aShowCommand)
{
    return Application::Execute();
    ////Create an application
    //GLFWwindow* window;
    //if (!glfwInit())
    //{
    //    return EXIT_FAILURE;
    //}
    //
    //window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    //if (!window)
    //{
    //    glfwTerminate();
    //    return EXIT_FAILURE;
    //}
    //
    //glfwMakeContextCurrent(window);
    //
    //while (!glfwWindowShouldClose(window))
    //{
    //    glfwSwapBuffers(window);
    //    glfwPollEvents();
    //}
    //
    //glfwTerminate();


	//try
	//{
	//	int code = Gem::Application::Execute(
	//		"Test App", 
	//		Gem::ApplicationType::Window, 
	//		aCurrentInstance, 
	//		new AppHandler() );
	//	system("pause");
	//	return code;
	//}
	//catch (...)
	//{
	//	MessageBox(NULL, "Failed", "Error", MB_OK | MB_ICONEXCLAMATION);
	//}
	//system("pause");
}
