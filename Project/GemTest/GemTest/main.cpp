#include <stdlib.h>
#include <utility>
#include <memory>
#include <chrono>
#include <iostream>
#include "../../Common/Engine.h"
#include <Windows.h>

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

struct GOData
{
	std::string name;
	std::string tag;
	UInt32 renderMask;
	UInt32 physicsMask;
	bool isActive;
	Vector3 scale;
	Vector3 position;
	Quaternion rotation;

	std::string GetData(Array<UInt8> & aBytes);
};

std::string GOData::GetData(Array<UInt8> & aBytes)
{
	BinaryFormatter formatter;
	Stream stream;

	formatter.Serialize(scale, stream);
	formatter.Serialize(position, stream);
	formatter.Serialize(rotation, stream);
	formatter.Serialize(name, stream);

	aBytes = stream.ToBytes();
	return stream.ToString();
}

void RenderThread(Thread & aThread)
{
	for (int i = 0; i < 10; i++)
	{
		Debugging::Debug::Log("Render Thread", "Slow Render Frame");
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

void OnStart();
void OnStop();
void OnInitialized();
void OnTestHash(Gem::Thread & aThread);

void OnStart()
{
	Application::RegisterEvent(ApplicationEventType::OnStop, OnStop);
	Application::RegisterEvent(ApplicationEventType::OnSystemsInitialized, OnInitialized);
	

	Gem::ApplicationType appType = Gem::Application::GetApplicationType();
	Gem::Debugging::Debug::LogFormat("Test", nullptr, "Starting Application Type %s", appType.ToString().c_str());
}

void OnStop()
{
	Gem::Debugging::Debug::LogFormat("Test", nullptr, "Stopping Application %s", Gem::Application::GetApplicationName().c_str());
}

void OnTestHash(Gem::Thread & aThread)
{

	


	std::string names[] = 
	{
		"Applering",
		"Flitterflower",
		"Clearleaf",
		"Sweetcheeks",
		"Sugargleam",
		"Silvergaze"
	};


	for (int i = 0; i < 6; i++)
	{
		int a = Hash(names[i]);
		int b = Hash(a);
		int c = Hash(i);
		int d = Hash(a >> i);

		Debugging::Debug::LogFormat("Hash", nullptr, "Hashing Name %s\na = %d\nb = %d\nc = %d\nd = %d", names[i].c_str(), a, b, c, d);
	}

	return;

	int elements = 100;
	int collisions = 0;

	int * hashArray = new int[elements];

	for (int i = 0; i < elements; i++)
	{
		hashArray[i] = Hash(i);
	}

	for (int i = 0; i < elements; i++)
	{
		for (int j = 0; j < elements; j++)
		{
			if (i == j)
			{
				continue;
			}
			if (hashArray[i] == hashArray[j])
			{
				collisions++;
			}
		}
	}

	Debugging::Debug::LogFormat("Hash", nullptr, "Collisions = %d", collisions);
	

}


void OnInitialized()
{
	Application::StartThread(OnTestHash);

	bool read = true;

	//Params
	std::string name = "Chelsea";
	std::string tag = "Alien";
	UInt32 renderMask = 0;	//Ignore - Default
	UInt32 physicsMask = 0; //Ignore - Default
	bool isActive = true;
	Vector3 scale = Vector3::One();
	Vector3 position = Vector3(425.0f, 75.6f, -184.0f);
	Quaternion rotation = Quaternion::Identity();

	if (!read)
	{
		FILE * file = fopen("testFile.txt", "w");

		if (file == NULL)
		{
			return;
		}

		//Instruction
		const char * instruction = "add gameobject";
		SInt32 index_id = 0;
		SInt32 parent_index_id = -1;
		
		int param_count = 3;

		fprintf(file, "%s [%d] [%d]\n", instruction, index_id, parent_index_id);

		fprintf(file, "count = %u {\n", param_count);


		std::string param_name = "name";
		std::string param_typename = "std::string";
		std::string param_value = name;
		fprintf(file, "%s = (%s)%s\n", param_name.c_str(), param_typename.c_str(), param_value.c_str());

		param_name = "tag";
		param_typename = "std::string";
		param_value = tag;
		fprintf(file, "%s = (%s)%s\n", param_name.c_str(), param_typename.c_str(), param_value.c_str());

		param_name = "position";
		param_typename = "Vector3";

		{
			BinaryFormatter formatter;
			Stream stream;
			formatter.Serialize(position, stream);
			param_value = stream.ToString();
		}
		fprintf(file, "%s = (%s)%s\n", param_name.c_str(), param_typename.c_str(), param_value.c_str());


		fprintf(file, "}\n");

		fclose(file);
	}
	else
	{

		Type type = Reflection::Runtime::TypeOf("GameObject");
		Reflection::Member * methodInfo = type.GetMethodInfo("OnPreSerializeData");
		
		//Instruction
		char buffer[100];
		char buffer2[100];
		SInt32 index_id = 346546;
		SInt32 parent_index_id = -34;

		//param
		int param_count = 3;
		char param_name_buffer[50];
		char param_typename_buffer[50];
		char param_value_buffer[1024];

		FILE * file = fopen("testFile.txt", "r");

		fgets(buffer, 100, file);

		int error = fscanf(file, "%s %s [%d] [%d]\n", &buffer, &buffer2, &index_id, &parent_index_id);
		fscanf(file, "count = %u {\n", &param_count);

		for (int i = 0; i < param_count; i++)
		{
			fscanf(file, "%s = (%s)%s\n", &param_name_buffer, &param_typename_buffer, &param_value_buffer);
			
			std::string param_name = param_name_buffer;
			std::string param_typename = param_typename_buffer;
			std::string param_value = param_value_buffer;


			if (param_name == "name")
			{
				name = param_value;
			}
			else if (param_name == "tag")
			{
				tag = param_value;
			}
			else if (param_name == "position")
			{

			}
			

		}

		fclose(file);
	}

	

}



int WINAPI WinMain(HINSTANCE aCurrentInstance, HINSTANCE aPreviousInstance, LPSTR aCommandLineArgs, int aShowCommand)
{
	try
	{
		Application::RegisterEvent(ApplicationEventType::OnStart, OnStart);
		int code = Gem::Application::Execute("Test App", Gem::ApplicationType::Window, aCurrentInstance);
		system("pause");
		return code;
	}
	catch (std::exception & aException)
	{
		MessageBox(NULL, aException.what(), "Error", MB_OK | MB_ICONEXCLAMATION);
	}
	system("pause");
	return EXIT_FAILURE;
}

//int main(int argc, char ** argv)
//{
//
//	Application::RegisterEvent(ApplicationEventType::OnStart, OnStart);
//	Application::RegisterEvent(ApplicationEventType::OnStop, OnStop);
//	Application::RegisterEvent(ApplicationEventType::OnSystemsInitialized, OnInitialized);
//	int code = Gem::Application::Execute("Test App", Gem::ApplicationType::Console, NULL);
//	system("pause");
//
//	return code;
//}
