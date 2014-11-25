#ifndef G_FRAME_BUFFER_H
#define G_FRAME_BUFFER_H

#include "../Primitives/G_PrimitiveObjects.h"

namespace Gem
{
	class FrameBuffer : public Object
	{
	public:
		unsigned int m_FBOHandle;

		unsigned int m_TextureHandle;
		unsigned int m_DepthBufferHandle;

		int m_Width;
		int m_Height;
		FrameBuffer();
		~FrameBuffer();

		void Setup(int width, int height);
	};
}
GEM_CLASS(FrameBuffer,Object)

#endif