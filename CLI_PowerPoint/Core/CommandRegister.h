#pragma once

#include "../Commands/factory/ICmdCreator.h"
#include <string>
#include <unordered_map>

namespace cli
{
	class CommandRegister
	{
	public:
		void registerCommand(const std::string& name, cmd::factory::CommandCreatorPtr creator);
		void unregisterCommand(const std::string& name);

		cmd::factory::CommandCreatorPtr find(const std::string& name);

	private:
		std::unordered_map<std::string, cmd::factory::CommandCreatorPtr> registry;
	};

}
