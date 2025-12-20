#pragma once

#include "Command.h"
#include "../Models/Slide.h"
#include "../Models/Presentation.h"

namespace cli::cmd
{
    class AddShapeCommand : public ICommand {
    public:
        AddShapeCommand(int slideId,
            document::shapes::IObject::ObjectType shapeType,
            const document::shapes::utility::Geometry& geometry,
            const document::shapes::utility::Color& fillColor,
            const document::shapes::utility::Border& border);
        ~AddShapeCommand() = default;
        void execute(editing::Editor& editor);

    private:
        int m_slideId;
        document::shapes::IObject::ObjectType m_shapeType;
        document::shapes::utility::Geometry m_geometry;
        document::shapes::utility::Color m_fillColor;
        document::shapes::utility::Border m_border;

        std::unique_ptr<document::shapes::IObject> createShape();
    };
}
