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
        File file = IO::sceneMasterList(true);
        if(file.verify() == false)
        {
            //Invalid Filename
            return;
        }

        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_file(file.filename().c_str());


        pugi::xml_node root = doc.child(NODE_ROOT);
        if(strcmp(root.name(), NODE_ROOT) != 0)
        {
            //Failed to find root bad file
            return;
        }

        pugi::xml_node sceneFileList = root.child(NODE_MASTER_LIST);
        if(strcmp(sceneFileList.name(), NODE_MASTER_LIST) != 0)
        {
            //Failed to find scenemaster list bad file
            return;
        }

        std::vector<std::string> tempNames;
        
        for(pugi::xml_node_iterator iter = sceneFileList.begin(); iter != sceneFileList.end(); ++iter)
        {
            pugi::xml_node node(iter->internal_object());
            if(strcmp(node.name(),NODE_SCENE_FILE) == 0)
            {
                std::string filename = node.attribute(NODE_FILE_NAME).as_string();
                if(IO::isFile(filename))
                {
                    tempNames.push_back(filename);
                }
            }
        }

        pugi::xml_node startFileNode = root.child(NODE_START_FILE);
        std::string startFile = startFileNode.attribute(NODE_FILE_NAME).as_string();
        
        if(IO::isFile(startFile))
        {
            m_StartSceneFilename = startFile;
        }
        else
        {
            m_StartSceneFilename = "";
        }

        m_SceneFilenames.clear();
        m_SceneFilenames = tempNames;
        


    }
    void SceneManager::saveToMasterList()
    {
        File file = IO::sceneMasterList(true);
        if(file.verify() == false)
        {
            //Invalid Filename
            return;
        }

        pugi::xml_document doc;
        pugi::xml_node root = doc.append_child(NODE_ROOT);
        pugi::xml_node sceneFileList = root.append_child(NODE_MASTER_LIST);

        for(int i = 0; i < m_SceneFilenames.size(); i++)
        {
            pugi::xml_node sceneNode = sceneFileList.append_child(NODE_SCENE_FILE);
            sceneNode.append_attribute(NODE_FILE_NAME) = m_SceneFilenames[i].c_str();
        }
        pugi::xml_node startNode = root.append_child(NODE_START_FILE);
        startNode.append_attribute(NODE_FILE_NAME) = m_StartSceneFilename.c_str();

        doc.save_file(file.filename().c_str());

    }

    bool SceneManager::addNewSceneName(std::string aName)
    {
        Directory assetsDirectory = IO::assetsDirectory();
        if(FileIO::IO::exists(assetsDirectory.directoryName() + "\\Scenes") == false)
        {
            FileIO::IO::createFolder(assetsDirectory.directoryName() + "\\Scenes");
        }
        std::string filename = assetsDirectory.directoryName() + "\\Scenes\\" + aName + ".xml";

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
    Type * SceneManager::getType()
    {
        return Type::create("SceneManager",TypeID::SCENE_MANAGER,sizeof(SceneManager),Object::getType());
    }
}