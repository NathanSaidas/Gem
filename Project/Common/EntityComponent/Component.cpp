#include "Component.h"
#include "GameObject.h"

namespace Gem
{
	RDEFINE_CLASS(Component, object)

	Component::Component() : object()
	{

	}
	Component::~Component()
	{

	}

	void Component::OnRegister()
	{

	}
	void Component::OnInitialize()
	{

	}
	void Component::OnLateInitialize()
	{

	}
	void Component::OnDestroy()
	{

	}
	void Component::OnLateDestroy()
	{

	}
	void Component::Update()
	{

	}
	void Component::FixedUpdate()
	{

	}
	void Component::LateUpdate()
	{

	}
	void Component::OnPreRender()
	{

	}
	void Component::OnRender()
	{

	}
	void Component::OnPostRender()
	{

	}

    void Component::OnSerializeEditor(IFormatter * aFormatter, Stream & aStream)
    {
        if (aFormatter != nullptr)
        {


        }
    }
    void Component::OnDeserializeEditor(IFormatter * aFormatter, Stream & aStream)
    {

    }

	GameObject * Component::GetGameObject()
	{
		return m_GameObject;
	}

	std::string Component::GetName()
	{
		return m_GameObject->GetName();
	}
	void Component::SetName(const std::string & aName)
	{
		m_GameObject->SetName(aName);
	}

	std::string Component::GetTag()
	{
		return m_GameObject->GetTag();
	}
	void Component::SetTag(const std::string & aName)
	{
		m_GameObject->SetTag(aName);
	}

	int Component::GetRenderMask()
	{
		return m_GameObject->GetRenderMask();
	}
	void Component::SetRenderMask(int aMask)
	{
		m_GameObject->SetRenderMask(aMask);
	}

	int Component::GetPhysicsMask()
	{
		return m_GameObject->GetPhysicsMask();
	}
	void Component::SetPhysicsMask(int aMask)
	{
		m_GameObject->SetPhysicsMask(aMask);
	}


	Component * Component::GetComponent(const std::string & aTypename)
	{
		return m_GameObject->GetComponent(aTypename);
	}
	Component * Component::GetComponent(const Type & aType)
	{
		return m_GameObject->GetComponent(aType);
	}
	Component * Component::GetComponentInChildren(const std::string & aTypename)
	{
		return m_GameObject->GetComponentInChildren(aTypename);
	}
	Component * Component::GetComponentInChildren(const Type & aType)
	{
		return m_GameObject->GetComponentInChildren(aType);
	}
	Component * Component::GetComponentInParent(const std::string & aTypename)
	{
		return m_GameObject->GetComponentInParent(aTypename);
	}
	Component * Component::GetComponentInParent(const Type & aType)
	{
		return m_GameObject->GetComponentInParent(aType);
	}
	std::vector<Component*> Component::GetComponents(const std::string & aTypename)
	{
		return m_GameObject->GetComponents(aTypename);
	}
	std::vector<Component*> Component::GetComponents(const Type & aType)
	{
		return m_GameObject->GetComponents(aType);
	}
	std::vector<Component*> Component::GetComponentsInChildren(const std::string & aTypename)
	{
		return m_GameObject->GetComponentsInChildren(aTypename);
	}
	std::vector<Component*> Component::GetComponentsInChildren(const Type & aType)
	{
		return m_GameObject->GetComponentsInChildren(aType);
	}
	std::vector<Component*> Component::GetComponentsInParent(const std::string & aTypename)
	{
		return m_GameObject->GetComponentsInParent(aTypename);
	}
	std::vector<Component*> Component::GetComponentsInParent(const Type & aType)
	{
		return m_GameObject->GetComponentsInParent(aType);
	}

}