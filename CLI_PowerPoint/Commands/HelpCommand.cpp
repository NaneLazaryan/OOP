#include "HelpCommand.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

using namespace cli::cmd;

HelpCommand::HelpCommand(CommandRegister& registry, const std::string& specificCmd)
	: m_registry(registry), m_specificCommand(specificCmd) {}

void HelpCommand::execute(editing::Editor& editor)
{
    if (m_specificCommand.empty()) {
        showAllCommands();
    }
    else {
        showSpecificCommand(m_specificCommand);
    }
}

void HelpCommand::showAllCommands()
{
    std::cout << "\n========================================\n";
    std::cout << "  Available Commands\n";
    std::cout << "========================================\n\n";

    auto metaCommands = m_registry.getAllMetaCommands();

    // Sort commands 
    std::sort(metaCommands.begin(), metaCommands.end(),
        [](const auto& a, const auto& b) {
            return a->getName() < b->getName();
        });
    
    // Find the longest command name for formatting
    size_t maxLength = 0;
    for (const auto& metaCmd : metaCommands) {
        maxLength = std::max(maxLength, metaCmd->getName().length());
    }

    for (const auto& metaCmd : metaCommands) {
        std::cout << "  " << std::left << std::setw(maxLength + 2)
            << metaCmd->getName() << " - "
            << metaCmd->getDescription() << "\n";
    }

    std::cout << "\n========================================\n";
    std::cout << "Use 'help <command>' for detailed information.\n";
    std::cout << "Example: help add slide\n";
    std::cout << "========================================\n\n";
}

void HelpCommand::showSpecificCommand(const std::string& cmdName)
{
    auto metaCmd = m_registry.findMetaCommand(cmdName);
    if (!metaCmd) {
        std::cout << "\nError: Unknown command '" << cmdName << "'\n";
        std::cout << "Use 'help' to see all available commands.\n\n";
        return;
    }

    std::cout << "\n========================================\n";
    std::cout << "  Command: " << metaCmd->getName() << "\n";
    std::cout << "========================================\n\n";

    std::cout << "Description:\n";
    std::cout << "  " << metaCmd->getDescription() << "\n\n";

    auto args = metaCmd->getArguments();
    if (!args.empty()) {
        std::cout << "Arguments:\n";

        // Separate required and optional arguments
        std::vector<ArgumentInfo> required;
        std::vector<ArgumentInfo> optional;

        for (const auto& arg : args) {
            if (arg.required) {
                required.push_back(arg);
            }
            else {
                optional.push_back(arg);
            }
        }

        // Show required arguments
        if (!required.empty()) {
            std::cout << "\n  Required:\n";
            for (const auto& arg : required) {
                std::cout << "    " << std::setw(20) << std::left << arg.name;
                std::cout << "\n";
            }
        }

        // Show optional arguments
        if (!optional.empty()) {
            std::cout << "\n  Optional:\n";
            for (const auto& arg : optional) {
                std::cout << "    " << std::setw(20) << std::left << arg.name;
                if (!arg.defaultValue.empty()) {
                    std::cout << " (default: " << arg.defaultValue << ")";
                }
                std::cout << "\n";
            }
        }
    }

    std::cout << "\n========================================\n\n";
}
