#ifndef G_SCENE_MANAGER_H
#define G_SCENE_MANAGER_H

#include "../Base Objects/G_Object.h"
#include <vector>

namespace Gem
{
    class Scene;
    //This is how you seal a class in c++
    class SceneManager sealed: public Object
    {
    public:
        

        static SceneManager * instance();
        static void destroy();

        void loadFromMasterList();
        void saveToMasterList();
        bool addNewSceneName(std::string aName);

        //This is how you force an override on a method
        //you can also use final to block further override
        virtual Reflection::Type * getType() override;
    private:
        //Singleton Stuff
        static SceneManager * s_Instance;
        SceneManager();
        ~SceneManager();

        std::vector<std::string> m_SceneFilenames;
        std::string m_StartSceneFilename;

        std::vector<Scene *> m_Scenes;
        Scene * m_CurrentScene;
        
        static const char * NODE_ROOT;
        static const char * NODE_MASTER_LIST;
        static const char * NODE_START_FILE;
        static const char * NODE_SCENE_FILE;
        static const char * NODE_FILE_NAME;

        friend PoolAllocator;
    };
    
}

#endif