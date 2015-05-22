#include "ECSerializer.h"

#include "../Engine.h"

using namespace Gem::Reflection;
using namespace Gem::Debugging;

namespace Gem
{
	namespace EntityComponent
	{
		void ECSerializer::TestBed(GameObject * aGameObject)
		{
			if (aGameObject == nullptr)
			{
				return;
			}
			//Get the type data from gameobjects
			Type type = aGameObject->GetType();
			//Search for its m_SerializerFlag with reflection.
			MemberInfo serializerFlagMember = type.GetMember("m_SerializerFlag");
			if (serializerFlagMember.GetClassName() == nullptr)
			{
				return;
			}
			//Get the memory address of the serializer flag variable.
			UInt32 * serializerFlag = (UInt32*)serializerFlagMember.GetOffsetPointer(aGameObject);

			//Set its place in memory.
			*serializerFlag = 32;



			//Go through each gameobject in the scene...
			//Mark the gameobject like so...


		}

		void ECSerializer::AddGameObject(GameObject * aGameObject)
		{
			//Fail Conditions... 
			/**
			* Null GameObject
			* GameObject already exists.
			* GameObject Parent has not been added yet
			* Fail to create flags.
			*/

			SInt32* flags = MarkGameObject(aGameObject);
			SInt32* parentFlags = nullptr;
			InstructionTokenID instructionTokenID = InstructionTokenID(InstructionTokenID::AddGameObject);
			if (flags == nullptr)
			{
				return;
			}

			
			// Generate instruction data using Frame Allocation. (This data will only be valid for this frame).
			InstructionData* instruction = NewInstruction();
			// InstructionTokenID is of type Gem::Enum which is a custom type which can be used as SInt32 (signed int) or CString (const char *) implicitly.
			// Add the token .
			instruction->tokenID = instructionTokenID;
			instruction->tokenIDName = instructionTokenID;
			// Specify the index and parent index.
			instruction->reservedParam0 = *flags;
			instruction->reservedParam1 = parentFlags != nullptr ? *parentFlags : -1;
			// Ignore these fields...
			instruction->paramCount = -1;
			instruction->terms = nullptr;

			//Add the instruction.
			m_AddGameObjectInstructions.push_back(instruction);
		}

		void ECSerializer::LinkGameObject(GameObject* aGameObject)
		{
			//Fail Conditions... 
			/**
			* Null GameObject
			* Fail to get flags.
			*/
			SInt32* flags = GetFlag(aGameObject);
			InstructionTokenID instructionTokenID = InstructionTokenID(InstructionTokenID::LinkGameObject);
			if (flags == nullptr || *flags == -1)
			{
				return;
			}


			//Create the instruction and fill out the basic data.
			InstructionData* instruction = NewInstruction();
			instruction->tokenID = instructionTokenID;
			instruction->tokenIDName = instructionTokenID;
			instruction->reservedParam0 = *flags;
			instruction->reservedParam1 = -1;

			//Use reflection to find two methods. OnPreSerializeData and OnSerializeData
			Type type = aGameObject->GetType();
			MethodInfo<GameObject, int> * preserializeMethod = nullptr;
			MethodInfo<GameObject, void, const int&, InstructionTerm **> * serializeMethod = nullptr;
			Member * member = type.GetMethodInfo("OnPreSerializeData");
			if (member != nullptr)
			{
				preserializeMethod = dynamic_cast<MethodInfo<GameObject, int>*>(member);
			}
			member = type.GetMethodInfo("OnSerializeData");
			if (member != nullptr)
			{
				serializeMethod = dynamic_cast<MethodInfo<GameObject, void, const int&, InstructionTerm **>*>(member);
			}

			//Exit: Methods dont exist or failed to cast...
			if (preserializeMethod == nullptr || serializeMethod == nullptr)
			{
				return;
			}
			
			//Get How Many Parameters are about to get created...
			instruction->paramCount = preserializeMethod->GetMethod().Invoke(aGameObject);
			if (instruction->paramCount > 0)
			{
				//Initialize all terms 
				instruction->terms = new InstructionTerm *[instruction->paramCount]();
				for (int i = 0; i < instruction->paramCount; i++)
				{
					instruction->terms[i] = MEM_FRAME_ALLOC_T(InstructionTerm);
				}
				//Fill Term Data.
				serializeMethod->GetMethod().Invoke(aGameObject, instruction->paramCount, instruction->terms);
			}
			else
			{
				instruction->terms = nullptr;
			}

			m_LinkGameObjectInstructions.push_back(instruction);
		}

		void  ECSerializer::AddComponents(Component* aComponent)
		{

		}

		void ECSerializer::WriteFile()
		{
			FILE * file = fopen("filename.txt", "w");
			if (file == NULL)
			{
				return;
			}

			//Write all AddGameObject instructions...
			InstructionTokenID tokenID = InstructionTokenID::AddGameObject;
			fprintf(file, "begin %s", tokenID.ToString().c_str());
			for (int i = 0; i < m_AddGameObjectInstructions.size(); i++)
			{
				InstructionData * current = m_AddGameObjectInstructions[i];
				fprintf(file, "%s %d %d\n", current->tokenIDName, current->reservedParam0, current->reservedParam1);
			}
			fprintf(file, "end %s", tokenID.ToString().c_str());


			//Write all Link GameObject Instructions...
			tokenID = InstructionTokenID::LinkGameObject;
			fprintf(file, "begin %s", tokenID.ToString().c_str());
			
			for (int i = 0; i < m_LinkGameObjectInstructions.size(); i++)
			{
				InstructionData * current = m_LinkGameObjectInstructions[i];
				fprintf(file, "%s [%d]\n count = %d{\n", current->tokenIDName, current->reservedParam0, current->paramCount);

				for (int j = 0; j < current->paramCount; j++)
				{
					fprintf(file, "%s\n", current->terms[j]->ToString().c_str());
				}
				
				fprintf(file, "}\n");
			}
			fprintf(file, "end %s", tokenID.ToString().c_str());

			//Write All AddComponent Instructions...
			

			//Write All LinkComponent Instructions...
		}

		InstructionData * ECSerializer::NewInstruction()
		{
			return static_cast<InstructionData*>(MEM_FRAME_ALLOC(sizeof(InstructionData), __alignof(InstructionData)));
		}

		void ECSerializer::GenerateInstruction(GameObject * aGameObject, InstructionTokenID & aInstructionType)
		{
			//Fail Conditions... 
			/**
			* Null GameObject
			* Invalid Instruction type
			* GameObject already exists.
			* GameObject Parent has not been added yet
			* Fail to create flags.
			*/

			//Exit: Null gameobject or (DEBUG ONLY) gameobject exists.
			if (aGameObject == nullptr)
			{
				return;
			}

			//The m_SerializedFlag of the gameobject.
			SInt32* flags = MarkGameObject(aGameObject);
			SInt32* parentFlags = nullptr;
			//Exit: Something went wrong...
			if (flags == nullptr)
			{
				return;
			}

			//Create Instructions...
			
			//Add GameObject...
			

			
			//Allocate instruction data for a singleframe...
			InstructionData* instruction = static_cast<InstructionData*>(MEM_FRAME_ALLOC(sizeof(InstructionData), __alignof(InstructionData)));
			// InstructionTokenID is of type Gem::Enum which is a custom type which can be used as SInt32 (signed int) or CString (const char *) implicitly.
			instruction->tokenID = aInstructionType; 
			instruction->tokenIDName = aInstructionType;

			switch (aInstructionType)
			{
					case InstructionTokenID::AddGameObject:
					{
						//If the instruction token id is add gameobject.. we can specify this instructions rparam0 is to be the ID of the gameobject in the list.
						instruction->reservedParam0 = *flags;

						//If the gameobject has a parent, the parent should have already been processed by now.
						GameObject* parent = aGameObject->GetParent();
						if (parent != nullptr)
						{
							parentFlags = GetFlag(parent);
							instruction->reservedParam1 = *parentFlags;
						}
						else
						{
							//No parent bad flag.
							instruction->reservedParam1 = -1;
						}
					}
					break;
			}

			
			
		}

		SInt32 ECSerializer::GetGameObjectIndex(GameObject * aGameObject)
		{
			SInt32 * flag = GetFlag(aGameObject);
			if (flag != nullptr)
			{
				return *flag;
			}
			return -1;
		}

		SInt32 * ECSerializer::GetFlag(GameObject * aGameObject)
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
			return (SInt32*)serializerFlagMember.GetOffsetPointer(aGameObject);
		}

		SInt32 * ECSerializer::MarkGameObject(GameObject * aGameObject)
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

		SInt32 * ECSerializer::MarkComponent(Component * aComponent)
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
}