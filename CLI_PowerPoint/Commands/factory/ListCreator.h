#pragma once

#include "ICmdCreator.h"
#include "../ListCommand.h"

namespace cli::cmd::factory
{
    class ListCreator : public ICmdCreator
    {
    public:
        CommandPtr createCommand(const ArgMap& args) override
        {
            return std::make_unique<ListCommand>();
        }
    };
}