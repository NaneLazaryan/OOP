#pragma once

#include "../Core/CommandRegister.h"
#include "../Core/Controller.h"
#include "Presentation.h"
#include "Editor.h"
#include <memory>

namespace cli // ??????????????????
{
	class Application
	{
	public:
		Application() = default;
		void run();

	private:
		void registerCommands(CommandRegister& registry);
	};
}