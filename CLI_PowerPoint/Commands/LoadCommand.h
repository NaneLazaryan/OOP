#pragma once

#include "Command.h"
#include <string>

namespace cli::cmd
{
    class LoadCommand : public ICommand
    {
    public:
        LoadCommand(const std::string& filename);
        ~LoadCommand() = default;
        void execute(editing::Editor& editor);

    private:
        std::string m_filename;
    };
}