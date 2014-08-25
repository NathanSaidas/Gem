#include "G_GameObjectManager.h"
#include "G_GameObject.h"
#include "../Reflection/G_Reflection.h"
#include "../Memory/G_Memory.h"
#include "../Utilities/G_Time.h"
#include "../Utilities/G_Utilities.h"

namespace Gem
{
    using namespace Reflection;
    GameObjectManager * GameObjectManager::s_Instance = nullptr;
    GameObjectManager * GameObjectManager::instance()
    {
        if(s_Instance == nullptr)
        {
            s_Instance = Memory::instantiate<GameObjectManager>();
        }
        return s_Instance;
    }
    void GameObjectManager::destroy()
    {
        if(s_Instance != nullptr)
        {
            s_Instance = Memory::destroy<GameObjectManager>(s_Instance);
        }
    }

    Type * GameObjectManager::getType()
    {
        return Type::create("GameObjectManager",TypeID::GAME_OBJECT_MANAGER,sizeof(GameObjectManager),Object::getType());
    }

    
    GameObject * GameObjectManager::instantiate(std::string aName)
    {
        return instance()->_instantiate(aName);
    }
    void GameObjectManager::destroy(GameObject * aGameObject)
    {
        instance()->_destroy(aGameObject);
    }
    void GameObjectManager::destroyImmediate(GameObject * aGameObject)
    {
        instance()->_destroyImmediate(aGameObject);
    }
    GameObject * GameObjectManager::_instantiate(std::string aName)
    {
        GameObject * gameObject = Memory::instantiate<GameObject>();
        gameObject->setName(aName);
        m_GameObjects.push_back(gameObject);
        return gameObject;
    }
    void GameObjectManager::_destroy(GameObject * aGameObject)
    {
        if(aGameObject == nullptr)
        {
            return;
        }
        for(int i = 0; i < m_DestroyRequests.size(); i++)
        {
            if(aGameObject == m_DestroyRequests[i])
            {
                return;
            }
        }
        aGameObject->raiseFlag(ECFlag::FLAGGED_FOR_DESTROY);
        m_DestroyRequests.push_back(aGameObject);
    }
    void GameObjectManager::_destroyImmediate(GameObject * aGameObject)
    {
        if(aGameObject == nullptr)
        {
            return;
        }
        for(int i = 0; i < m_DestroyRequests.size(); i++)
        {
            if(m_DestroyRequests[i] == aGameObject)
            {
                m_DestroyRequests.erase(m_DestroyRequests.begin() + i);
                break;
            }
        }
        for(int i = 0; i < m_GameObjects.size(); i++)
        {
            if(m_GameObjects[i] == aGameObject)
            {
                m_GameObjects.erase(m_GameObjects.begin() + i);
                break;
            }
        }
        aGameObject = Memory::destroy<GameObject>(aGameObject);
    }
    void GameObjectManager::update()
    {
        float startTime = Time::getTime();

        bool fixedUpdate = fabsf(Time::getTime() - m_FixedUpdateTimeStamp) > PHYSICS_UPDATE_TIME;
        bool slowUpdate = fabsf(Time::getTime() - m_SlowUpdateTimeStamp) > 1.0f;
        if(fixedUpdate == true)
        {
            m_FixedUpdateTimeStamp = Time::getTime();
        }
        if(slowUpdate == true)
        {
            m_SlowUpdateTimeStamp = Time::getTime();
        }

        for(int i = 0; i < m_GameObjects.size(); i++)
        {
            m_GameObjects[i]->preUpdate();
        }
        for(int i = 0; i < m_GameObjects.size(); i++)
        {
            m_GameObjects[i]->update();
        }
        for(int i = 0; i < m_GameObjects.size(); i++)
        {
            m_GameObjects[i]->postUpdate();
        }
        if(fixedUpdate == true)
        {
            for(int i = 0; i < m_GameObjects.size(); i++)
            {
                m_GameObjects[i]->fixedUpdate();
            }
        }
        if(slowUpdate == true)
        {
            for(int i = 0; i < m_GameObjects.size(); i++)
            {
                m_GameObjects[i]->slowUpdate();
            }
        }

        //log("Update Time - " + F2S(Time::getTime() - startTime));
    }
    void GameObjectManager::renderPass(int aWindowID, int aScreenID, int aCameraID)
    {

    }
    void GameObjectManager::processDestroyRequests()
    {
        if(m_DestroyRequests.size() == 0)
        {
            return;
        }
        log("Garbage Collection Started");
        //A vector to store garbage objects
        std::vector<GameObject*> garbage;
        float startTime = Time::getTime();
        //Go through all objects in the request list
        for(int i = m_DestroyRequests.size() -1; i >= 0; i--)
        {
            bool failure = true;
            //Find the matching object in the object list
            for(int j = m_GameObjects.size() -1; j >= 0; j--)
            {
                if(m_GameObjects[j] == m_DestroyRequests[i])
                {
                    m_GameObjects.erase(m_GameObjects.begin() + j);
                    garbage.push_back(m_DestroyRequests[i]);
                    m_DestroyRequests.erase(m_DestroyRequests.begin() + i);
                    failure = false;
                    break;
                }
            }
            if(failure == true)
            {
                //error an object was not instantiated properly possible memory leak
            }
            float delta = Time::getTime() - startTime;
            if(delta > GARBAGE_COLLECTION_TIME)
            {
                log("Garbage Collection Time Reached");
                break;
            }
        }

        for(int i = 0; i < garbage.size(); i++)
        {
            garbage[i] = Memory::destroy<GameObject>(garbage[i]);
        }

        log("Garbage Collection Ended");
        log("Time - " + F2S(Time::getTime() - startTime));
    }
    GameObjectManager::GameObjectManager() : Object()
    {
        m_SlowUpdateTimeStamp = 0.0f;
        m_FixedUpdateTimeStamp = 0.0f;
    }
    GameObjectManager::~GameObjectManager()
    {
        //Clean up objects

        for(int i = 0; i < m_GameObjects.size(); i++)
        {
            m_GameObjects[i] = Memory::destroy<GameObject>(m_GameObjects[i]);
        }
        m_GameObjects.clear();
    }



    std::vector<GameObject*> & GameObjectManager::getGameObjects()
    {
        return m_GameObjects;
    }
}