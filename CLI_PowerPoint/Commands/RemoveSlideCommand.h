#pragma once

#include "Command.h"
#include "../Models/Slide.h"
#include "../Models/Presentation.h"

namespace cli::cmd
{
    class RemoveSlideCommand : public ICommand 
    {
    public:
        RemoveSlideCommand(size_t pos);
        ~RemoveSlideCommand() = default;
        void execute(editing::Editor& editor);

    private:
        size_t m_pos;
    };
}

