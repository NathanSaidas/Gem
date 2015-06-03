#include "GameObject.h"
#include "Scene.h"
#include "../Application/Application.h"
#include "../Memory/Memory.h"
#include "EntityComponentSerialization.h"
#include "InstructionTerm.h"

using namespace Gem::Debugging;

namespace Gem
{
	RDEFINE_CLASS(GameObject, object)
	RDEFINE_PRIVATE_MEMBER(GameObject, m_SerializerFlag, SInt32)


	
	

	GameObject::GameObject() : object(),
		m_Name("New GameObject"),
		m_Tag(""),
		m_RenderMask(0),
		m_PhysicsMask(0),
		m_IsActive(false),
		m_Parent(nullptr),
		m_Position(Vector3::Zero()),
		m_Rotation(Quaternion::Identity()),
		m_Scale(Vector3::Zero()),
		m_SerializerFlag(-1)
	{
		InternalOnCreate();
	}
	GameObject::GameObject(const std::string & aName) : object(),
		m_Name(aName),
		m_Tag(""),
		m_RenderMask(0),
		m_PhysicsMask(0),
		m_IsActive(false),
		m_Parent(nullptr),
		m_Position(Vector3::Zero()),
		m_Rotation(Quaternion::Identity()),
		m_Scale(Vector3::Zero()),
		m_SerializerFlag(-1)
	{
		InternalOnCreate();
	}

	GameObject::GameObject(SInt32 aInternalHash) : object(),
		m_Name("__ROOT__"),
		m_Tag(""),
		m_RenderMask(0),
		m_PhysicsMask(0),
		m_IsActive(false),
		m_Parent(nullptr),
		m_Position(Vector3::Zero()),
		m_Rotation(Quaternion::Identity()),
		m_Scale(Vector3::Zero()),
		m_SerializerFlag(-1),
		m_Owner(nullptr),
		m_HashCode(0)
	{
		//Does not call InternalOnCreate this constructor is intended to be used for by SceneGraphs.
	}

	GameObject::~GameObject()
	{
		if (m_Owner != nullptr)
		{
			InternalOnDestroy();
		}
		
	}
	
	void GameObject::Destroy(GameObject * aGameObject)
	{
		if (aGameObject != nullptr)
		{
			Scene * scene = Application::GetCurrentScene();
			if (scene != nullptr)
			{
				scene->Register(aGameObject);
			}
		}
	}


	

	

	void GameObject::SetActive(bool aFlag)
	{
		if (!m_IsActive && aFlag)
		{
			//OnEnable();
		}
		else if (m_IsActive && !aFlag)
		{
			//OnDisable();
		}
		m_IsActive = aFlag;
	}
	bool GameObject::IsActive()
	{
		return m_IsActive;
	}

	std::string GameObject::GetName()
	{
		return m_Name;
	}
	void GameObject::SetName(std::string aName)
	{
		//If the name is not the same. The scene might have to rehash.
		if (aName != m_Name && m_Owner != nullptr)
		{
			//Check and see what the new hash will be.
			UInt32 newHash = m_Owner->HashString(aName);
			//If the codes are different the scene will need to rehash this gameobject.
			if (newHash != m_HashCode)
			{
				m_Owner->OnGameObjectWillChangeName(this);
				m_Name = aName;
				m_HashCode = newHash;
				m_Owner->OnGameObjectChangedName(this);
			}
			else
			{
				m_Name = aName;
			}
		}
	}

	std::string GameObject::GetTag()
	{
		return m_Tag;
	}
	void GameObject::SetTag(std::string aTag)
	{
		m_Tag = aTag;
	}

	int GameObject::GetRenderMask()
	{
		return m_RenderMask;
	}
	void GameObject::SetRenderMask(int aMask)
	{
		m_RenderMask = aMask;
	}

	int GameObject::GetPhysicsMask()
	{
		return m_PhysicsMask;
	}
	void GameObject::SetPhysicsMask(int aMask)
	{
		m_PhysicsMask = aMask;
	}

	GameObject * GameObject::GetParent()
	{
		if (m_Owner->IsRoot(m_Parent))
		{
			return nullptr;
		}
		return m_Parent;
	}
	void GameObject::SetParent(GameObject * aParent)
	{
		if (m_Parent == aParent)
		{
			return;
		}
		if (m_Parent != nullptr)
		{
			m_Parent->RemoveChild(this);
			if (m_Owner != nullptr)
			{
				m_Owner->OnGameObjectWillChangeParent(this);
			}
		}
		m_Parent = aParent;
		if (m_Parent != nullptr)
		{
			m_Parent->AddChild(this);
		}
		if (m_Owner != nullptr)
		{
			m_Owner->OnGameObjectChangedParent(this);
		}
	}

	Vector3 GameObject::GetPosition()
	{
		return m_Position;
	}
	void GameObject::SetPosition(Vector3 aPosition)
	{
		m_Position = aPosition;
	}

    Quaternion GameObject::GetRotation()
	{
		return m_Rotation;
	}
    void GameObject::SetRotation(Quaternion aRotation)
	{
		m_Rotation = aRotation;
	}

	Vector3 GameObject::GetScale()
	{
		return m_Scale;
	}

	void GameObject::SetScale(Vector3 aScale)
	{
		m_Scale = aScale;
	}

	Vector3 GameObject::GetLocalPosition()
	{
		//TODO
		return m_Position;;
	}
	void GameObject::SetLocalPosition(Vector3 aLocalPosition)
	{
		if (m_Parent != nullptr)
		{
			m_Position = m_Parent->GetPosition() + aLocalPosition;
		}
		else
		{
			m_Position = aLocalPosition;
		}
	}

    Quaternion GameObject::GetLocalRotation()
	{
		//TODO
		return m_Rotation;
	}
    void GameObject::SetLocalRotation(Quaternion aLocalRotation)
	{
		if (m_Parent != nullptr)
		{
			m_Rotation = m_Parent->GetRotation() * aLocalRotation;
		}
		else
		{
			m_Rotation = aLocalRotation;
		}
	}

	Matrix4x4 GameObject::GetLocalToWorldMatrix()
	{
		Matrix4x4 matrix = Matrix4x4::Identity();
		if (m_Parent != nullptr)
		{
			matrix = m_Parent->GetLocalToWorldMatrix();
			matrix = Matrix4x4::TRS(m_Position, m_Rotation, m_Scale);
		}
		else
		{
			matrix = Matrix4x4::TRS(m_Position, m_Rotation, m_Scale);
		}
		return matrix;
	}
	void GameObject::LookAt(Vector3 aPosition)
	{
        Vector3 direction = aPosition - GetPosition();
        Matrix4x4 mat = Matrix4x4::LookAt(GetPosition(), -(direction.Normalized()));
		m_Rotation = mat.GetRotation();
		//Quaternion inverse = Quaternion::Inverse(mat.GetRotation());
        //m_Rotation.w = inverse.y;
        //m_Rotation.x = inverse.z;
        //m_Rotation.y = -inverse.w;
        //m_Rotation.z = -inverse.x;
	}

	
	void GameObject::AddChild(GameObject * aChild)
	{
		if (aChild != nullptr && aChild != this)
		{
			if (!Utilities::Exists<GameObject*>(m_Children, aChild))
			{
				m_Children.push_back(aChild);

				if (aChild->m_Parent != this && aChild->m_Parent != nullptr)
				{
					aChild->m_Parent->RemoveChild(aChild);
					aChild->m_Parent = this;
				}
			}
		}
	}
	void GameObject::AddChildren(const std::vector<GameObject*> & aChildren)
	{
		for (std::vector<GameObject*>::const_iterator it = aChildren.begin(); it != aChildren.end(); it++)
		{
			AddChild(*it);
		}
	}
	void GameObject::AddChildren(const Array<GameObject*> & aChildren)
	{
		for (unsigned int i = 0; i < aChildren.GetCount(); i++)
		{
			AddChild(aChildren[i]);
		}
	}
	
	void GameObject::RemoveChild(std::string aName)
	{
		for (std::vector<GameObject*>::iterator it = m_Children.begin(); it != m_Children.end(); it++)
		{
			if ((*it)->GetName() == aName)
			{
				m_Children.erase(it);
				break;
			}
		}
	}
	void GameObject::RemoveChild(GameObject * aChild)
	{
		if (aChild != this)
		{
			Utilities::Remove<GameObject*>(m_Children, aChild);
		}
	}
	void GameObject::RemoveChildren(const std::vector<GameObject*> & aChildren)
	{
		for (std::vector<GameObject*>::const_iterator it = aChildren.begin(); it != aChildren.end(); it++)
		{
			RemoveChild(*it);
		}
	}
	void GameObject::RemoveChildren(const Array<GameObject*> & aChildren)
	{
		for (unsigned int i = 0; i < aChildren.GetCount(); i++)
		{
			RemoveChild(aChildren[i]);
		}
	}
	//
	Component * GameObject::GetComponent(const std::string & aTypename)
	{
		for (std::vector<Component*>::iterator it = m_Components.begin(); it != m_Components.end(); it++)
		{
			if ((*it)->GetType().GetName() == aTypename)
			{
				return *it;
			}
		}
		return nullptr;
	}
	Component * GameObject::GetComponent(const Type & aType)
	{
		return GetComponent(aType.GetName());
	}
	Component * GameObject::GetComponentInChildren(const std::string & aTypename)
	{
		for (std::vector<GameObject*>::iterator it = m_Children.begin(); it != m_Children.end(); it++)
		{
			Component * component = (*it)->GetComponent(aTypename);
			if (component != nullptr)
			{
				return component;
			}
		}
		return nullptr;
	}
	Component * GameObject::GetComponentInChildren(const Type & aType)
	{
		for (std::vector<GameObject*>::iterator it = m_Children.begin(); it != m_Children.end(); it++)
		{
			Component * component = (*it)->GetComponent(aType);
			if (component != nullptr)
			{
				return component;
			}
		}
		return nullptr;
	}
	Component * GameObject::GetComponentInParent(const std::string & aTypename)
	{
		if (m_Parent != nullptr)
		{
			return m_Parent->GetComponent(aTypename);
		}
		return nullptr;
	}
	Component * GameObject::GetComponentInParent(const Type & aType)
	{
		if (m_Parent != nullptr)
		{
			return m_Parent->GetComponent(aType);
		}
		return nullptr;
	}
	std::vector<Component *> GameObject::GetComponents(const std::string & aTypename)
	{
		std::vector<Component*> components;
		for (std::vector<Component*>::iterator it = m_Components.begin(); it != m_Components.end(); it++)
		{
			if ((*it)->GetType().GetName() == aTypename)
			{
				components.push_back(*it);
			}
		}
		return components;
	}
	std::vector<Component *> GameObject::GetComponents(const Type & aType)
	{
		return GetComponents(aType.GetName());
	}
	std::vector<Component *> GameObject::GetComponentsInChildren(const std::string & aTypename)
	{
		std::vector<Component*> components;
	
		for (std::vector<GameObject*>::iterator it = m_Children.begin(); it != m_Children.end(); it++)
		{
			std::vector<Component *> childComponents = (*it)->GetComponents(aTypename);
			for (std::vector<Component*>::iterator childIT = childComponents.begin(); childIT != childComponents.end(); childIT++)
			{
				components.push_back(*childIT);
			}
		}
		return components;
	}
	std::vector<Component *> GameObject::GetComponentsInChildren(const Type & aType)
	{
		return GetComponentsInChildren(aType.GetName());
	}
	std::vector<Component *> GameObject::GetComponentsInParent(const std::string & aTypename)
	{
		if (m_Parent != nullptr)
		{
			return m_Parent->GetComponents(aTypename);
		}
		return std::vector<Component*>();
	}
	std::vector<Component *> GameObject::GetComponentsInParent(const Type & aType)
	{
		return GetComponentsInParent(aType.GetName());
	}
	Component * GameObject::AddComponent(const std::string & aTypename)
	{
		return AddComponent(Reflection::Runtime::TypeOf(aTypename));
	}
	Component * GameObject::AddComponent(const Type & aType)
	{
		if (Reflection::Runtime::IsBadType(aType))
		{
			Debug::Error("Gem","Failed to add a component. The type is invalid");
			return nullptr;
		}
		//Property prop = aType.GetProperty("DisallowMultipleComponent");
	
		//if (!prop.IsValid() && GetComponent(aType.GetName()) != nullptr)
		//{
		//	DEBUG_LOG("Failed to add a component. %s contains the %s property and a component of the type already exists on the gameobject.", aType.GetName().c_str(), "DisallowMultipleComponent");
		//	return nullptr;
		//}
		//Initialize the component with the memory manager and invoke the constructor through reflection.
		Component * component = (Component*)MEM_POOL_ALLOC(aType.GetSize(), aType.GetAlignment());
		if (component != nullptr)
		{
			component->m_GameObject = this;
			component = (Component*)aType.GetConstructor().Invoke(component);
			m_Components.push_back(component);
			component->OnRegister();
		}
		else
		{
			Debug::Error("Gem", "Failed to allocate a component, Possibly out of memory");
		}
		return component;
	}
	void GameObject::RemoveComponent(Component * aComponent)
	{
		Utilities::Remove<Component*>(m_Components, aComponent);
	}

    Bounds GameObject::GetBounds()
    {
        return m_Bounds;
    }
    void GameObject::SetBounds(Bounds aBounds)
    {
        m_Bounds = aBounds;
    }



	RDEFINE_PRIVATE_FUNCTION(GameObject, OnSerializeData, void, std::vector<InstructionTerm*>&)
	void GameObject::OnSerializeData(std::vector<InstructionTerm*> & aTerms)
	{
		aTerms.push_back(SERIALIZE_TERM(m_Name));
		aTerms.push_back(SERIALIZE_TERM(m_Tag));
		aTerms.push_back(SERIALIZE_TERM(m_RenderMask));
		aTerms.push_back(SERIALIZE_TERM(m_RenderMask));
		aTerms.push_back(SERIALIZE_TERM(m_PhysicsMask));
		aTerms.push_back(SERIALIZE_TERM(m_IsActive));
		aTerms.push_back(SERIALIZE_TERM(m_Position));
		aTerms.push_back(SERIALIZE_TERM(m_Rotation));
		aTerms.push_back(SERIALIZE_TERM(m_Scale));
	}

	RDEFINE_PRIVATE_FUNCTION(GameObject, InternalOnCreate, void)
	void GameObject::InternalOnCreate()
	{
		Scene * scene = Application::GetCurrentScene();
		m_Owner = scene;
		m_HashCode = scene->HashString(m_Name);
		scene->Register(this);
	}
	RDEFINE_PRIVATE_FUNCTION(GameObject, InternalOnDestroy, void)
	void GameObject::InternalOnDestroy()
	{
		Scene * scene = Application::GetCurrentScene();
		if (scene == nullptr)
		{
			return;
		}
		scene->Unregister(this);

		//Destroy All Children...
		for (int i = 0; i < m_Children.size(); i++)
		{
			GameObject * gameObject = m_Children[i];
			Memory::AllocatorType allocType = Memory::MemoryUtils::GetAllocatorType(gameObject);
			switch (allocType)
			{
			case Memory::AllocatorType::Pool:
				MEM_POOL_DEALLOC_T(gameObject, GameObject);
				break;
			case Memory::AllocatorType::Stack:
				MEM_STACK_DEALLOC_T(gameObject, GameObject);
				break;
			default:
				Debug::Error("Scene", "Invalid GameObject allocation made.");
				break;
			}

		}

		//Destroy all components.
		for (int i = m_Components.size() - 1; i >= 0; i--)
		{
			Component * component = m_Components[i];
			Type type = component->GetType();
			type.GetDestructor().Invoke(component);
			MEM_POOL_DEALLOC(component, type.GetSize());
		}
		m_Components.clear();
	}

	RDEFINE_PRIVATE_FUNCTION(GameObject, InternalOnStateUpdate, void)
	void GameObject::InternalOnStateUpdate()
	{
		//Update Components states...
	}

	RDEFINE_PRIVATE_FUNCTION(GameObject, InternalOnUpdate, void, int)
	void GameObject::InternalOnUpdate(int aStep)
	{
		if (aStep == 0)
		{
			for (std::vector<Component*>::iterator it = m_Components.begin(); it != m_Components.end(); it++)
			{
				if ((*it)->IsReceivingGameObjectMessages())
				{
					(*it)->Update();
				}
			}
		}
		else if (aStep == 1)
		{
			for (std::vector<Component*>::iterator it = m_Components.begin(); it != m_Components.end(); it++)
			{
				if ((*it)->IsReceivingGameObjectMessages())
				{
					(*it)->LateUpdate();
				}
			}
		}
	}

	RDEFINE_PRIVATE_FUNCTION(GameObject, InternalOnPhysicsUpdate, void, int)
	void GameObject::InternalOnPhysicsUpdate(int aStep)
	{
		for (std::vector<Component*>::iterator it = m_Components.begin(); it != m_Components.end(); it++)
		{
			if ((*it)->IsReceivingGameObjectMessages())
			{
				(*it)->FixedUpdate();
			}
		}
	}

	RDEFINE_PRIVATE_FUNCTION(GameObject, InternalOnRenderUpdate, void, int)
	void GameObject::InternalOnRenderUpdate(int aStep)
	{
		if (aStep == 0)
		{
			for (std::vector<Component*>::iterator it = m_Components.begin(); it != m_Components.end(); it++)
			{
				if ((*it)->IsReceivingGameObjectMessages())
				{
					(*it)->OnPreRender();
				}
			}
		}
		else if (aStep == 1)
		{
			for (std::vector<Component*>::iterator it = m_Components.begin(); it != m_Components.end(); it++)
			{
				if ((*it)->IsReceivingGameObjectMessages())
				{
					(*it)->OnRender();
				}
			}
		}
		else if (aStep == 2)
		{
			for (std::vector<Component*>::iterator it = m_Components.begin(); it != m_Components.end(); it++)
			{
				if ((*it)->IsReceivingGameObjectMessages())
				{
					(*it)->OnPostRender();
				}
			}
		}

		
	}

	void GameObject::OnWindowFocus(Window * aWindow)
	{

	}
	void GameObject::OnWindowUnfocus(Window * aWindow)
	{

	}
	void GameObject::OnWindowClose(Window * aWindow)
	{

	}
	void GameObject::OnWindowChangeSize(Window * aWindow, int aWidth, int aHeight)
	{

	}
}
	