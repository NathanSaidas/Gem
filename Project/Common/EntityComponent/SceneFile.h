#ifndef GEM_SCENE_FILE_H
#define GEM_SCENE_FILE_H

#include "../Core/BasicTypes.h"
#include "InstructionTokenID.h"
#include "InstructionData.h"
#include "InstructionTerm.h"



namespace Gem
{
	//Class Declarations.
	class Scene;
	class GameObject;
	class Component;

	//Force Exports...
	FORCE_EXPORT_META(SceneFile);
	FORCE_EXPORT(std::vector<GameObject*>);
	FORCE_EXPORT(std::vector<Component*>);
	FORCE_EXPORT(std::vector<InstructionData*>);
	
	//Class
	/**
	
	*/
	class GEM_API SceneFile : public object
	{
		RDECLARE_CLASS(SceneFile)
	public:
	
		void SaveScene(Scene* aScene, const std::string & aFilename);
	
	private:

		

		std::vector<GameObject*> m_MarkedGameObjects;
		std::vector<Component*> m_MarkedComponents;
		std::vector<InstructionData*> m_AddGameObjectInstructions;
		std::vector<InstructionData*> m_LinkGameObjectInstructions;
		std::vector<InstructionData*> m_AddComponentInstructions;
		std::vector<InstructionData*> m_LinkComponentInstructions;
		
		/** 
		* This method will generate an instruction to create a game object.
		* @param aGameObject The gameobject to re-create.
		*/
		void AddGameObject(GameObject* aGameObject);

		/** 
		* This method wil generate an instruction to create a component .
		* @param aComponent The component to create.
		*/
		void AddComponent(Component* aComponent);

		/**
		* This method will save meta data about the object that will be set when it is re-created.
		* @param aGameObject The source of the meta data.
		*/
		void LinkGameObject(GameObject* aGameObject);

		/**
		* This method will save the meta data about the component. This data is used when the component is re-created to set variables.
		* @param aComponent The source of the meta data.
		*/
		void LinkComponent(Component* aComponent);

		/** 
		* This method creates a new instuction data with the Frame Allocator.
		*/
		InstructionData * NewInstruction();

		/**
		* Retrieves the gameobjects index from the m_SerializedFlag variable
		* @param aGameObject The game object to set the variable of.
		* @return Returns a valid index if the gameobject is found or a -1 for invalid index.
		*/
		SInt32 GetObjectIndex(object * aGameObject);

		/**
		* Retrieves the objects m_SerializedFlag variable from the specified object.
		* @param aGameObject The game object to set the variable of.
		* @return Returns the m_SerializedFlag of the object.
		*/
		SInt32 * GetFlag(object * aObject);

		/**
		* Marks a gameobject setting the m_SerializedFlag variable to the index of where the object is located.
		* @param aGameObject The game object to set the variable of.
		* @return returns the address of te m_SerializedFlag variable of the given gameobject.
		*/
		SInt32 * MarkGameObject(GameObject * aGameObject);

		/**
		* Marks a component setting its m_SerializedFlag variable to the index of where the object is located.
		* @param aComponent The component to set the variable of
		* @return returns the address of the m_SerializedFlag variable.
		*/
		SInt32 * MarkComponent(Component * aComponent);
	
	};
	
	TYPE_DEFINE(SceneFile)

}


#endif // GEM_SCENE_FILE_H

