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
		static std::unique_ptr<cmd::ICommand> createCommand(Keyword action, Keyword target);
		static bool isValidCombination(Keyword action, Keyword target);

	private:
		CommandFactory() = default;

		static std::unique_ptr<cmd::ICommand> createAddCommand(Keyword target);
		static std::unique_ptr<cmd::ICommand> createRemoveCommand(Keyword target);
		static std::unique_ptr<cmd::ICommand> createSetCommand(Keyword target);
		static std::unique_ptr<cmd::ICommand> createEditCommand(Keyword target);
	};
}
