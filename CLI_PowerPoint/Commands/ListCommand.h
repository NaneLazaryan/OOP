#pragma once

#include "Command.h"

namespace cli::cmd
{
    class ListCommand : public ICommand
    {
    public:
        ListCommand() = default;
        ~ListCommand() = default;
        void execute(editing::Editor& editor);

    private:
        void printSlideInfo(const document::Slide& slide, size_t slideNumber);
        void printShapeInfo(const document::shapes::IObject& shape, size_t shapeIndex);
        std::string getShapeTypeString(document::shapes::IObject::ObjectType type);
        std::string getColorString(const document::shapes::utility::Color& color);
    };
}