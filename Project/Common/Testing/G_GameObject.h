#ifndef G_GAME_OBJECT_H
#define G_GAME_OBJECT_H

#include "../G_Object.h"

namespace Gem
{
	class GameObject : public Object
	{
		G_CLASS_DEF(GameObject)
	public:
		GameObject();
		~GameObject();
		virtual void Update();
	};

}

#endif