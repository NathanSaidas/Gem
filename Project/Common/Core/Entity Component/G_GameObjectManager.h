#ifndef G_GAME_OBJECT_MANAGER_H
#define G_GAME_OBJECT_MANAGER_H

#include "../Primitives/G_PrimitiveObjects.h"
#include <vector>
#include <string>


namespace Gem
{
    //How much time to give the garbage collection system to collect destroyed objects
    const float GARBAGE_COLLECTION_TIME = 0.02f;
    const float PHYSICS_UPDATE_TIME = 0.003f;
    
    class Scene;
    class GameObject;
    class GameObjectManager : public Object
    {
    public:
        GameObjectManager();
        virtual ~GameObjectManager();
        static GameObjectManager * instance();
        static void destroy();

        static GameObject * instantiate(std::string aName);
        static void destroy(GameObject * aGameObject);
        static void destroyImmediate(GameObject * aGameObject);

        void unloadScene(Scene * aScene);

        inline int objectCount()
        {
            return m_GameObjects.size();
        }
        std::vector<GameObject*> & getGameObjects();

        virtual Pointer<Reflection::Type> getType() override;
    private:
        static GameObjectManager * s_Instance;
        

        GameObject * _instantiate(std::string aName);
        void _destroy(GameObject * aGameObject);
        void _destroyImmediate(GameObject * aGameObject);

        void update();
        void renderPass(int aWindowID, int aScreenID, int aCameraID);
        void processDestroyRequests();
        void processSceneUnload();
		void onApplicationQuit();

        std::vector<GameObject*> m_GameObjects;
        std::vector<GameObject*> m_DestroyRequests;

        float m_SlowUpdateTimeStamp;
        float m_FixedUpdateTimeStamp;

        Scene * m_SceneToUnload;

        friend class PoolAllocator;
        friend class Application;
        friend class Screen;
        friend class Window;
    };
}
GEM_CLASS(GameObjectManager,Object)

#endif