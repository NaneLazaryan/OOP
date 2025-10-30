#pragma once

#include <memory>
#include "Command.h"
#include "Tokenizer.h"

class CommandFactory
{
public:
	static std::unique_ptr<Command> createCommand(Keyword action, Keyword target);
	static bool isValidCombination(Keyword action, Keyword target);

private:
	CommandFactory() = default;

	static std::unique_ptr<Command> createAddCommand(Keyword target);
	static std::unique_ptr<Command> createRemoveCommand(Keyword target);
	static std::unique_ptr<Command> createSetCommand(Keyword target);
	static std::unique_ptr<Command> createEditCommand(Keyword target);
};