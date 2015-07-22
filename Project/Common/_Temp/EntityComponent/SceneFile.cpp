#include "SceneFile.h"
#include <stack>
#include "../Reflection/Reflection.h"
#include "../Memory/Memory.h"
#include "../Utilities/Utilities.h"
#include "../Core/Debug.h"
#include "Scene.h"



using namespace Gem::Debugging;
using namespace Gem::Reflection;


namespace Gem
{
	RDEFINE_CLASS(SceneFile,object)

	void SceneFile::SaveScene(Scene* aScene, const std::string & aFilename)
	{
		FILE* file = fopen(aFilename.c_str(), "w");
		if (file == NULL)
		{
			//Invalid file
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
				(*it)->typeName.c_str());
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
		fprintf(file, "end %s\n", currentInstruction.ToString().c_str());
		
		fclose(file);
	}

	void SceneFile::LoadScene(const std::string & aFilename)
	{
		const int STATE_SEARCHING = 0;
		const int STATE_ADD_GAMEOBJECT = 1;
		const int STATE_ADD_COMPONENT = 2;
		const int STATE_LINK_GAMEOBJECT = 3;
		const int STATE_LINK_COMPONENT = 4;

		const int STATE_READING_HEADER = 0;
		const int STATE_READING_PARAM = 1;

		FILE* file = fopen(aFilename.c_str(), "r");

		if (file == nullptr)
		{
			Debug::Log("Gem", "Failed load scene, cannot open the file.");
			return;
		}

		std::string line = "";
		std::vector<std::string> words;
		bool endOfFile = false;
		//The current overall state.
		int currentState = STATE_SEARCHING;
		//A secondary state for linking.
		int secondaryState = STATE_READING_HEADER;
		//The current instruction being made. (This is for linking only, not adding).
		InstructionData* currentInstruction = nullptr;

		while (endOfFile != true)
		{
			words.clear();
			line = "";
			//Read the line...
			endOfFile = Utilities::ReadLine(file, line);
			Utilities::GetWords(line, words);

			switch (currentState)
			{
			case STATE_SEARCHING:
			{
				if (words.size() >= 1 && words[0] == "begin")
				{
					if (words.size() >= 2)
					{
						std::string nextState = words[1];
						if (nextState == Enum::GetName<InstructionTokenID>(InstructionTokenID::AddGameObject))
						{
							currentState = STATE_ADD_GAMEOBJECT;
						}
						else if (nextState == Enum::GetName<InstructionTokenID>(InstructionTokenID::AddComponent))
						{
							currentState = STATE_ADD_COMPONENT;
						}
						else if (nextState == Enum::GetName<InstructionTokenID>(InstructionTokenID::LinkGameObject))
						{
							currentState = STATE_LINK_GAMEOBJECT;
						}
						else if (nextState == Enum::GetName<InstructionTokenID>(InstructionTokenID::LinkComponent))
						{
							currentState = STATE_LINK_COMPONENT;
						}
						else
						{
							Debug::ErrorFormat("Gem", nullptr, "Invalid state found %", nextState.c_str());
						}
					}
					else
					{
						Debug::Error("Gem", "Suggested new state but there is no state to transition to.");
					}
				}
			}
				break;
			case STATE_ADD_GAMEOBJECT:
			{
				if (words.size() >= 1 && words[0] == "end")
				{
					currentState = STATE_SEARCHING;
				}
				else if (words.size() != 3)
				{
					Debug::Error("Gem", "Cannot add the new gameobject because there is not 3 parameters.");
				}
				else
				{
					SInt32 id = Utilities::S2I(words[1]);
					SInt32 parentID = Utilities::S2I(words[2]);
					AddGameObject(id, parentID);
				}
			}
				break;
			case STATE_ADD_COMPONENT:
			{
				if (words.size() >= 1 && words[0] == "end")
				{
					currentState = STATE_SEARCHING;
				}
				else if (words.size() != 4)
				{
					Debug::Error("Gem", "Cannot add the new component because there is not 4 parameters.");
				}
				else
				{
					SInt32 id = Utilities::S2I(words[1]);
					SInt32 gameObjectID = Utilities::S2I(words[2]);
					AddComponent(id, gameObjectID, words[3]);
				}
			}
				break;
			case STATE_LINK_GAMEOBJECT:
			{
				switch (secondaryState)
				{
				case STATE_READING_HEADER:
					if (words.size() == 2)
					{
						if (words[0] == "end")
						{
							currentState = STATE_SEARCHING;
						}
						else if (words[0] != Enum::GetName<InstructionTokenID>(InstructionTokenID::LinkGameObject))
						{
							Debug::ErrorFormat("Gem", nullptr, "Invalid linking instruction %s, Currently linking gameobjects.", words[0].c_str());
						}
						else
						{
							//Create a new instruction...
							currentInstruction = NewInstruction();
							currentInstruction->tokenID = InstructionTokenID::LinkGameObject;
							currentInstruction->tokenIDName = words[0].c_str();
							currentInstruction->reservedParam0 = Utilities::S2I(words[1]);
							currentInstruction->reservedParam1 = -1;
							currentInstruction->typeName = Reflection::Runtime::TypeOf<GameObject>().GetName();
							currentInstruction->paramCount - 1;
						}
					}
					else if (words.size() == 4)
					{
						if (words[0] == "count" && currentInstruction != nullptr)
						{
							currentInstruction->paramCount = Utilities::S2I(words[2]);
							secondaryState = STATE_READING_PARAM;
						}
					}
					break;
				case STATE_READING_PARAM:
					if (words.size() == 1)
					{
						if (words[0] == "}")
						{
							secondaryState = STATE_READING_HEADER;
							m_LinkGameObjectInstructions.push_back(currentInstruction);
							currentInstruction = nullptr;
						}
					}
					else if (words.size() == 3 && currentInstruction != nullptr)
					{
						currentInstruction->terms.push_back(MEM_FRAME_ALLOC_T(InstructionTerm, words[0], words[2]));
					}
					break;
				}
			}
				break;
			case STATE_LINK_COMPONENT:
			{
				switch (secondaryState)
				{
				case STATE_READING_HEADER:
					if (words.size() == 2)
					{
						if (words[0] == "end")
						{
							currentState = STATE_SEARCHING;
						}
						else if (words[0] != Enum::GetName<InstructionTokenID>(InstructionTokenID::LinkComponent))
						{
							Debug::ErrorFormat("Gem", nullptr, "Invalid linking instruction %s, Currently linking gameobjects.", words[0].c_str());
						}
						else
						{
							//Create a new instruction...
							currentInstruction = NewInstruction();
							currentInstruction->tokenID = InstructionTokenID::LinkComponent;
							currentInstruction->tokenIDName = words[0].c_str();
							currentInstruction->reservedParam0 = Utilities::S2I(words[1]);
							currentInstruction->reservedParam1 = -1;
						}
					}
					else if (words.size() == 4)
					{
						if (words[0] == "count" && currentInstruction != nullptr)
						{
							currentInstruction->paramCount = Utilities::S2I(words[2]);
							secondaryState = STATE_READING_PARAM;
						}
					}
					break;
				case STATE_READING_PARAM:
					if (words.size() == 1)
					{
						if (words[0] == "}")
						{
							secondaryState = STATE_READING_HEADER;
							m_LinkComponentInstructions.push_back(currentInstruction);
							currentInstruction = nullptr;
						}
					}
					else if (words.size() == 3 && currentInstruction != nullptr)
					{
						currentInstruction->terms.push_back(MEM_FRAME_ALLOC_T(InstructionTerm, words[0], words[2]));
					}
					break;
				}
			}
				break;
			}
		}


		fclose(file);
		//Parse instruction data to create gameobjects and components and deserialize there data.
		m_MarkedGameObjects.clear();
		m_MarkedComponents.clear();

		//Add GameObjects

		for (std::vector<InstructionData*>::iterator it = m_AddGameObjectInstructions.begin();
			it != m_AddGameObjectInstructions.end();
			it++)
		{
			InstructionData* currentData = *it;
			if (currentData == nullptr)
			{
				Debug::Error("Gem","Problem loading scene file. (AddGameObject) InstructionData was null");
				return;
			}

			if (currentData->reservedParam0 != m_MarkedGameObjects.size())
			{
				Debug::ErrorFormat("Gem", nullptr, "Problem loading scene file. The current object index specified doesn't match the vector.");
				return;
			}

			//Create a gameobject and get its serialize flag to set. Then add it onto the list.
			GameObject * gameObject = MEM_POOL_ALLOC_T(GameObject);
			SInt32* flags = GetFlag(gameObject);
			*flags = currentData->reservedParam0;
			m_MarkedGameObjects.push_back(gameObject);
		}


		for (std::vector<InstructionData*>::iterator it = m_LinkGameObjectInstructions.begin();
			it != m_LinkGameObjectInstructions.end();
			it++)
		{
			InstructionData* currentData = *it;
			if (currentData == nullptr)
			{
				Debug::Error("Gem", "Problem loading scene file. (LinkGameObject) InstructionData was null");
				return;
			}

			GameObject* gameObject = nullptr;
			GameObject* parent = nullptr;


			if (currentData->reservedParam0 < 0 || currentData->reservedParam0 >= m_MarkedGameObjects.size())
			{
				Debug::WarningFormat("Gem", nullptr, "Failed to Link GameObject %d. Index is out of range.", currentData->reservedParam0);
				continue;
			}
			gameObject = m_MarkedGameObjects[currentData->reservedParam0];
			if (currentData->reservedParam1 > 0 && currentData->reservedParam1 < m_MarkedGameObjects.size())
			{
				parent = m_MarkedGameObjects[currentData->reservedParam1];
				if (parent != gameObject)
				{
					gameObject->SetParent(parent);
				}
			}

			Type type = gameObject->GetType();
			MethodInfo<GameObject, void, std::vector<InstructionTerm*>&>* method =
				dynamic_cast<MethodInfo<GameObject, void, std::vector<InstructionTerm*>&>*>(type.GetMethodInfo("OnDeserializeData"));
			if (method != nullptr)
			{
				method->GetMethod().Invoke(gameObject, currentData->terms);
			}
		}


		for (std::vector<InstructionData*>::iterator it = m_AddComponentInstructions.begin();
			it != m_AddComponentInstructions.end();
			it++)
		{

			InstructionData* currentData = *it;
			if (currentData == nullptr)
			{
				Debug::Error("Gem", "Problem loading scene file. (AddComponents) InstructionData was null");
				return;
			}
	
			GameObject* gameObject = nullptr;
			if (currentData->reservedParam1 < 0 || currentData->reservedParam1 >= m_MarkedGameObjects.size())
			{
				Debug::WarningFormat("Gem", nullptr, "Failed to Add Component %d. Index is out of range.", currentData->reservedParam0);
				continue;
			}
			gameObject = m_MarkedGameObjects[currentData->reservedParam1];
			SInt32* flags = GetFlag(gameObject);
			if (flags == nullptr || (*flags) != currentData->reservedParam1)
			{
				Debug::WarningFormat("Gem", nullptr, "Failed to Add Component %d. Index mismatch.", currentData->reservedParam0);
				continue;
			}

			Component* component = gameObject->AddComponent(currentData->typeName);
			if (component == nullptr)
			{
				Debug::WarningFormat("Gem", nullptr, "Failed to add a component to the gameobject. Possibly invalid type %s", currentData->typeName.c_str());
				continue;
			}
			flags = GetFlag(component);
			if (flags == nullptr)
			{
				m_MarkedComponents.push_back(component);
				Debug::Warning("Gem","Component does not have a flag");
				continue;
			}
			*flags = currentData->reservedParam0;
			m_MarkedComponents.push_back(component);
		}

		for (std::vector<InstructionData*>::iterator it = m_LinkComponentInstructions.begin();
			it != m_LinkComponentInstructions.end();
			it++)
		{
			InstructionData* currentData = *it;
			if (currentData == nullptr)
			{
				Debug::Error("Gem", "Failed to link component, bad link instruction.");
				continue;
			}

			
			if (currentData->reservedParam0 < 0 || currentData->reservedParam0 >= m_MarkedComponents.size())
			{
				Debug::Error("Gem", "Failed to link component, index out of range");
				continue;
			}
			Component* component = m_MarkedComponents[currentData->reservedParam0];
			Type type = component->GetType();
			MethodInfo<Component, void, std::vector<InstructionTerm*>&>* method =
				dynamic_cast<MethodInfo<Component, void, std::vector<InstructionTerm*>&>*>(type.GetMethodInfo("Component","OnDeserializeData"));
			if (method != nullptr)
			{
				method->GetMethod().Invoke(component, currentData->terms);
			}
		}

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
		//Terms are created in the constructor.

		m_AddGameObjectInstructions.push_back(instructionData);
	}

	void SceneFile::AddGameObject(SInt32 aID, SInt32 aParentID)
	{
		if (aID <= -1)
		{
			Debug::Error("Gem", "Cannot add gameobject because it has an invalid index.");
			return;
		}
		if (aID != m_AddGameObjectInstructions.size())
		{
			Debug::WarningFormat("Gem", nullptr, "Loading data may be corrupted. GameObject with ID %d should be %d", aID, m_AddGameObjectInstructions.size());
		}
		InstructionTokenID instructionTokenID = InstructionTokenID::AddGameObject;
		InstructionData* instructionData = NewInstruction();

		instructionData->tokenID = instructionTokenID;
		instructionData->tokenIDName = instructionTokenID;
		instructionData->typeName = Reflection::Runtime::TypeOf<GameObject>().GetName();
		instructionData->reservedParam0 = aID;
		instructionData->reservedParam1 = aParentID;
		instructionData->paramCount = -1; 
		//Terms are created in the constructor.

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

		m_AddComponentInstructions.push_back(instructionData);
	}

	void SceneFile::AddComponent(SInt32 aID, SInt32 aParentID, const std::string & aTypename)
	{
		if (aID < 0 || aParentID < 0)
		{
			Debug::Error("Gem", "Cannot add component because it has an invalid index.");
			return;
		}

		InstructionTokenID instructionTokenID = InstructionTokenID::AddComponent;
		InstructionData* instructionData = NewInstruction();
		instructionData->tokenID = instructionTokenID;
		instructionData->tokenIDName = instructionTokenID;
		instructionData->typeName = aTypename;
		instructionData->reservedParam0 = aID;
		instructionData->reservedParam1 = aParentID;
		instructionData->paramCount = -1;
		m_AddComponentInstructions.push_back(instructionData);
	}

	void SceneFile::LinkGameObject(GameObject* aGameObject)
	{
		//Get the flags..
		SInt32* flags = GetFlag(aGameObject);
		SInt32* parentFlags = GetFlag(aGameObject->GetParent());
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
		instructionData->reservedParam1 = parentFlags == nullptr ? -1 : *parentFlags;
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