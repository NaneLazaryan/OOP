#pragma once
#include "Command.h"
#include "../Models/Slide.h"
#include "../Models/Presentation.h"

namespace cli::cmd
{
    class AddSlideCommand : public ICommand 
    {
    public:
        AddSlideCommand(size_t pos);
        ~AddSlideCommand() = default;
        void execute(editing::Editor& editor);
        
    private:
        int m_slideId;
        size_t m_pos;
    };
}

