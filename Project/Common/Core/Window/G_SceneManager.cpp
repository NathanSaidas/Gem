#include "G_SceneManager.h"
#include "../Reflection/G_Reflection.h"
#include "../Memory/G_Memory.h"
#include <pugixml.hpp>
#include "../Utilities/G_IO.h"



namespace Gem
{
    using namespace Reflection;
    using namespace FileIO;

    const char * SceneManager::NODE_ROOT = "Root";
    const char * SceneManager::NODE_MASTER_LIST = "Master_List";
    const char * SceneManager::NODE_START_FILE = "Start_File";
    const char * SceneManager::NODE_SCENE_FILE = "Scene_File";
    const char * SceneManager::NODE_FILE_NAME = "Filename";

    SceneManager * SceneManager::s_Instance = nullptr;
    SceneManager * SceneManager::instance()
    {
        if(s_Instance == nullptr)
        {
            s_Instance = Memory::instantiate<SceneManager>();
        }
        return s_Instance;
    }

    void SceneManager::destroy()
    {
        if(s_Instance != nullptr)
        {
            s_Instance = Memory::destroy<SceneManager>(s_Instance);
        }
    }
    SceneManager::SceneManager()
    {

    }
    SceneManager::~SceneManager()
    {


    }



    void SceneManager::loadFromMasterList()
    {
        

    }
    void SceneManager::saveToMasterList()
    {
        

    }

    bool SceneManager::addNewSceneName(std::string aName)
    {
        Directory assetsDirectory = IO::assetsDirectory();
        if(FileIO::IO::exists(assetsDirectory.directoryName() + "\\Scenes") == false)
        {
            FileIO::IO::createFolder(assetsDirectory.directoryName() + "\\Scenes");
        }
        string filename = assetsDirectory.directoryName() + "\\Scenes\\" + aName + ".xml";

        if(IO::isFile(filename) == false)
        {
            return false;
        }
        
        for(int i = 0; i < m_SceneFilenames.size(); i++)
        {
            if(m_SceneFilenames[i] == filename)
            {
                return false;
            }
        }
        m_SceneFilenames.push_back(filename);
        return true;
    }
    Pointer<Reflection::Type> SceneManager::getType()
    {
        return typeOf("SceneManager");
    }
}