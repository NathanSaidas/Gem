#ifndef GEM_EC_SERIALIZER_H
#define GEM_EC_SERIALIZER_H

#include "../Core/ValueTypes.h"
#include <vector>

#include "InstructionTokenID.h"

namespace Gem
{
	class GameObject;
	class Component;

	namespace EntityComponent
	{
		struct InstructionData;

		class ECSerializer
		{
			public:	

				void TestBed(GameObject * aGameObject);

				void AddGameObject(GameObject* aGameObject);
				void AddComponents(Component* aComponent);

				void LinkGameObject(GameObject* aGameObject);

				void WriteFile();
		private:

			

			std::vector<GameObject*> m_MarkedGameObjects;
			std::vector<Component*> m_MarkedComponents;
			std::vector<InstructionData*> m_AddGameObjectInstructions;
			std::vector<InstructionData*> m_LinkGameObjectInstructions;

			InstructionData * NewInstruction();

			void GenerateInstruction(GameObject * aGameObject, InstructionTokenID & aInstructionType );

			void AddGameObjectInstruction(GameObject * aGameObject, InstructionTokenID * aInstructionType);

			/** 
			* Retrieves the gameobjects index from the m_SerializedFlag variable
			* @param aGameObject The game object to set the variable of.
			* @return Returns a valid index if the gameobject is found or a -1 for invalid index.
			*/
			SInt32 GetGameObjectIndex(GameObject * aGameObject);

			/**
			* Retrieves the gameobjects m_SerializedFlag variable from the specified gameobject.
			* @param aGameObject The game object to set the variable of.
			* @return Returns the m_SerializedFlag of the gameobject.
			*/
			SInt32 * GetFlag(GameObject * aGameObject);

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
	}
}

#endif