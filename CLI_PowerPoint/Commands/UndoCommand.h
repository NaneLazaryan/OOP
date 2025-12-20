#pragma once

#include "Command.h"

namespace cli::cmd
{
    class UndoCommand : public ICommand
    {
    public:
        UndoCommand() = default;
        ~UndoCommand() = default;

        void execute(editing::Editor& editor)
        {
            editor.undo();
        }
    };
}