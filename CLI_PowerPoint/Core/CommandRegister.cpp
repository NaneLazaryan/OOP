#include "CommandRegister.h"
#include <algorithm>
#include <cctype>

using namespace cli;
using namespace cli::cmd;

void CommandRegister::registerCommand(const std::string& name, std::shared_ptr<IMetaCommand> metaCmd)
{
	if (!metaCmd) return;

	std::string lowerName = name;
	std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);

	registry[lowerName] = metaCmd;
}

void CommandRegister::unregisterCommand(const std::string& name)
{
	std::string lowerName = name;
	std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
	registry.erase(lowerName);
}

cmd::factory::CommandCreatorPtr CommandRegister::findCreator(const std::string& name)
{
	std::string lowerName = name;
	std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);

	auto it = registry.find(lowerName);
	if (it != registry.end())
		return it->second->getCreator();

	return nullptr;
}

std::shared_ptr<IMetaCommand> CommandRegister::findMetaCommand(const std::string& name)
{
	std::string lowerName = name;
	std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);

	auto it = registry.find(lowerName);
	if (it != registry.end())
		return it->second;

	return nullptr;
}

std::vector<std::string> CommandRegister::getAllCommandName() const 
{
	std::vector<std::string> names;
	for (const auto& pair : registry) {
		names.push_back(pair.second->getName());
	}
	return names;
}

std::vector<std::shared_ptr<IMetaCommand>> CommandRegister::getAllMetaCommands() const
{
	std::vector<std::shared_ptr<IMetaCommand>> commands;
	for (const auto& pair : registry) {
		commands.push_back(pair.second);
	}
	return commands;
}