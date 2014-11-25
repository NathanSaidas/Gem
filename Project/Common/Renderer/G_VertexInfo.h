#ifndef G_VERTEX_INFO_H
#define G_VERTEX_INFO_H

#include "../Primitives/G_PrimitiveObjects.h"
#include "../Math/G_Math.h"
#include "G_Color.h"

namespace Gem
{
	struct VertexInfo //: public Object
	{
	public:
		VertexInfo();
		VertexInfo(ClassUtils::Vector3 aPosition, Color aColor, ClassUtils::Vector2 aTexCoord);//, ClassUtils::Vector3 & aNormal);
		~VertexInfo();
		ClassUtils::Vector3 position;
		Color color;
		ClassUtils::Vector2 texCoord;
		ClassUtils::Vector3 normal;
	};

	struct DualVertexInfo : VertexInfo
	{
		DualVertexInfo();
		DualVertexInfo(ClassUtils::Vector3 aPosition, ClassUtils::Vector3 aPositionB, Color aColor, ClassUtils::Vector2 aTexCoord);
		~DualVertexInfo();

		ClassUtils::Vector3 positionB;
	};
}
//GEM_CLASS(VertexInfo,Object)

#endif