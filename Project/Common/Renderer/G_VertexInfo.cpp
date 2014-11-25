#include "G_VertexInfo.h"

namespace Gem
{
	VertexInfo::VertexInfo()
	{
		position.Set(0.0f, 0.0f, 0.0f);
		texCoord.Set(0.0f, 0.0f);
	}
	VertexInfo::VertexInfo(ClassUtils::Vector3 aPosition, Color aColor, ClassUtils::Vector2 aTexCoord)// , ClassUtils::Vector3 & aNormal)
	{
		position = aPosition;
		color = aColor;
		texCoord = aTexCoord;
		normal.Set(0.0f, 1.0f, 0.0f);
	}
	VertexInfo::~VertexInfo()
	{

	}

	DualVertexInfo::DualVertexInfo()
	{
		position.Set(0.0f, 0.0f, 0.0f);
	}
	DualVertexInfo::DualVertexInfo(ClassUtils::Vector3 aPosition, ClassUtils::Vector3 aPositionB, Color aColor, ClassUtils::Vector2 aTexCoord)
		: VertexInfo(aPosition, aColor, aTexCoord)
	{
		positionB = aPositionB;
	}
	DualVertexInfo::~DualVertexInfo()
	{

	}
}