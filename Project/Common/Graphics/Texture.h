#ifndef GEM_TEXTURE_H
#define GEM_TEXTURE_H

//============================================================
// Date:			May,	13,		2015
// Description:		Defines the class Texture used base class for all Texture types.
//============================================================

#pragma  region CHANGE LOG
// -- Nathan Hanlan	- Refactored this class to fit in the Gem project from SchoolGameEngine
// -- Nathan Hanlan - Constructor now uses initialization list.
#pragma endregion

#include "../Core/GemAPI.h"
#include "../Core/BasicTypes.h"
#include "../Resource/Resource.h"
#include "GraphicEnums.h"

namespace Gem
{
	/*
	* Texture
	*	- ImageTexture (Load Data From File)
	*	- RenderTexture (Handles to data on GPU)
	*
	* Shared Parameters
	*	- Filter Mode
	*	- Clamp Mode
	*	- Size Width/Height
	*	- Handle
	*
	*
	* Image Texture Parameters
	* - Image Format


	*/

    //FORCE_EXPORT_META(Texture);

	class Texture : public Resource
	{
		RDECLARE_CLASS(Texture)
	public:
		/**
		* Default constructor sets default values of the object
		*/
		Texture();
		/**
		* Releases used memory of the resource.
		*/
		virtual ~Texture();

		/**
		* Gets the filter mode last set.
		* @return The filter mode being used.
		*/
		FilterMode GetFilterMode();
		/**
		* Sets the filter mode. Note that this has no effect if the texture is uploaded
		* @param aMode The filter mode to use.
		*/
		void SetFilterMode(FilterMode aMode);

		/** 
		* Gets the wrap mode last set.
		* @return Returns the wrap mode being used.
		*/
		WrapMode GetWrapMode();
		/**
		* Sets the wrap mode. Note that this has no effect if the texture is uploaded.
		* @param aMode The wrap mode to use.
		*/
		void SetWrapMode(WrapMode aMode);

		/**
		* Gets the width of the texture
		* @return Returns the height of the texture.
		*/
		UInt32 GetWidth();
		/**
		* Gets the height of the texture
		* @return Returns the height of the texture.
		*/
		UInt32 GetHeight();
		/**
		* Gets a raw handle to the texture from OpenGL
		* Use this wil caution. Do not deallocate the handle as its handled internally.
		* @return Returns a raw handle to the texture from OpenGL
		*/
		unsigned int GetHandle();
		/**
		* Releases resources allocated on both the CPU and the GPU
		*/
		void Release();
		/**
		* Releases resources allocated on the CPU
		*/
		virtual void ReleaseCPU();
		/**
		* Releases resources allocated on the GPU
		*/
		virtual void ReleaseGPU();
		/**
		* Determines if the texture is uploaded or not and can be used with rendering.
		* @return Returns true if the texture is uploaded to the GPU. Returns false otherwise.
		*/
		bool IsUploaded();

	protected:
		/**
		* Sets whether or not the texture is uploaded.
		* @param aValue Whether or not the texture is uploaded or not.
		*/
		void SetIsUploaded(bool aValue);
		/**
		* Internal set size. Sets the size of the texture.
		* @param aWidth The width of the texture
		* @param aHeight The height of the texture.
		*/
		void SetSize(UInt32 aWidth, UInt32 aHeight);

		/**
		* Generates a texture handle.
		*/
		void GenerateTextureHandle();
		/**
		* Deletes a texture handle;
		*/
		void DeleteTextureHandle();

	private:
		/**
		* The width of the texture in pixels.
		*/
		UInt32 m_Width;
		/**
		* The height of the texture in pixels
		*/
		UInt32 m_Height;
		/**
		* The filter mode of the texture
		*/
		FilterMode m_FilterMode;
		/**
		* The wrap mode of the texture
		*/
		WrapMode m_WrapMode;
		/**
		* A raw handle to the texture. Use care when using this outside of the graphics systems.
		*/
		unsigned int m_TextureHandle;
		/**
		* Whether or not the texture is uploaded to the GPU
		*/
		bool m_IsUploaded;
	};

	TYPE_DEFINE(Texture);
}

#endif