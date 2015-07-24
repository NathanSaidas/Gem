#ifndef GAME_ENGINE_RESOURCE_DATABASE_H
#define GAME_ENGINE_RESOURCE_DATABASE_H

//============================================================
// Date:			May,	13,		2015
// Description:		Defines the class Shader used in Graphics.
//============================================================

#pragma  region CHANGE LOG
// -- Nathan Hanlan - Implemented methods for resource database. Load methods still require file io and actual loading implementation.
// -- Nathan Hanlan - Implemented Unload resource method and LoadTexture method.
// -- Nathan Hanlan - Implemented LoadMesh method.
// -- Nathan Hanlan - Implemented LoadShader method.
// -- Nathan Hanlan	- Refactored this class to fit in the Gem project from SchoolGameEngine
// -- Nathan Hanlan - Constructor now uses initialization list.
#pragma endregion

#pragma region CHANGE LOG

#pragma endregion

#include <map>
//Using for basic types. object
#include "../Core/GemAPI.h"
#include "../Core/BasicTypes.h"
#include "../Graphics/ImageTexture.h"
#include "../Graphics/Mesh.h"
#include "../Graphics/Shader.h"
#include "../Graphics/Material.h"
#include "../Memory/Memory.h"
#include "../Utilities/Utilities.h"
#include "Resource.h"


namespace Gem
{

	//class ResourceDatabase;
	//template class GEM_API Reflection::MetaObject<ResourceDatabase>;
    //
	typedef std::map<std::string,Pointer<Resource>> ResourceCache;
	typedef std::pair<ResourceCache::iterator, ResourceCache::iterator> ResourcePair;
	//
    //
	//template class GEM_API std::map<std::string, Pointer<Resource>>;

	/*
	* The resource database will manage all resources.
	* 
	*/
	class ResourceDatabase : public object
	{
		RDECLARE_CLASS(ResourceDatabase)
	public:
		static const std::string META_ID_TOKEN;

		/**
		* Initializes the database.
		*/
		static void Initialize();
		/**
		* Unloads all loaded resources.
		*/
		static void Terminate();
		/**
		* Loads a texture. This function will call GetTexture to see if its already loaded.
		* @param aName The name of the texture resource to load
		* @return Returns a managed pointer to the image texture resource.
		*/
		static Pointer<ImageTexture> LoadTexture(const std::string & aName);
		/**
		* Searches for a loaded texture resource by name.
		* @param aName The name of the texture resource to find.
		* @return Returns a managed pointer to the image texture resource. A nullptr is returned if there is no texture.
		*/
		static Pointer<ImageTexture> GetTexture(const std::string & aName);
		/**
		* Loads a mesh resource. This function will call GetMesh first to see if its already loaded.
		* @param aName The name of the mesh resource to load.
		* @return Returns a managed pointer to the mesh resource.
		*/
		static Pointer<Mesh> LoadMesh(const std::string & aName);
		/**
		* Searches for a loaded mesh resource by name.
		* @param aName The name of the mesh resource to find.
		* @return Returns a managed pointer to the mesh resource. A nullptr is returned if the mesh is not loaded.
		*/
		static Pointer<Mesh> GetMesh(const std::string & aName);
		/**
		* Loads a shader. This function will call GetShader to see if its already loaded.
		* @param aName The name of the shader resource to load. 
		* @return Returns a managed pointer to the shader resource.
		*/
		static Pointer<Shader> LoadShader(const std::string & aName);
		/**
		* Searches for a loaded shader resource by name.
		* @param aName The name of the shader resource to find.
		* @return Returns a managed pointer to the shader resource. A nullptr is returned if the mesh is not loaded.
		*/
		static Pointer<Shader> GetShader(const std::string & aName);
		/**
		* Loads a material. This function will call GetMaterial to see if its already loaded.
		* @param aName The name of the material resource to load.
		* @return Returns a managed pointer to the material resource. 
		*/
		static Pointer<Material> LoadMaterial(const std::string & aName);
		/**
		* Searches for a loaded material resource by name.
		* @param aName The name of the material to find.
		* @return Returns a managed pointer to the material resource. A nullptr is returned if the mesh is not loaded.
		*/
		static Pointer<Material> GetMaterial(const std::string & aName);

		/**
		* Loads a resource by typename. If the typename is not a valid resource typename this will return a nullptr.
		* @param aName The name of the resource to load.
		* @param aTypename The name of the type to load.
		* @return A managed pointer to the resource. 
		*/
		static Pointer<Resource> LoadResource(const std::string & aName, const std::string & aTypename);
		/**
		* Loads a resource by type. If the type is not a valid resourec type this will return a nullptr.
		* @param aName The name of the resource to load.
		* @param aType The type of resource to load.
		* @return A managed pointer to the resource.
		*/
		static Pointer<Resource> LoadResource(const std::string & aName, const Type & aType);

		/**
		* Searches for a loaded resource by name.
		* @param aName The name of the shader resource to find.
		 *@return Returns a managed pointer to the resource. A nullptr is returned if the resource is not loaded.
		*/
		static Pointer<Resource> GetResource(const std::string & aName);
		/**
		* Unloads a resource by name. If the resource exists Terminate is called on the managed pointer.
		* @param aName The name of the resource to unload.
		*/
		static void UnloadResource(const std::string & aName);

	private:
		/**
		* Static pointer to the singleton instance of the ResourceDatabase.
		*/
		static ResourceDatabase * s_Instance;
		ResourceDatabase();
		~ResourceDatabase();
		friend class Reflection::MetaObject<ResourceDatabase>;

		ResourceCache m_ResourceCache;
		Type m_ImageTextureType;
		Type m_MeshType;
		Type m_ShaderType;
		Type m_MaterialType;

		Directory m_WorkingDirectory;

		static std::string FilenameToResourceName(const std::string & aFilename);
		
	};

	TYPE_DEFINE(ResourceDatabase)
}


#endif