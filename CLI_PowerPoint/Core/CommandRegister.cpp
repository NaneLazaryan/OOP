#include "CommandRegister.h"
#include <algorithm>
#include <cctype>

using namespace cli;
using namespace cli::cmd::factory;

void CommandRegister::registerCommand(const std::string& name, CommandCreatorPtr creator)
{
	if (!creator) return;

	std::string lowerName = name;
	std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);

	registry[lowerName] = creator;
}

void CommandRegister::unregisterCommand(const std::string& name)
{
	std::string lowerName = name;
	std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
	registry.erase(lowerName);
}

CommandCreatorPtr CommandRegister::find(const std::string& name)
{
	std::string lowerName = name;
	std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);

	auto it = registry.find(lowerName);
	if (it != registry.end())
		return it->second;

	return nullptr;
}