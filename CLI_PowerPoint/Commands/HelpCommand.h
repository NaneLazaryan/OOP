#pragma once

#include "Command.h"
#include "IMetaCommand.h"
#include "../Core/CommandRegister.h"
#include <map>

namespace cli::cmd
{
	class HelpCommand : public ICommand
	{
	public:
		HelpCommand(CommandRegister& registry, const std::string& specificCmd = "");

		void execute(editing::Editor& editor);
	private:
		CommandRegister& m_registry;
		std::string m_specificCommand;

	private:
		void showAllCommands();
		void showSpecificCommand(const std::string& cmdName);
	};
}