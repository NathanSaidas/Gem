#ifndef G_GAME_OBJECT_H
#define G_GAME_OBJECT_H

#include "../Primitives/G_PrimitiveObjects.h"

namespace Gem
{
	class GameObject : public Object
	{
	public:
		GameObject();
		~GameObject();
		virtual void Update();
	};

}

GEM_CLASS(GameObject,Object)

#endif