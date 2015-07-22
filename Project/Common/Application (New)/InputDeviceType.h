#ifndef GEM_INPUT_DEVICE_TYPE_H
#define GEM_INPUT_DEVICE_TYPE_H

#include "../Core/GemAPI.h"
#include "../Core/BasicTypes.h"
#include "../Core/Enum.h"

namespace Gem
{
	//FORCE_EXPORT_META(InputDeviceType);

	class InputDeviceType : public Enum
	{
	public:
		enum Values
		{
			Key,
			MouseButton,
			MousePosition
		};
		RDECLARE_ENUM(InputDeviceType)
		RENUM_H(InputDeviceType, Values, Values::MousePosition + 1)
	};

	TYPE_DEFINE(InputDeviceType)
}

#endif