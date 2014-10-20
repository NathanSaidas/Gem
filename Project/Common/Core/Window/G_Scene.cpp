#include "G_Scene.h"
#include "../Reflection/G_Reflection.h"
#include "../Utilities/G_IO.h"
#include "../Entity Component/G_GameObject.h"
#include "../Entity Component/G_GameObjectManager.h"
#include <pugixml.hpp>

namespace Gem
{
    using namespace Reflection;
    using namespace FileIO;

    Scene::Scene()
    {
        m_Name = "";
        m_Filename = "";
        m_Index = 0;
        m_IsEditing = false;
    }

    Scene::~Scene()
    {
    }

    void Scene::placeGameObject(GameObject * aGameObject)
    {
        if(isEditing() == false)
        {
            return;
        }
        m_GameObjects.push_back(aGameObject);
    }
    void Scene::removeGameObject(GameObject * aGameObject)
    {
        if(isEditing() == false)
        {
            return;
        }
        for(int i = 0; i < m_GameObjects.size(); i++)
        {
            if(m_GameObjects[i] == aGameObject)
            {
                m_GameObjects.erase(m_GameObjects.begin() + i);
                return;
            }
        }
    }


    void Scene::save()
    {
        if(isEditing() == false)
        {
            return;
        }
        string file = filename();
        if(IO::isFile(file) == false)
        {
            return;
        }

        pugi::xml_document doc;
        
        pugi::xml_node rootNode = doc.child("Root");
        pugi::xml_node sceneInfoNode = rootNode.append_child("SceneInfo");
        pugi::xml_node gameObjectsNode = rootNode.append_child("GameObjects");

        for(int i = 0; i < m_GameObjects.size(); i++)
        {
            if(m_GameObjects[i] == nullptr)
            {
                continue;
            }
            //m_GameObjects[i]->deserialize(gameObjectsNode.append_child("GameObject"));
        }

        if(doc.save_file(file.c_str()) == false)
        {
            //error saving
        }

    }
    void Scene::load()
    {
        string file = filename();
        if(IO::isFile(file) == false)
        {
            return;
        }

        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_file(file.c_str());

        if(result.status != pugi::xml_parse_status::status_ok)
        {
            //Error parsing file
        }

        pugi::xml_node rootNode = doc.child("Root");

        pugi::xml_node sceneInfoNode = rootNode.child("SceneInfo");

        pugi::xml_node gameObjectsNode = rootNode.child("GameObjects");

        for(pugi::xml_node_iterator iter = gameObjectsNode.begin(); iter != gameObjectsNode.end(); ++iter)
        {
            pugi::xml_node gameObjectNode(iter->internal_object());

            if(strcmp(gameObjectNode.name(),"GameObject") != 0)
            {
                continue;
            }

            //GameObject * gameObject = GameObject::instantiate("Empty_GameObject");
            //gameObject->deserialize(gameObjectNode);
            //placeGameObject(gameObject);
        }
    }

    void Scene::unload()
    {
        GameObjectManager::instance()->unloadScene(this);
    }
    void Scene::close()
    {
        for(int i = 0; i < m_GameObjects.size(); i ++)
        {
            GameObject::destroyImmediate(m_GameObjects[i]);
        }
    }
    void Scene::open()
    {
        //close current scene
        //load this one
        load();
    }

    string Scene::name()
    {
        return m_Name;
    }
    string Scene::filename()
    {
        return m_Filename;
    }
    int Scene::index()
    {
        return m_Index;
    }
    bool Scene::isEditing()
    {
        return m_IsEditing;
    }


    void Scene::setName(string aName)
    {
        m_Name = aName;
        m_Filename = FileIO::IO::assetsDirectory().directoryName() + "\\Scenes\\" + aName + ".xml";
    }
    void Scene::setIsEditing(bool aValue)
    {
        m_IsEditing = aValue;
    }

    Pointer<Reflection::Type> Scene::getType()
    {
		return typeOf("Scene");
    }

}