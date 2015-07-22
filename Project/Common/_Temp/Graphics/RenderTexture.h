#ifndef GEM_RENDER_TEXTURE_H
#define GEM_RENDER_TEXTURE_H

//============================================================
// Date:			May,	13,		2015
// Description:		Defines the class RenderTexture used rendering to.
//============================================================

#pragma  region CHANGE LOG
// -- Nathan Hanlan	- Refactored this class to fit in the Gem project from SchoolGameEngine
// -- Nathan Hanlan - Constructor now uses initialization list.
// -- Nathan Hanlan - Changed unsigned int types to UInt32 types.
#pragma endregion

#include "../Core/GemAPI.h"
#include "../Core/BasicTypes.h"
#include "Texture.h"

namespace Gem
{
	class RenderTexture;
	template class GEM_API Reflection::MetaObject <RenderTexture>;

	class GEM_API RenderTexture : public Texture
	{
		RDECLARE_CLASS(RenderTexture)
	public:
		RenderTexture();
		~RenderTexture();
		

		UInt32 GetFBOHandle();
		UInt32 GetDepthHandle();

		void Create(UInt32 aWidth, UInt32 aHeight);
		void ReleaseGPU();

	private:

		UInt32 m_FBOHandle;
		UInt32 m_DepthTextureHandle;
	};

	TYPE_DEFINE(RenderTexture)
}

#endif