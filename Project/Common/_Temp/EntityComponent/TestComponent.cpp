#include "TestComponent.h"
#include <glm\gtc\matrix_transform.hpp>
#include <stddef.h>
#include <Windows.h>




namespace Gem
{
	RDEFINE_CLASS(TestComponent, "")

	TestComponent::TestComponent() : Component()
	{

	}
	TestComponent::~TestComponent()
	{

	}

	void TestComponent::OnRegister()
	{
		//DEBUG_LOG("Registered %s", GetType().GetName().c_str());

	}
	
	void TestComponent::OnInitialize()
	{
		//DEBUG_LOG("Initialized %s", GetType().GetName().c_str());
		////Create Input Axis
		//Input * input = Input::Instance();
		//input->CreateAxis("Vertical", AxisCode::W, AxisCode::S);
		//input->CreateAxis("Horizontal", AxisCode::A, AxisCode::D);
		//
		//input->CreateAxis("Light V", AxisCode::T, AxisCode::G);
		//input->CreateAxis("Light H", AxisCode::F, AxisCode::H);
		//input->CreateAxis("Light Y", AxisCode::Y, AxisCode::R);
		//
		//input->CreateAxis("Target V", AxisCode::Up, AxisCode::Down);
		//input->CreateAxis("Target H", AxisCode::Left, AxisCode::Right);
		//
		//m_Camera = GetComponentInChildren<Camera>();
		//m_Renderer = GetComponentInChildren<Renderer>();
		//
		////m_Mesh.MakeUnique(Geometry::CreateCube(2.0f, 2.0f, 2.0f, Color::Crimson(), Memory::AllocatorType::Pool));
		//m_Mesh.MakeUnique(Geometry::CreatePlane(25.0f, 25.0f, Color::Crimson(), Memory::AllocatorType::Pool));
		//
		//Pointer<ImageTexture> texture = ResourceDatabase::LoadTexture("Wall3.png");
		//Pointer<Shader> shader = ResourceDatabase::LoadShader("Shaders\\Built-In\\DefaultShader.glsl");
		//
		//if (!texture.IsAlive())
		//{
		//	DEBUG_LOG("Missing Wall3.png");
		//}
		//else
		//{
		//	m_Material->SetTexture(texture.Cast<Texture>());
		//}
		//if (!shader.IsAlive())
		//{
		//	DEBUG_LOG("Missing Built-in DefaultShader.glsl");
		//}
		//else
		//{
		//	m_Material->SetShader(shader);
		//}
		//
		//
		//
		//
		//if (m_Camera != nullptr)
		//{
		//	m_FieldOfView = 60.0f;
		//	m_Camera->SetFieldOfView(m_FieldOfView);
		//	GameObject * gameObject = m_Camera->GetGameObject();
		//	gameObject->SetPosition(Vector3(0.0f, 3.0f, -3.0f));
		//	gameObject->LookAt(Vector3(0.0f,3.5f,0.0f));
		//}
		//
		//if (m_Renderer != nullptr)
		//{
		//	m_Renderer->SetMaterial(m_Material);
		//	m_Renderer->SetMesh(m_Mesh);
		//	GameObject * gameObject = m_Renderer->GetGameObject();
		//}

	}

	GameObject * CreateCube(Vector3 aPosition, Pointer<Material> & aMaterial, float aSize)
	{
		Pointer<Mesh> mesh;
		mesh.MakeUnique(Geometry::CreateCube(
			aSize, aSize, aSize,
			Color::CornflowerBlue(),
			Memory::AllocatorType::Pool));
		GameObject * cube = NEW_POOL(GameObject)("Cube");
		Renderer * renderer = cube->AddComponent<Renderer>();
		renderer->SetMaterial(aMaterial);
		renderer->SetMesh(mesh);
		cube->SetPosition(aPosition);

		return cube;
	}
		
	GameObject * CreatePlane(Vector3 aPosition, Pointer<Material> & aMaterial, float aSize)
	{
		Pointer<Mesh> mesh;
		mesh.MakeUnique(Geometry::CreatePlane(
			aSize, aSize,
			Color::CornflowerBlue(),
			Memory::AllocatorType::Pool));
		GameObject * cube = NEW_POOL(GameObject)("Plane");
		Renderer * renderer = cube->AddComponent<Renderer>();
		renderer->SetMaterial(aMaterial);
		renderer->SetMesh(mesh);
		cube->SetPosition(aPosition);

		return cube;
	}

	void TestComponent::OnLateInitialize()
	{
		//DEBUG_LOG("Late Initialization %s", GetType().GetName().c_str());
		//
		//m_LightTarget = CreateCube(Vector3(3.0f, 1.0f, 0.0f), m_Material, 2.0f);
		//m_Light =  CreateCube(Vector3(-2.0f, 3.0f, 5.0f), m_Material, 1.0f);
		//
		//
        //CreateCube(Vector3(4.0f, 1.0f, 0.0f), m_Material, 3.0f);
	}
	
	void TestComponent::OnDestroy()
	{
		
	}
	void TestComponent::OnLateDestroy()
	{
	
	}
	
	void TestComponent::Update()
	{
		//float speed = 3.0f;
		//float fovSpeed = 5.0f;
		//Input * input = Input::Instance();
		//
		//float v = input->GetAxis("Vertical");
		//float h = input->GetAxis("Horizontal");
		//float lightV = input->GetAxis("Light V");
		//float lightH = input->GetAxis("Light H");
		//float lightY = input->GetAxis("Light Y");
		//float targetV = input->GetAxis("Target V");
		//float targetH = input->GetAxis("Target H");
		//
		//
		//
		//if (m_Renderer != nullptr && m_Camera != nullptr)
		//{
		//	GameObject * gameObject = m_Camera->GetGameObject();
		//	Vector3 position = gameObject->GetPosition();
		//
		//	position.x += h * Time::GetDeltaTime() * speed;
		//	position.z += v * Time::GetDeltaTime() * speed;
		//
		//	gameObject->SetPosition(position);
		//
		//	if (input->GetKey(KeyCode::Alpha0))
		//	{
		//		m_FieldOfView = Math::Clamp(m_FieldOfView + Time::GetDeltaTime() * fovSpeed, 10.0f, 90.0f);
		//	}
		//	else if (input->GetKey(KeyCode::Alpha9))
		//	{
		//		m_FieldOfView = Math::Clamp(m_FieldOfView - Time::GetDeltaTime() * fovSpeed, 10.0f, 90.0f);
		//	}
		//	if (input->GetKeyDown(KeyCode::GraveAccent))
		//	{
		//		DEBUG_LOG("FOV: %f", m_FieldOfView);
		//		DEBUG_LOG("Pos: %f %f %f", Graphics::DEBUG_POSITION.x, Graphics::DEBUG_POSITION.y, Graphics::DEBUG_POSITION.z);
		//		DEBUG_LOG("Dir: %f %f %f", Graphics::DEBUG_DIRECTION.x, Graphics::DEBUG_DIRECTION.y, Graphics::DEBUG_DIRECTION.z);
		//	}
		//
		//	m_Camera->SetFieldOfView(m_FieldOfView);
		//
		//	//Quaternion rotation = renderer->GetRotation();
		//	//rotation *= Quaternion::Euler(Vector3(0.0f, 35.0f * Time::GetDeltaTime(), 0.0f));
		//	//renderer->SetRotation(rotation);
		//}
		//
		//if (m_Light != nullptr && m_LightTarget != nullptr)
		//{
		//	Vector3 position = m_Light->GetPosition();
		//	position.x += lightH * Time::GetDeltaTime() * speed;
		//	position.z += lightV * Time::GetDeltaTime() * speed;
		//	position.y += lightY * Time::GetDeltaTime() * speed;
		//	m_Light->SetPosition(position);
		//
		//	position = m_LightTarget->GetPosition();
		//	position.x += targetH * Time::GetDeltaTime() * speed;
		//	position.z += targetV * Time::GetDeltaTime() * speed;
		//	m_LightTarget->SetPosition(position);
		//
		//	
		//
		//	if (input->GetKeyDown(KeyCode::B))
		//	{
		//		if (m_Light->IsActive())
		//		{
		//			m_Light->SetActive(false);
		//		}
		//		else
		//		{
		//			m_Light->SetActive(true);
		//		}
		//	}
		//
		//	if (input->GetKeyDown(KeyCode::Alpha1))
		//	{
		//		m_Light->SetPosition(Vector3(20.0f, 5.0f, 20.0f));
		//		m_LightTarget->SetPosition(Vector3(-5.0f, 0.0f, 0.0f));
		//	}
		//
		//	Vector3 direction = m_LightTarget->GetPosition() - m_Light->GetPosition();
		//	Graphics::DEBUG_POSITION = m_Light->GetPosition();
		//	Graphics::DEBUG_DIRECTION = (direction.Normalized());
		//}


	}
	void TestComponent::LateUpdate()
	{
	
	}
	void TestComponent::FixedUpdate()
	{
	
	}
	
	void TestComponent::OnPreRender()
	{
		
	}
	void TestComponent::OnRender()
	{
	
	}
	void TestComponent::OnPostRender()
	{
	
	}
}