#ifndef GEM_APP_HANDLER_H
#define GEM_APP_HANDLER_H

#include "../Engine.h"

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
	};

	TYPE_DEFINE(AppHandler)
}

#endif