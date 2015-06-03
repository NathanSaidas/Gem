#include <stdlib.h>
#include <utility>
#include <memory>
#include <chrono>
#include <iostream>
#include "../../Common/Engine.h"
#include "../../Common/Gameplay/AppHandler.h"
#include <Windows.h>
#include <functional>

using namespace Gem;

int Hash(int v)
{
	v = v << 10;
	v = v ^ 6;
	return v;
}

int Hash(const std::string & aString)
{
	int n = 0;

	for (int i = 0; i < aString.size(); i++)
	{
		n += aString[i];
	}


	return n / aString.size();
}



int WINAPI WinMain(HINSTANCE aCurrentInstance, HINSTANCE aPreviousInstance, LPSTR aCommandLineArgs, int aShowCommand)
{

	try
	{
		int code = Gem::Application::Execute(
			"Test App", 
			Gem::ApplicationType::Window, 
			aCurrentInstance, 
			new AppHandler() );
		system("pause");
		return code;
	}
	catch (...)
	{
		MessageBox(NULL, "Failed", "Error", MB_OK | MB_ICONEXCLAMATION);
	}
	system("pause");
	return EXIT_FAILURE;
}