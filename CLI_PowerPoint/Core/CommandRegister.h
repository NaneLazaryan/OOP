#pragma once

#include "../Commands/factory/ICmdCreator.h"
#include "../Commands/IMetaCommand.h"
#include <string>
#include <unordered_map>

namespace cli
{
	class CommandRegister
	{
	public:
		void registerCommand(const std::string& name, std::shared_ptr<cmd::IMetaCommand> metaCmd);
		void unregisterCommand(const std::string& name);

		cmd::factory::CommandCreatorPtr findCreator(const std::string& name);
		std::shared_ptr<cmd::IMetaCommand> findMetaCommand(const std::string& name);

		// Get all commands
		std::vector<std::string> getAllCommandName() const;
		std::vector<std::shared_ptr<cmd::IMetaCommand>> getAllMetaCommands() const;

	private:
		std::unordered_map<std::string, std::shared_ptr<cmd::IMetaCommand>> registry;
	};

}
