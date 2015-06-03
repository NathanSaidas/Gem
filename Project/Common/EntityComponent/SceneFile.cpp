#include "SceneFile.h"
#include <stack>
#include "../Reflection/Reflection.h"
#include "../Memory/Memory.h"
#include "../Core/Debug.h"
#include "Scene.h"



using namespace Gem::Debugging;
using namespace Gem::Reflection;


namespace Gem
{
	RDEFINE_CLASS(SceneFile,object)
	/// <summary>
	/// Saves the scene.
	/// </summary>
	/// <param name="aScene">a scene.</param>
	void SceneFile::SaveScene(Scene* aScene, const std::string & aFilename)
	{
		FILE* file = fopen(aFilename.c_str(), "w");
		if (file == NULL)
		{
			return;
		}

		std::stack<GameObject*> gameObjectStack;
		std::stack<SInt32> indexStack;
		std::vector<GameObject*> inverseGameObjectList;

		GameObject* root = aScene->GetRootGameObject();

		gameObjectStack.push(root);
		indexStack.push(0);

		//Build the component list.
		while (gameObjectStack.size() > 0)
		{
			GameObject* top = gameObjectStack.top();
			SInt32 currentIndex = indexStack.top();

			if (top == root && currentIndex >= root->m_Children.size())
			{
				break;
			}

			std::vector<GameObject*> & children = top->m_Children;

			//Go deeper.
			if (children.size() > currentIndex)
			{
				gameObjectStack.push(children[currentIndex]);
				indexStack.push(0);
			}
			//Reached a dead end, add myself and pop the stacks.
			else
			{
				inverseGameObjectList.push_back(top);
				gameObjectStack.pop();
				indexStack.pop();
				int & newIndex = indexStack.top();
				newIndex++;
			}
		}

		//Add All GameObjects
		for (std::vector<GameObject*>::reverse_iterator it = inverseGameObjectList.rbegin();
			it != inverseGameObjectList.rend();
			it++)
		{
			AddGameObject(*it);
		}
		
		//Link All GameObjects
		for (std::vector<GameObject*>::iterator it = m_MarkedGameObjects.begin();
			it != m_MarkedGameObjects.end();
			it++)
		{
			LinkGameObject(*it);
		} 

		//Add all Components
		for (std::vector<GameObject*>::iterator it = m_MarkedGameObjects.begin();
			it != m_MarkedGameObjects.end();
			it++)
		{
			GameObject* gameObject = *it;
			std::vector<Component*> components = gameObject->m_Components;

			for (std::vector<Component*>::iterator compIt = components.begin();
				compIt != components.end();
				compIt++)
			{
				AddComponent(*compIt);
			}
		}

		//Link all Components.
		for (std::vector<Component*>::iterator it = m_MarkedComponents.begin();
			it != m_MarkedComponents.end();
			it++)
		{
			LinkComponent(*it);
		}

		//Now all the data is available to write it to a file in whatever format...

		InstructionTokenID currentInstruction = InstructionTokenID::AddGameObject;

		fprintf(file, "begin %s\n", currentInstruction.ToString().c_str());
		for (std::vector<InstructionData*>::iterator it = m_AddGameObjectInstructions.begin();
			it != m_AddGameObjectInstructions.end();
			it++)
		{
			fprintf(file, "%s %d %d\n", currentInstruction.ToString().c_str(), (*it)->reservedParam0, (*it)->reservedParam1);
		}
		fprintf(file, "end %s\n", currentInstruction.ToString().c_str());

		currentInstruction = InstructionTokenID::LinkGameObject;

		fprintf(file, "begin %s\n", currentInstruction.ToString().c_str());
		for (std::vector<InstructionData*>::iterator it = m_LinkGameObjectInstructions.begin();
			it != m_LinkGameObjectInstructions.end();
			it++)
		{
			//print state ment.
			fprintf(file, "%s %d\ncount = %d {\n", currentInstruction.ToString().c_str(), (*it)->reservedParam0, (*it)->terms.size());
			//print each term.
			for (std::vector<InstructionTerm*>::iterator termIt = (*it)->terms.begin();
				termIt != (*it)->terms.end();
				termIt++)
			{
				fprintf(file, "%s\n", (*termIt)->ToString().c_str());
			}
			fprintf(file, "}\n");
		}
		fprintf(file, "end %s\n", currentInstruction.ToString().c_str());

		//Add Components.
		currentInstruction = InstructionTokenID::AddComponent;
		fprintf(file, "begin %s\n", currentInstruction.ToString().c_str());
		for (std::vector<InstructionData*>::iterator it = m_AddComponentInstructions.begin();
			it != m_AddComponentInstructions.end();
			it++)
		{
			fprintf(file, "%s %d %d %s\n",
				currentInstruction.ToString().c_str(),
				(*it)->reservedParam0,
				(*it)->reservedParam1,
				(*it)->typeName);
		}
		fprintf(file, "end %s\n", currentInstruction.ToString().c_str());

		//Link Components..
		currentInstruction = InstructionTokenID::LinkComponent;
		fprintf(file, "begin %s\n", currentInstruction.ToString().c_str());
		for (std::vector<InstructionData*>::iterator it = m_LinkComponentInstructions.begin();
			it != m_LinkComponentInstructions.end();
			it++)
		{
			fprintf(file, "%s %d\ncount = %d {\n",
				currentInstruction.ToString().c_str(),
				(*it)->reservedParam0,
				(*it)->terms.size());

			for (std::vector<InstructionTerm*>::iterator termIt = (*it)->terms.begin();
				termIt != (*it)->terms.end();
				termIt++)
			{
				fprintf(file, "%s\n", (*termIt)->ToString().c_str());
			}
			fprintf(file, "}\n");
		}
		fprintf(file, "begin %s\n", currentInstruction.ToString().c_str());
		
		fclose(file);
	}

	void SceneFile::AddGameObject(GameObject* aGameObject)
	{

		SInt32* flags = MarkGameObject(aGameObject);
		SInt32* parentFlags = GetFlag(aGameObject->GetParent());
		InstructionTokenID instructionTokenID = InstructionTokenID::AddGameObject;
		if (flags == nullptr)
		{
			return;
		}

		InstructionData* instructionData = NewInstruction();

		instructionData->tokenID = instructionTokenID;
		instructionData->tokenIDName = instructionTokenID;
		instructionData->typeName = aGameObject->GetType().GetName();

		instructionData->reservedParam0 = *flags;
		instructionData->reservedParam1 = parentFlags != nullptr ? *parentFlags : -1;

		instructionData->paramCount = -1;
		instructionData->terms = std::vector<InstructionTerm*>();

		m_AddGameObjectInstructions.push_back(instructionData);
	}

	void SceneFile::AddComponent(Component* aComponent)
	{
		SInt32* flags = MarkComponent(aComponent);
		SInt32* gameobjectFlags = GetFlag(aComponent->GetGameObject());
		InstructionTokenID instructionTokenID = InstructionTokenID::AddComponent;
		if (flags == nullptr)
		{
			return;
		}

		InstructionData* instructionData = NewInstruction();

		instructionData->tokenID = instructionTokenID;
		instructionData->tokenIDName = instructionTokenID;
		instructionData->typeName = aComponent->GetType().GetName();

		instructionData->reservedParam0 = *flags;
		instructionData->reservedParam1 = *gameobjectFlags;

		instructionData->paramCount = -1;
		instructionData->terms = std::vector<InstructionTerm*>();

		m_AddComponentInstructions.push_back(instructionData);
	}

	void SceneFile::LinkGameObject(GameObject* aGameObject)
	{
		//Get the flags..
		SInt32* flags = GetFlag(aGameObject);
		InstructionTokenID instructionTokenID = InstructionTokenID::LinkGameObject;
		if (flags == nullptr || *flags == -1)
		{
			return;
		}
		Type type = aGameObject->GetType();
		//Set the basic instruction data.
		InstructionData* instructionData = NewInstruction();
		instructionData->tokenID = instructionTokenID;
		instructionData->tokenIDName = instructionTokenID;
		instructionData->typeName = type.GetName();
		instructionData->reservedParam0 = *flags;
		instructionData->reservedParam1 = -1;
		instructionData->terms = std::vector<InstructionTerm*>();
		
		MethodInfo<GameObject, void, std::vector<InstructionTerm*>&>* serializeMethod = 
			dynamic_cast<MethodInfo<GameObject, void, std::vector<InstructionTerm*>&>*>(type.GetMethodInfo("OnSerializeData"));
		if (serializeMethod != nullptr)
		{
			serializeMethod->GetMethod().Invoke(aGameObject, instructionData->terms);
		}
		m_LinkGameObjectInstructions.push_back(instructionData);
	}

	void SceneFile::LinkComponent(Component* aComponent)
	{
		SInt32* flags = GetFlag(aComponent);
		InstructionTokenID instructionTokenID = InstructionTokenID::LinkComponent;
		if (flags == nullptr || *flags == -1)
		{
			return;
		}
		Type type = aComponent->GetType();

		InstructionData* instructionData = NewInstruction();
		instructionData->tokenID = instructionTokenID;
		instructionData->tokenIDName = instructionTokenID;
		instructionData->typeName = type.GetName();
		instructionData->reservedParam0 = *flags;
		instructionData->reservedParam1 = -1;
		instructionData->terms = std::vector<InstructionTerm*>();

		MethodInfo<Component, void, std::vector<InstructionTerm*>&>* serializeMethod =
			dynamic_cast<MethodInfo<Component, void, std::vector<InstructionTerm*>&>*>(type.GetMethodInfo("OnSerializeData"));
		if (serializeMethod != nullptr)
		{
			serializeMethod->GetMethod().Invoke(aComponent, instructionData->terms);
		}
		m_LinkComponentInstructions.push_back(instructionData);

	}

	InstructionData * SceneFile::NewInstruction()
	{
		return MEM_FRAME_ALLOC_T(InstructionData);
	}

	SInt32 SceneFile::GetObjectIndex(object * aGameObject)
	{
		SInt32 * flag = GetFlag(aGameObject);
		if (flag != nullptr)
		{
			return *flag;
		}
		return -1;
	}



	SInt32 * SceneFile::GetFlag(object * aObject)
	{
		if (aObject == nullptr)
		{
			return nullptr;
		}

		//Get the type data from gameobjects
		Type type = aObject->GetType();
		//Search for its m_SerializerFlag with reflection.
		MemberInfo serializerFlagMember = type.GetMember("m_SerializerFlag");
		//Check if its a bad member info.
		if (serializerFlagMember.GetClassName() == nullptr)
		{
			return nullptr;
		}
		//Get the memory address of the serializer flag variable and set its value.
		return (SInt32*)serializerFlagMember.GetOffsetPointer(aObject);
	}

	SInt32 * SceneFile::MarkGameObject(GameObject * aGameObject)
	{
		if (aGameObject == nullptr)
		{
			return nullptr;
		}

		//Get the type data from gameobjects
		Type type = aGameObject->GetType();
		//Search for its m_SerializerFlag with reflection.
		MemberInfo serializerFlagMember = type.GetMember("m_SerializerFlag");
		//Check if its a bad member info.
		if (serializerFlagMember.GetClassName() == nullptr)
		{
			return nullptr;
		}
		//Get the memory address of the serializer flag variable and set its value.
		SInt32 * serializerFlag = (SInt32*)serializerFlagMember.GetOffsetPointer(aGameObject);
		*serializerFlag = m_MarkedGameObjects.size();
		m_MarkedGameObjects.push_back(aGameObject);
		return serializerFlag;
	}

	SInt32 * SceneFile::MarkComponent(Component * aComponent)
	{
		if (aComponent == nullptr)
		{
			return nullptr;
		}

		Type type = aComponent->GetType();
		MemberInfo serializerFlagMember = type.GetMember("m_SerializerFlag");
		if (serializerFlagMember.GetClassName() == nullptr)
		{
			return nullptr;
		}

		SInt32 * serializerFlag = (SInt32*)serializerFlagMember.GetOffsetPointer(aComponent);
		*serializerFlag = m_MarkedComponents.size();
		m_MarkedComponents.push_back(aComponent);
		return serializerFlag;
	}


}