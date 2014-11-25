#ifndef G_TEXTURE_H
#define G_TEXTURE_H

#include "../Primitives/G_PrimitiveObjects.h"

namespace Gem
{
	class Texture : public Object
	{
	public:
		Texture();
		~Texture();

		///An OpenGL handle to the texture in OpenGL memory
		unsigned int Handle();
		///Returns true if the texture was uploaded to GPU memory
		bool IsUploaded();
		///Getter for the wrap mode of the texture
		unsigned int WrapMode();
		///Setter for the wrap mode of the texture
		unsigned int WrapMode(unsigned int & aWrapMode);
		///Getter for the filter mode of the texture
		unsigned int FilterMode();
		///Setter for the filter mode of the texture
		unsigned int FilterMode(unsigned int & aFilterMode);
		///The current format of the texture
		int ImageFormat();
		///The width of the texture
		unsigned int Width();
		///The height of the texture
		unsigned int Height();
		///The colors of the texture, this reflects the image format.
		unsigned char * Colors();

		///Uploads the mesh to the GPU
		///@aFree - If this is true the data currently loaded on the CPU will be freed.
		void Upload(bool aFree = true);
		///Frees the texture data from CPU memory
		void FreeCPU();
		///Frees the texture data from the GPU memory
		void FreeGPU();
		///Calls FreeCPU and FreeGPU
		void Free();
		///Loads an image with the given filename
		void Load(string & aFilename, int aImageFormat = 4);

	private:
		///Determines whether or not the mesh was uploaded to the GPU
		bool m_IsUploaded;
		///Data set by the Upload method
		unsigned int m_Handle;
		unsigned int m_WrapMode;
		unsigned int m_FilterMode;
		///Image format is requested through the load method. It's not a guarantee it will be what was requested.
		int m_ImageFormat;
		///Data set by the Load Method
		unsigned int m_Width;
		unsigned int m_Height;
		unsigned char * m_Data;
	};
}

GEM_CLASS(Texture,Object)

#endif