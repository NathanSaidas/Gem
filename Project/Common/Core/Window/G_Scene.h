#ifndef G_SCREEN_H
#define G_SCREEN_H

#include "../Base Objects/G_Object.h"
#include <string>
#include <vector>

namespace Gem
{
    class GameObject;

    //The purpose of a scene class is.
    //-Add gameobjects to a scene during edit time
    //-Remove gameobjects from a scene during edit time
    //-Save gameobjects from the scene to a file during edit time
    //-Load gameobjects from the file and into the scene during edit time
    
    


    class Scene : public Object
    {
    public:
        Scene();
        ~Scene();

        //This method places a game object into the scene at edit time
        //(Designed for edit time)
        void placeGameObject(GameObject * aGameObject);
        //This method removes a game object from the scene at edit time
        //(Designed for edit time)
        void removeGameObject(GameObject * aGameObject);

        //this method saves all game objects in the scene to a file
        //(Designed for edit time)
        void save();
        //this method creates game objects and deserializes them from file data
        //(Designed for edit time)
        void load();

        //This method tells the game object manager to unload this scene
        //When it comes time to unload a scene 
        //(Designed for run time)
        void unload();

        //This method unloads all gameobjects from this scene immediately
        //(Designed for edit time)
        void close();
        //This method will close the current scene and load this one
        //(Designed for edit time)
        void open();

        std::string name();
        std::string filename();
        int index();
        bool isEditing();


        void setName(std::string aName);
        void setIsEditing(bool aValue);


        virtual Reflection::Type * getType();
    private:
        std::string m_Name;
        std::string m_Filename;
        int m_Index;

        bool m_IsEditing;

        std::vector<GameObject*> m_GameObjects;
    };

}

#endif