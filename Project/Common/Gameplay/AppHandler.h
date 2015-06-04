#ifndef GEM_APP_HANDLER_H
#define GEM_APP_HANDLER_H

#include "../Engine.h"
#include <random>

namespace Gem
{
	class AppHandler : public BaseAppHandler
	{
		RDECLARE_CLASS(AppHandler)
	public:
		AppHandler();
		~AppHandler();

		void OnStart() override;

		void OnStop() override;

		void OnSystemsInitialized() override;

		void Update() override;

	private:

		Vector2 m_MousePosition;

		std::mt19937 m_MTE;
		std::uniform_int_distribution<int> m_Int;
		std::uniform_real_distribution<float> m_Float;

		float RandomRange(float aMin, float aMax);
		int RandomRange(int aMin, int aMax);
	};

	TYPE_DEFINE(AppHandler)
}

#endif