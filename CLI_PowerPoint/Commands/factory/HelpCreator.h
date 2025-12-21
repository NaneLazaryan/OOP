#pragma once

#include "ICmdCreator.h"
#include "../HelpCommand.h"
#include "../../Core/CommandRegister.h"

namespace cli::cmd::factory
{
	class HelpCreator : public ICmdCreator
	{
	public:
		explicit HelpCreator(CommandRegister& registry) : m_registry(registry) {}

		CommandPtr createCommand(const ArgMap& args)
		{
			std::string specificCommand;
			
            // Try first positional argument
            auto it0 = args.find("$0");
            if (it0 != args.end()) {
                specificCommand = std::get<std::string>(it0->second);

                // Check for second part ("add slide")
                auto it1 = args.find("$1");
                if (it1 != args.end()) {
                    specificCommand += " " + std::get<std::string>(it1->second);
                }
            }

            // Try -cmd flag
            auto itCmd = args.find("-cmd");
            if (itCmd != args.end()) {
                specificCommand = std::get<std::string>(itCmd->second);
            }

            return std::make_unique<HelpCommand>(m_registry, specificCommand);
		}

	private:
		CommandRegister& m_registry;
	};
}