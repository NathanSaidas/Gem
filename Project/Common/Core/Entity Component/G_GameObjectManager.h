#ifndef G_GAME_OBJECT_MANAGER_H
#define G_GAME_OBJECT_MANAGER_H

#include "../Base Objects/G_Object.h"
#include <vector>
#include <string>

namespace Gem
{
    //How much time to give the garbage collection system to collect destroyed objects
    const float GARBAGE_COLLECTION_TIME = 0.02f;
    const float PHYSICS_UPDATE_TIME = 0.003f;
    

    class GameObject;
    class GameObjectManager : public Object
    {
    public:
        static GameObjectManager * instance();
        static void destroy();

        static GameObject * instantiate(std::string aName);
        static void destroy(GameObject * aGameObject);
        static void destroyImmediate(GameObject * aGameObject);

        inline int objectCount()
        {
            return m_GameObjects.size();
        }
        std::vector<GameObject*> & getGameObjects();

        virtual Reflection::Type * getType();
    private:
        static GameObjectManager * s_Instance;
        GameObjectManager();
        virtual ~GameObjectManager();

        GameObject * _instantiate(std::string aName);
        void _destroy(GameObject * aGameObject);
        void _destroyImmediate(GameObject * aGameObject);

        void update();
        void renderPass(int aWindowID, int aScreenID, int aCameraID);
        void processDestroyRequests();

        std::vector<GameObject*> m_GameObjects;
        std::vector<GameObject*> m_DestroyRequests;

        float m_SlowUpdateTimeStamp;
        float m_FixedUpdateTimeStamp;

        friend class PoolAllocator;
        friend class Application;
        friend class Screen;
    };
}

#endif