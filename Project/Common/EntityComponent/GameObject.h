#ifndef GEM_GAME_OBJECT_H
#define GEM_GAME_OBJECT_H

#pragma region CHANGE LOG
///	-- April	3, 2015 - Nathan Hanlan - Removed IFormatter from GameObject class.
#pragma endregion

#include "../Core/GemAPI.h"
#include "../Core/BasicTypes.h"
#include "../Utilities/Utilities.h"
#include "../Math/Math.h"
#include "Component.h"
#include "Bounds.h"
#include "InitializationStep.h"

namespace Gem
{
	namespace EntityComponent
	{
		class InstructionTerm;
	}


	class Window;
	
	FORCE_EXPORT_META(GameObject);

	template class GEM_API std::vector<GameObject*>;
	template class GEM_API std::vector<Component*>;

	class GEM_API GameObject : public object
	{
		RDECLARE_CLASS(GameObject)
	public:
		GameObject();
		GameObject(const std::string & aName);
		~GameObject();

		static void Destroy(GameObject * aGameObject);

		//Getters and Setters
#pragma region ACCESSORS

		void SetActive(bool aFlag);
		bool IsActive();

		std::string GetName();
		void SetName(std::string aName);

		std::string GetTag();
		void SetTag(std::string aTag);

		int GetRenderMask();
		void SetRenderMask(int aMask);

		int GetPhysicsMask();
		void SetPhysicsMask(int aMask);

		GameObject * GetParent();
		void SetParent(GameObject * aParent);

		Vector3 GetPosition();
		void SetPosition(Vector3 aPosition);

		Quaternion GetRotation();
        void SetRotation(Quaternion aRotation);

		Vector3 GetScale();
		void SetScale(Vector3 aScale);

		Vector3 GetLocalPosition();
		void SetLocalPosition(Vector3 aLocalPosition);

        Quaternion GetLocalRotation();
        void SetLocalRotation(Quaternion aLocalRotation);

#pragma endregion

		Matrix4x4 GetLocalToWorldMatrix();

		void LookAt(Vector3 aPosition);
		
		void AddChild(GameObject * aChild);
		void AddChildren(const std::vector<GameObject*> & aChildren);
		void AddChildren(const Array<GameObject*> & aChildren);
		
		void RemoveChild(std::string aName);
		void RemoveChild(GameObject * aChild);
		void RemoveChildren(const std::vector<GameObject*> & aChildren);
		void RemoveChildren(const Array<GameObject*> & aChildren);

		//
		//
		////Component Getters
		Component * GetComponent(const std::string & aTypename);
		Component * GetComponent(const Type & aType);
		template<class COMPONENT>
		COMPONENT* GetComponent()
		{
			return (COMPONENT*)GetComponent(Reflection::Runtime::TypeOf<COMPONENT>());
		}
		Component * GetComponentInChildren(const std::string & aTypename);
		Component * GetComponentInChildren(const Type & aType);
		template<typename COMPONENT>
		COMPONENT* GetComponentInChildren()
		{
			return (COMPONENT*)GetComponentInChildren(Reflection::Runtime::TypeOf<COMPONENT>());
		}
		Component * GetComponentInParent(const std::string & aTypename);
		Component * GetComponentInParent(const Type & aType);
		template<typename COMPONENT>
		COMPONENT* GetComponentInParent()
		{
			return (COMPONENT*)GetComponentInParent(Reflection::Runtime::TypeOf<COMPONENT>());
		}
		
		std::vector<Component *> GetComponents(const std::string & aTypename);
		std::vector<Component *> GetComponents(const Type & aType);
		template<typename COMPONENT>
		std::vector<COMPONENT*> GetComponents()
		{
			return (COMPONENT*)GetComponents(Reflection::Runtime::TypeOf<COMPONENT>());
		}
		std::vector<Component *> GetComponentsInChildren(const std::string & aTypename);
		std::vector<Component *> GetComponentsInChildren(const Type & aType);
		template<typename COMPONENT>
		std::vector<COMPONENT*> GetComponentsInChildren()
		{
			return (COMPONENT*)GetComponentsInChildren(Reflection::Runtime::TypeOf<COMPONENT>());
		}
		std::vector<Component *> GetComponentsInParent(const std::string & aTypename);
		std::vector<Component *> GetComponentsInParent(const Type & aType);
		template<typename COMPONENT>
		std::vector<COMPONENT*> GetComponentsInParent()
		{
			return (COMPONENT*)GetComponentsInParent(Reflection::Runtime::TypeOf<COMPONENT>());
		}
		
		//Add Components
		Component * AddComponent(const std::string & aTypename);
		Component * AddComponent(const Type & aType);
		template<typename COMPONENT>
		COMPONENT * AddComponent()
		{
			return (COMPONENT*)AddComponent(Reflection::Runtime::TypeOf<COMPONENT>());
		}
		
		//Remove Components
		void RemoveComponent(Component * aComponent);

        Bounds GetBounds();
        void SetBounds(Bounds aBounds);

		template<typename ... ARGS>
		void BroadcastMessage(const std::string & aFunctionName, ARGS... args)
		{
			Type type = GetType();
			Reflection::MethodInfo<GameObject, void, ARGS...> * method = dynamic_cast<Reflection::MethodInfo<GameObject, void, ARGS...>*>(type.GetMethodInfo(aFunctionName));
			if (method != nullptr)
			{
				method->GetMethod().Invoke(this, args...);
			}

			//for (std::vector<GameObject*>::iterator it = m_Children.begin(); it != m_Children.end(); it++)
			//{
			//	(*it)->BroadcastMessage<ARGS...>(aFunctionName, args...);
			//}
		}

		template<typename ... ARGS>
		void BroadcastMessage(const std::string & aFunctionName, bool aSendComponents, ARGS... args)
		{
			if (aSendComponents)
			{
				Type type = GetType();
				Reflection::MethodInfo<GameObject, void, ARGS...> * method = dynamic_cast<Reflection::MethodInfo<GameObject, void, ARGS...>*>(type.GetMethodInfo(aFunctionName));
				if (method != nullptr)
				{
					method->GetMethod().Invoke(this, args...);
				}

				for (std::vector<Component*>::iterator it = m_Components.begin(); it != m_Components.end(); it++)
				{
					(*it)->SendMessage<ARGS...>(aFunctionName, args...);
				}

				for (std::vector<GameObject*>::iterator it = m_Children.begin(); it != m_Children.end(); it++)
				{
					(*it)->BroadcastMessage<ARGS...>(aFunctionName,aSendComponents, args...);
				}
			}
			else
			{
				BroadcastMessage<ARGS...>(aFunctionName, args...);
			}
		}

		GameObject(SInt32 aInternalHash);
	private:
		

		std::string m_Name;
		std::string m_Tag;
		UInt32 m_RenderMask;
		UInt32 m_PhysicsMask;
		bool m_IsActive;
		GameObject * m_Parent;
		std::vector<GameObject*> m_Children;
		std::vector<Component*> m_Components;

		//Transform Variables.
		Vector3 m_Position;
        Quaternion m_Rotation;
		Vector3 m_Scale;

        Bounds m_Bounds;
		
		
		
		/** This is used by ECSerializer to find its location faster in the data stream.*/
		SInt32 m_SerializerFlag;
		RDECLARE_PRIVATE_MEMBER(GameObject, m_SerializerFlag)
		
		
		/**
		* This method is used to tell the ECSerializer how many instruction terms it will be saving.
		* @return Return the number of elements to
		*/
		int OnPreSerializeData();
		RDECLARE_PRIVATE_FUNCTION(GameObject, OnPreSerializeData)
		
		
		/**
		* This method is intended to get called from ECSerializer to gets its instructions data. ("Terms")
		* @param aCount The amount of terms being passed in.
		* @param aTerms A dynamic array of terms to modify, they are already allocated but will only last for the frame.
		*/
		void OnSerializeData(const int & aCount, EntityComponent::InstructionTerm** aTerms);
		RDECLARE_PRIVATE_FUNCTION(GameObject, OnSerializeData)

		/**
		* This method gets called by gameobject itself to register itself with the scene.
		*/
		void InternalOnCreate();
		RDECLARE_PRIVATE_FUNCTION(GameObject, InternalOnCreate)
		/**
		* This method gets called by the gameobject itself when it's destroyed. Destroys any attached components and children.
		*/
		void InternalOnDestroy();
		RDECLARE_PRIVATE_FUNCTION(GameObject, InternalOnDestroy)


		//Update calls..
		/**
		* This method gets called by the active scene through reflection. To update the gameobject's component's state's.
		*/
		void InternalOnStateUpdate();
		RDECLARE_PRIVATE_FUNCTION(GameObject, InternalOnStateUpdate)
		/**
		* This method gets called by the active scene through reflection to update the gameobjects components.
		* @param aStep The current update step. 0 = Normal Update, 1 = Late Update 
		*/
		void InternalOnUpdate(int aStep);
		RDECLARE_PRIVATE_FUNCTION(GameObject, InternalOnUpdate)
		/**
		* This method gets called by the active scene through reflection to do a physics update on the gameobjects components.
		* @param aStep The current update step, There is only one for now...
		*/
		void InternalOnPhysicsUpdate(int aStep);
		RDECLARE_PRIVATE_FUNCTION(GameObject, InternalOnPhysicsUpdate)
		/**
		* This method gets called by the active scene through reflection to do a render update.
		* @param aStep The current render operation. 0 = pre render, 1 = render, 2 = post render.
		*/
		void InternalOnRenderUpdate(int aStep);
		RDECLARE_PRIVATE_FUNCTION(GameObject, InternalOnRenderUpdate)

		
		void OnWindowFocus(Window * aWindow);
		void OnWindowUnfocus(Window * aWindow);
		void OnWindowClose(Window * aWindow);
		void OnWindowChangeSize(Window * aWindow, int aWidth, int aHeight);

		friend class SceneGraph;
	};

	TYPE_DEFINE(GameObject)
}

#endif
