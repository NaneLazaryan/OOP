#pragma once

#include "Command.h"
#include <string>

namespace cli::cmd
{
    class SaveCommand : public ICommand
    {
    public:
        SaveCommand(const std::string& filename);
        ~SaveCommand() = default;
        void execute(editing::Editor& editor);

    private:
        std::string m_filename;
    };
}