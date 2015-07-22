#include "ResourceDatabase.h"
#include "../Core/GemAPI.h"

using namespace Gem::Debugging;

namespace Gem
{
	RDEFINE_CLASS(ResourceDatabase, object)

	const std::string ResourceDatabase::META_ID_TOKEN = "Meta-ID";

	ResourceDatabase * ResourceDatabase::s_Instance = nullptr;
	ResourceDatabase::ResourceDatabase()
	{
		m_ImageTextureType = Reflection::Runtime::TypeOf<ImageTexture>();
		m_MeshType = Reflection::Runtime::TypeOf<Mesh>();
		m_ShaderType = Reflection::Runtime::TypeOf<Shader>();
		m_MaterialType = Reflection::Runtime::TypeOf<Material>();

		m_WorkingDirectory = Directory::GetCurrent();
		m_WorkingDirectory.Back(2);
		m_WorkingDirectory.Change("Resources\\");
	}
	ResourceDatabase::~ResourceDatabase()
	{
		//TODO(Nathan): Release resources.

		//Terminate resources in reverse order
		for (ResourceCache::reverse_iterator it = m_ResourceCache.rbegin(); it != m_ResourceCache.rend(); it++)
		{
			(*it).second.Terminate();
		}
		m_ResourceCache.clear();
	}

	/**
	* Initializes the database.
	*/
	void ResourceDatabase::Initialize()
	{
		if (s_Instance == nullptr)
		{
			s_Instance = new ResourceDatabase();
		}
	}

	/**
	* Unloads all loaded resources.
	*/
	void ResourceDatabase::Terminate()
	{
		if (s_Instance != nullptr)
		{
			delete s_Instance;
			s_Instance = nullptr;
		}
	}

	/**
	* Loads a texture. This function will call GetTexture to see if its already loaded.
	* @param aName The name of the texture resource to load
	* @return Returns a managed pointer to the image texture resource.
	*/
	Pointer<ImageTexture> ResourceDatabase::LoadTexture(const std::string & aName)
	{
		if (s_Instance == nullptr)
		{
			return Pointer<ImageTexture>::Null();
		}
		//Isolate filename.
		std::string resourceName = FilenameToResourceName(aName);
		Pointer<ImageTexture> imageTexture = GetTexture(resourceName);
		if (imageTexture.IsAlive())
		{
			return imageTexture;
		}
		
		//Create the filepath and verify that it exists.
		std::string filepath = s_Instance->m_WorkingDirectory.GetPath();
		filepath.append(aName);
		if (!Directory::FileExists(filepath))
		{
			return Pointer<ImageTexture>::Null();
		}
		
		//Create the image texture pointer & the resource casted pointer.
		imageTexture = Pointer<ImageTexture>();
		Pointer<Resource> resource = imageTexture.Cast<Resource>();
		if (imageTexture.IsAlive() && resource.IsAlive())
		{
			//Read in meta data.
			std::string metaFilename = filepath;
			metaFilename.append(".meta");
			IniFileStream filestream;
			filestream.SetPath(metaFilename);
			filestream.Read();
			//Set the resource name
			imageTexture->SetName(resourceName);
			//Load Meta Data. (If there is any)
			resource->LoadMeta(filestream);
			//Load Texture & Upload to GPU and free CPU resources.
			imageTexture->Load(filepath);
			imageTexture->Upload();
			if (!imageTexture->IsUploaded())
			{
				Debug::ErrorFormat("Resource", nullptr, "Failed to load resource (%s)\nFilepath: %s", resourceName.c_str(), filepath.c_str());
			}
			else
			{
				resource->SaveMeta(filestream);
				filestream.Save();
				s_Instance->m_ResourceCache.insert(std::pair<std::string, Pointer<Resource>>(resource->GetName(),resource));
				return imageTexture;
			}
		}
		else
		{
			Debug::Error("Resource", "Error creating resource ImageTexture");

		}
		
		return Pointer<ImageTexture>::Null();
	}
	/**
	* Searches for a loaded texture resource by name.
	* @param aName The name of the texture resource to find.
	* @return Returns a managed pointer to the image texture resource. A nullptr is returned if there is no texture.
	*/
	Pointer<ImageTexture> ResourceDatabase::GetTexture(const std::string & aName)
	{
		if (s_Instance == nullptr)
		{
			//Return a nullptr because the database is not loaded.
			return Pointer<ImageTexture>::Null();
		}
		//Get access to the cache
		ResourceCache & cache = s_Instance->m_ResourceCache;
		//Find elements with the key equal to aName.
		ResourcePair iterator = cache.equal_range(aName);
		//If it exists
		if (iterator.first != iterator.second)
		{
			//Check its type then return the casted pointer.
			Pointer<Resource> resource = iterator.first->second;
			Type type = resource->GetType();
			if (type == Reflection::Runtime::TypeOf<ImageTexture>())
			{
				return resource.Cast<ImageTexture>();
			}
			//TODO(Nathan): Debug Log out what type of resource this is an report the type mismatch. Make this a config option.
		}
		//Return a nullptr because the resource didn't exist or if it did it was the incorrect type.
		return Pointer<ImageTexture>::Null();
	}

	/**
	* Loads a mesh resource. This function will call GetMesh first to see if its already loaded.
	* @param aName The name of the mesh resource to load.
	* @return Returns a managed pointer to the mesh resource.
	*/
	Pointer<Mesh> ResourceDatabase::LoadMesh(const std::string & aName)
	{
		if (s_Instance == nullptr)
		{
			return Pointer<Mesh>::Null();
		}
		//Isolate filename.
		std::string resourceName = FilenameToResourceName(aName);
		Pointer<Mesh> mesh = GetMesh(resourceName);
		if (mesh.IsAlive())
		{
			return mesh;
		}

		//Create the filepath and verify that it exists.
		std::string filepath = s_Instance->m_WorkingDirectory.GetPath();
		filepath.append(aName);
		if (!Directory::FileExists(filepath))
		{
			return Pointer<Mesh>::Null();
		}

		//Create the mesh pointer & the resource casted pointer.
		mesh = Pointer<Mesh>();
		Pointer<Resource> resource = mesh.Cast<Resource>();
		if (mesh.IsAlive() && resource.IsAlive())
		{
			//Read in meta data.
			std::string metaFilename = filepath;
			metaFilename.append(".meta");
			IniFileStream filestream;
			filestream.SetPath(metaFilename);
			filestream.Read();
			//Set the resource name
			mesh->SetName(resourceName);
			//Load Meta Data. (If there is any)
			resource->LoadMeta(filestream);
			//Load Mesh & Upload to GPU and free CPU resources.
			mesh->LoadFile(filepath);
			mesh->Upload();
			if (!mesh->IsUploaded())
			{
				Debug::ErrorFormat("Resource", nullptr, "Failed to load resource (%s)\nFilepath: %s", resourceName.c_str(), filepath.c_str());
			}
			else
			{
				resource->SaveMeta(filestream);
				filestream.Save();
				s_Instance->m_ResourceCache.insert(std::pair<std::string, Pointer<Resource>>(resource->GetName(), resource));
				return mesh;
			}
		}
		else
		{
			Debug::Error("Resource", "Error creating resource Mesh");
		}

		return Pointer<Mesh>::Null();
	}

	/**
	* Searches for a loaded mesh resource by name.
	* @param aName The name of the mesh resource to find.
	* @return Returns a managed pointer to the mesh resource. A nullptr is returned if the mesh is not loaded.
	*/
	Pointer<Mesh> ResourceDatabase::GetMesh(const std::string & aName)
	{
		if (s_Instance == nullptr)
		{
			//Return a nullptr because the database is not loaded.
			return Pointer<Mesh>::Null();
		}
		//Get access to the cache
		ResourceCache & cache = s_Instance->m_ResourceCache;
		//Find elements with the key equal to aName.
		ResourcePair iterator = cache.equal_range(aName);
		//If it exists
		if (iterator.first != iterator.second)
		{
			//Check its type then return the casted pointer.
			Pointer<Resource> resource = iterator.first->second;
			Type type = resource->GetType();
			if (type == Reflection::Runtime::TypeOf<Mesh>())
			{
				return resource.Cast<Mesh>();
			}
			//TODO(Nathan): Debug Log out what type of resource this is an report the type mismatch. Make this a config option.
		}
		//Return a nullptr because the resource didn't exist or if it did it was the incorrect type.
		return Pointer<Mesh>::Null();
	}

	/**
	* Loads a shader. This function will call GetShader to see if its already loaded.
	* @param aName The name of the shader resource to load.
	* @return Returns a managed pointer to the shader resource.
	*/
	Pointer<Shader> ResourceDatabase::LoadShader(const std::string & aName)
	{
		if (s_Instance == nullptr)
		{
			return Pointer<Shader>::Null();
		}
		//Isolate filename.
		std::string resourceName = FilenameToResourceName(aName);
		Pointer<Shader> shader = GetShader(resourceName);
		if (shader.IsAlive())
		{
			return shader;
		}

		//Create the filepath and verify that it exists.
		std::string filepath = s_Instance->m_WorkingDirectory.GetPath();
		filepath.append(aName);
		if (!Directory::FileExists(filepath))
		{
			return Pointer<Shader>::Null();
		}

		//Create the shader pointer & the resource casted pointer.
		shader = Pointer<Shader>();
		Pointer<Resource> resource = shader.Cast<Resource>();
		if (shader.IsAlive() && resource.IsAlive())
		{
			//Read in meta data.
			std::string metaFilename = filepath;
			metaFilename.append(".meta");
			IniFileStream filestream;
			filestream.SetPath(metaFilename);
			filestream.Read();
			//Set the resource name
			shader->SetName(resourceName);
			//Load Meta Data. (If there is any)
			resource->LoadMeta(filestream);
			//Load Shader & Upload to GPU and free CPU resources.
			shader->Load(filepath);
			shader->ReleaseFile();
			if (!shader->IsUploaded())
			{
				Debug::ErrorFormat("Resource", nullptr, "Failed to load resource (%s)\nFilepath: %s", resourceName.c_str(), filepath.c_str());
			}
			else
			{
				resource->SaveMeta(filestream);
				filestream.Save();
				s_Instance->m_ResourceCache.insert(std::pair<std::string, Pointer<Resource>>(resource->GetName(), resource));
				return shader;
			}
		}
		else
		{
			Debug::Error("Resource","Error creating resource Shader");
		}

		return Pointer<Shader>::Null();
	}

	/**
	* Searches for a loaded shader resource by name.
	* @param aName The name of the shader resource to find.
	* @return Returns a managed pointer to the shader resource. A nullptr is returned if the mesh is not loaded.
	*/
	Pointer<Shader> ResourceDatabase::GetShader(const std::string & aName)
	{
		if (s_Instance == nullptr)
		{
			//Return a nullptr because the database is not loaded.
			return Pointer<Shader>::Null();
		}
		//Get access to the cache
		ResourceCache & cache = s_Instance->m_ResourceCache;
		//Find elements with the key equal to aName.
		ResourcePair iterator = cache.equal_range(aName);
		//If it exists
		if (iterator.first != iterator.second)
		{
			//Check its type then return the casted pointer.
			Pointer<Resource> resource = iterator.first->second;
			if (resource.IsAlive())
			{
				Type type = resource->GetType();
				if (type == Reflection::Runtime::TypeOf<Shader>())
				{
					return resource.Cast<Shader>();
				}
			}
			//TODO(Nathan): Debug Log out what type of resource this is an report the type mismatch. Make this a config option.
		}
		//Return a nullptr because the resource didn't exist or if it did it was the incorrect type.
		return Pointer<Shader>::Null();
	}

	/**
	* Loads a material. This function will call GetMaterial to see if its already loaded.
	* @param aName The name of the material resource to load.
	* @return Returns a managed pointer to the material resource.
	*/
	Pointer<Material> ResourceDatabase::LoadMaterial(const std::string & aName)
	{
		Pointer<Material> material = GetMaterial(aName);
		if (material.IsAlive())
		{
			return material;
		}
		//TODO(Nathan): Use file io to parse the working directory and load in the material.

		return Pointer<Material>::Null();
	}

	/**
	* Searches for a loaded material resource by name.
	* @param aName The name of the material to find.
	* @return Returns a managed pointer to the material resource. A nullptr is returned if the mesh is not loaded.
	*/
	Pointer<Material> ResourceDatabase::GetMaterial(const std::string & aName)
	{
		if (s_Instance == nullptr)
		{
			return Pointer<Material>::Null();
		}

		//Get access to the cache
		ResourceCache & cache = s_Instance->m_ResourceCache;
		//Find elements with the key equal to aName.
		ResourcePair iterator = cache.equal_range(aName);
		//If it exists
		if (iterator.first != iterator.second)
		{
			//Check its type then return the casted pointer.
			Pointer<Resource> resource = iterator.first->second;
			if (resource.IsAlive())
			{
				Type type = resource->GetType();
				if (type == Reflection::Runtime::TypeOf<Material>())
				{
					return resource.Cast<Material>();
				}
			}
			//TODO(Nathan): Debug Log out what type of resource this is an report the type mismatch. Make this a config option.
		}
		//Return a nullptr because the resource didn't exist or if it did it was the incorrect type.
		return Pointer<Material>::Null();
	}

	/**
	* Loads a resource by typename. If the typename is not a valid resource typename this will return a nullptr.
	* @param aName The name of the resource to load.
	* @param aTypename The name of the type to load.
	* @return A managed pointer to the resource.
	*/
	Pointer<Resource> ResourceDatabase::LoadResource(const std::string & aName, const std::string & aTypename)
	{
		Type type = Reflection::Runtime::TypeOf(aTypename);
		return LoadResource(aName, type);
	}

	/**
	* Loads a resource by type. If the type is not a valid resourec type this will return a nullptr.
	* @param aName The name of the resource to load.
	* @param aType The type of resource to load.
	* @return A managed pointer to the resource.
	*/
	Pointer<Resource> ResourceDatabase::LoadResource(const std::string & aName, const Type & aType)
	{
		if (s_Instance == nullptr)
		{
			return Pointer<Resource>::Null();
		}

		Pointer<Resource> resource = GetResource(aName);
		if (resource.IsAlive())
		{
			return resource;
		}

		if (aType == s_Instance->m_ImageTextureType)
		{
			Pointer<ImageTexture> imageTexture = LoadTexture(aName);
			if (imageTexture.IsAlive())
			{
				return imageTexture.Cast<Resource>();
			}
		}
		else if (aType == s_Instance->m_MeshType)
		{
			Pointer<Mesh> mesh = LoadMesh(aName);
			if (mesh.IsAlive())
			{
				return mesh.Cast<Resource>();
			}
		}
		else if (aType == s_Instance->m_ShaderType)
		{
			Pointer<Shader> shader = LoadShader(aName);
			if (shader.IsAlive())
			{
				return shader.Cast<Resource>();
			}
		}
		else if (aType == s_Instance->m_MaterialType)
		{
			Pointer<Material> material = LoadMaterial(aName);
			if (material.IsAlive())
			{
				return material.Cast<Resource>();
			}
		}
		return Pointer<Resource>::Null();
	}

	/**
	* Searches for a loaded resource by name.
	* @param aName The name of the shader resource to find.
	*@return Returns a managed pointer to the resource. A nullptr is returned if the resource is not loaded.
	*/
	Pointer<Resource> ResourceDatabase::GetResource(const std::string & aName)
	{
		if (s_Instance == nullptr)
		{
			//Return a nullptr because the database is not loaded.
			return Pointer<Resource>::Null();
		}
		//Get the cache from the singleton and search for the pair. 
		ResourceCache & cache = s_Instance->m_ResourceCache;
		ResourcePair iterator = cache.equal_range(aName);
		//Do a comparison to see if it exists.
		if (iterator.first != iterator.second)
		{
			//Return the resource pointer.
			return iterator.first->second;
		}
		return Pointer<Resource>::Null();
	}

	/**
	* Unloads a resource by name. Any existing managed pointers will have their count set to 0 indicating the memory is no longer valid.
	*/
	void ResourceDatabase::UnloadResource(const std::string & aName)
	{
		if (s_Instance == nullptr)
		{
			//Cannot unload resource. Resource database has not been loaded yet.
			return;
		}
		//Get the resource cache from the singleton instance.
		ResourceCache & cache = s_Instance->m_ResourceCache;
		//Find the resource within the map.
		ResourcePair iterator = cache.equal_range(aName);

		//Make sure the iterator exists.
		if (iterator.first != iterator.second)
		{
			//Get the managed pointer and make sure its alive (not null)
			Pointer<Resource> resource = iterator.first->second;
			if (resource.IsAlive())
			{
				//Remove from cache and terminate.
				cache.erase(iterator.first);
				resource.Terminate();
			}
		}
	}


	std::string ResourceDatabase::FilenameToResourceName(const std::string & aFilename)
	{
		//Isolate the resource name.
		bool hasExtension = false;
		std::string resourceName = aFilename;
		int extensionIndex = 0;

		for (int i = resourceName.length(); i >= 0; i--)
		{
			//Find extension begin index.
			if (resourceName[i] == '.')
			{
				extensionIndex = i;
				hasExtension = true;
			}
			//Find directory begin index.
			else if (resourceName[i] == '\\' || resourceName[i] == '/')
			{
				if (hasExtension)
				{
					return resourceName.substr(i + 1, extensionIndex - i - 1);
				}
				else
				{
					return resourceName.substr(i + 1, resourceName.length() - i);
				}
			}
		}
		//No directory but extension return a substring
		if (hasExtension)
		{
			return resourceName.substr(0, extensionIndex);
		}
		//Return the original.
		return resourceName;
	}
}