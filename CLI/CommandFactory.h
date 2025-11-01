#pragma once

#include <memory>
#include "Command.h"
#include "Tokenizer.h"
#include "Presentation.h"

namespace cli
{
	class CommandFactory
	{
	public:
		static cmd::CommandPtr createCommand(Keyword action, Keyword target);
		static cmd::CommandPtr createStandaloneCommand(Keyword action);
		static bool isStandaloneCommand(Keyword action);
		static bool isValidCombination(Keyword action, Keyword target);

	private:
		CommandFactory() = default;

		static cmd::CommandPtr createAddCommand(Keyword target);
		static cmd::CommandPtr createRemoveCommand(Keyword target);
		static cmd::CommandPtr createSetCommand(Keyword target);
		static cmd::CommandPtr createEditCommand(Keyword target);
	};
}
