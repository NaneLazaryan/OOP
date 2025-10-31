#pragma once

#include <memory>
#include "Command.h"
#include "Tokenizer.h"
#include "Presentation.h"

class CommandFactory
{
public:
	static std::unique_ptr<ICommand> createCommand(Keyword action, Keyword target);
	static bool isValidCombination(Keyword action, Keyword target);

private:
	CommandFactory() = default;

	static std::unique_ptr<ICommand> createAddCommand(Keyword target);
	static std::unique_ptr<ICommand> createRemoveCommand(Keyword target);
	static std::unique_ptr<ICommand> createSetCommand(Keyword target);
	static std::unique_ptr<ICommand> createEditCommand(Keyword target);
};