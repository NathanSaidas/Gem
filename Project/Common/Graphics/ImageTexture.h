#ifndef GEM_IMAGE_TEXTURE_H
#define GEM_IMAGE_TEXTURE_H

//============================================================
// Date:			May,	13,		2015
// Description:		Defines the class ImageTexture used drawing images with Graphics.
//============================================================

#pragma  region CHANGE LOG
// -- Nathan Hanlan	- Refactored this class to fit in the Gem project from SchoolGameEngine
// -- Nathan Hanlan - Constructor now uses initialization list.
#pragma endregion

#include "../Core/GemAPI.h"
#include "Texture.h"
#include "Color.h"

namespace Gem
{
	
    //FORCE_EXPORT_META(ImageTexture);

	//template class GEM_API Array<UInt8>;

	class ImageTexture : public Texture
	{
		RDECLARE_CLASS(ImageTexture)
	public:
		/**
		* Default Constructor of ImageTexture. Sets default values
		*/
		ImageTexture();
		/**
		Destructor of ImageTexture. Releases used resources.
		*/
		~ImageTexture();
		
		/**
		* Gets the image format being used.
		* @return Returns the image format being used.
		*/
		ImageFormat GetImageFormat();
		/**
		* Sets the image format. This has no effect on uploaded textures.
		* @param aFormat The format for the image to use.
		*/
		void SetImageFormat(ImageFormat aFormat);
		
		/**
		* Loads an array of bytes as a texture. Call upload after to use this texture.
		* @param aData The array of bytes to read from
		*/
		void Load(const Array<UInt8> & aData);
		/**
		* Loads a filename as a texture onto the CPU. Call upload after to use this texture.
		* @param aFilename The filename to load. (This is the full path)
		*/
		void Load(const std::string & aFilename);
		/**
		* Uploads the data to the GPU. 
		* @param aReleaseCPU This is true by default but its an option whether or not to upload it right away.
		*/
		void Upload(bool aReleaseCPU);
		void Upload();

		/**
		* Sets the pixels of the texture. Call upload to apply the changes to the texture
		* @param aPixels The new pixels of the texture. 
		*/
		void SetPixels(const Array<Color> & aPixels);
		/**
		* Gets the pixels of the texture
		* @return Returns the pixels of the texture when uploaded.
		*/
		Array<Color> GetPixels();

		/**
		* Releases resources allocated on the CPU
		*/
		virtual void ReleaseCPU();
		/**
		* Releases resources allocated on the GPU
		*/
		virtual void ReleaseGPU();

	protected:

		void SaveMeta(IniFileStream & aFilename);
		void LoadMeta(IniFileStream & aFilename);

	private:
		/**
		* The image format of the texture.
		*/
		ImageFormat m_ImageFormat;
		/**
		* An array of pixels to upload the texture with.
		*/
		Array<UInt8> m_Pixels;
	};

	TYPE_DEFINE(ImageTexture)
}

#endif // GEM_IMAGE_TEXTURE_H