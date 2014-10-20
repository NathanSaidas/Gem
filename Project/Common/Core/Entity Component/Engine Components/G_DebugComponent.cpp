#include "G_DebugComponent.h"
#include <pugixml.hpp>
#include "../../G_Application.h"
#include "../../Utilities/G_Utilities.h"
#include "../../Reflection/G_Reflection.h"
#include "../../Memory/G_Memory.h"
#include "../../Math/G_Math.h"
#include "../../Utilities/G_Debug.h"


#include "../../Input/G_Input.h"
#include "../../Utilities/G_IO.h"

#include <cstdio>

#include "../../Utilities/Collections/G_Collections.h"

namespace Gem
{
    using namespace Reflection;
	using namespace FileIO;

    DebugComponent::DebugComponent()
    {
        
    }
    DebugComponent::~DebugComponent()
    {
        
    }


    Pointer<Reflection::Type> DebugComponent::getType()
    {
        return typeOf("DebugComponent");
    }

    //Creation/Delete Functions
    void DebugComponent::onCreate()   //Unsafe Reference
    {
        log("Create");
    }
    void DebugComponent::onInit()     //Safe Reference 
    {
        log("Init");

        //bool isEqual = instanceOf(objType.ref(),compType.ref());
        //isEqual = instanceOf(compType.ref(),objType.ref());
        //isEqual = instanceOf(type.ref(), objType.ref());
		int * a = Memory::instantiate<int>();
		int * b = Memory::instantiate<int>();
		int * c = Memory::instantiate<int>();
		int * d = Memory::instantiate<int>();

		(*a) = 0;
		(*b) = 5;
		(*c) = 7;
		(*d) = 18;

		Collections::List<int> intList = Collections::List<int>();
		intList.add(a);
		intList.add(b);
		intList.add(c);
		intList.add(d);

		int * indexed = intList.at(2);
		intList.remove(indexed);
		intList.removeAt(2);
		Memory::destroy<int>(a);
		Memory::destroy<int>(b);
		Memory::destroy<int>(c);
		Memory::destroy<int>(d);
    }
    void DebugComponent::onLateInit() //Safe Reference
    {
        log("Late Init");
    }

    void DebugComponent::onEnable()   //Safe Reference
    {
        log("Enable");
    }
    void DebugComponent::onDisable()  //Safe Reference  
    {

    }
    void DebugComponent::onDestroy()  //Safe Reference  
    {
        log("Destroy");
        m_IntPtr.release();
    }

	void DebugComponent::onApplicationQuit()
	{

	}

    void DebugComponent::preUpdate() 
    {
        

    }
    void DebugComponent::update()
    {
		if (Input::instance()->getKeyDown(KeyCode::ALPHA_1))
		{
			//Debug::saveLogFile();
			Application::exit();
		}

		if (Input::instance()->getKeyDown(KeyCode::ALPHA_2))
		{
			
			Pointer<Vector2> mousePosition;	
			(*mousePosition.ref()) = Input::instance()->mousePoisition();
			Debug::log(mousePosition.ref(), true);
		}

		//{
		//	Directory logsDirectory = FileIO::IO::logFileDirectory();
		//	if (logsDirectory.verify() == false)
		//	{
		//		return;
		//	}
		//
		//	string path = logsDirectory.path();
		//	//path.append("\\Debug_Log.txt");
		//
		//	string write = "Hello World.";
		//	string filename = "\\Debug_Log.txt";
		//	string filenames[] = { "\\Log_01", "\\Log_02", "\\Log_03", "\\Log_04", "\\Log_05" };
		//
		//	Pointer<File> file;
		//	File * fileRef = file.ref();
		//	if (fileRef != nullptr)
		//	{
		//		fileRef->setWriteStream(write);
		//		fileRef->createFile(path,filename);
		//		for (int i = 0; i < 5; i++)
		//		{
		//			fileRef->createFile(path, filenames[i]);
		//		}
		//		fileRef->open(path, FileMode::READ_WRITE);
		//
		//	
		//
		//		fileRef->close();
		//	}
		//}
    }
    void DebugComponent::postUpdate()
    {

    }

    void DebugComponent::slowUpdate()
    {
		Debug::log("Slow Update");
    }

    void DebugComponent::fixedUpdate()
    {

    }

    void DebugComponent::preRender()
    {

    }
    void DebugComponent::render()
    {

    }
    void DebugComponent::postRender()
    {

    }

}