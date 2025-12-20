#pragma once

#include "Command.h"
#include "../Models/Slide.h"
#include "../Models/Presentation.h"

namespace cli::cmd
{
    class RemoveShapeCommand : public ICommand
    {
    public:
        RemoveShapeCommand(int slideId, size_t shapeIndex);
        ~RemoveShapeCommand() = default;
        void execute(editing::Editor& editor);

    private:
        int m_slideId;
        size_t m_shapeIndex;
    };
}