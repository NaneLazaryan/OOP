#pragma once

#include "ICmdCreator.h"
#include "../SaveCommand.h"
#include "../../Core/Parser.h"

namespace cli::cmd::factory
{
    class SaveCreator : public ICmdCreator
    {
    public:
        CommandPtr createCommand(const ArgMap& args) override
        {
            std::string filename;

            // Try to get filename from first positional argument
            auto it0 = args.find("$0");
            if (it0 != args.end()) {
                filename = std::get<std::string>(it0->second);
            }
            else {
                // Try to get from -file flag
                auto itFile = args.find("-file");
                if (itFile != args.end()) {
                    filename = std::get<std::string>(itFile->second);
                }
                else {
                    throw std::invalid_argument("Filename is required for save command");
                }
            }

            return std::make_unique<SaveCommand>(filename);
        }
    };
}