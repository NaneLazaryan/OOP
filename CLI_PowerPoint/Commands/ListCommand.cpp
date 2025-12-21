#include "ListCommand.h"
#include "../Models/objects/Circle.h"
#include "../Models/objects/Rectangle.h"
#include "../Models/objects/Text.h"
#include "../Models/objects/Image.h"
#include "../Models/objects/Line.h"
#include <iostream>
#include <sstream>

using namespace cli::cmd;
using namespace document;
using namespace document::shapes;

void ListCommand::execute(editing::Editor& editor)
{
    const Presentation& presentation = editor.getPresentation();
    size_t slideCount = presentation.getSlideCount();

    if (slideCount == 0) {
        std::cout << "\nPresentation is empty (no slides).\n\n";
        return;
    }

    std::cout << "\n========================================\n";
    std::cout << "  Presentation Contents\n";
    std::cout << "========================================\n";
    std::cout << "Total Slides: " << slideCount << "\n\n";

    for (size_t i = 0; i < slideCount; ++i) {
        const Slide& slide = presentation.getSlide(i);
        printSlideInfo(slide, i);
        std::cout << "\n";
    }

    std::cout << "========================================\n\n";
}

void ListCommand::printSlideInfo(const Slide& slide, size_t slideNumber)
{
    std::cout << "Slide " << (slideNumber + 1) << " (ID: " << slide.getId() << ")\n";
    std::cout << std::string(40, '-') << "\n";

    size_t shapeCount = slide.getShapeCount();

    if (shapeCount == 0) {
        std::cout << "  (empty slide)\n";
        return;
    }

    std::cout << "  Shapes: " << shapeCount << "\n";

    for (size_t i = 0; i < shapeCount; ++i) {
        const IObject& shape = slide.getShape(i);
        printShapeInfo(shape, i);
    }
}

void ListCommand::printShapeInfo(const IObject& shape, size_t shapeIndex)
{
    const auto& geometry = shape.getGeometry();
    const auto& topLeft = geometry.getTopLeft();
    const auto& bottomRight = geometry.getBottomRight();

    std::cout << "\n  [" << shapeIndex << "] " << getShapeTypeString(shape.getType()) << "\n";
    std::cout << "      Position: (" << topLeft.x << ", " << topLeft.y << ")\n";
    std::cout << "      Size: " << (bottomRight.x - topLeft.x) << " x "
        << (bottomRight.y - topLeft.y) << "\n";

    // Type-specific information
    switch (shape.getType()) {
    case IObject::ObjectType::Circle:
    case IObject::ObjectType::Rectangle:
    {
        if (shape.hasFillColor()) {
            const auto& fillColor = shape.getFillColor();
            std::cout << "      Fill Color: " << getColorString(fillColor) << "\n";
        }

        if (shape.hasBorder() && shape.getBorder().isVisible()) {
            const auto& border = shape.getBorder();
            std::cout << "      Border: " << getColorString(border.getColor())
                << ", thickness: " << border.getThickness() << "\n";
        }
        break;
    }

    case IObject::ObjectType::Line:
    {
        const Line* line = dynamic_cast<const Line*>(&shape);
        if (line) {
            std::cout << "      From: (" << topLeft.x << ", " << topLeft.y << ") "
                << "To: (" << bottomRight.x << ", " << bottomRight.y << ")\n";
            std::cout << "      Color: " << getColorString(line->getLineColor()) << "\n";
            std::cout << "      Thickness: " << line->getThickness() << "\n";
        }
        break;
    }

    case IObject::ObjectType::Text:
    {
        const Text* text = dynamic_cast<const Text*>(&shape);
        if (text) {
            std::cout << "      Text: \"" << text->getText() << "\"\n";
            std::cout << "      Text Color: " << getColorString(text->getTextColor()) << "\n";

            if (text->hasFillColor() && text->getFillColor().getAlpha() > 0) {
                std::cout << "      Background: " << getColorString(text->getFillColor()) << "\n";
            }

            if (text->hasBorder() && text->getBorder().isVisible()) {
                const auto& border = text->getBorder();
                std::cout << "      Border: " << getColorString(border.getColor())
                    << ", thickness: " << border.getThickness() << "\n";
            }
        }
        break;
    }

    case IObject::ObjectType::Image:
    {
        const Image* image = dynamic_cast<const Image*>(&shape);
        if (image) {
            std::cout << "      Image Path: \"" << image->getImagePath() << "\"\n";

            if (image->hasBorder() && image->getBorder().isVisible()) {
                const auto& border = image->getBorder();
                std::cout << "      Border: " << getColorString(border.getColor())
                    << ", thickness: " << border.getThickness() << "\n";
            }
        }
        break;
    }
    }
}

std::string ListCommand::getShapeTypeString(IObject::ObjectType type)
{
    switch (type) {
    case IObject::ObjectType::Circle:
        return "Circle";
    case IObject::ObjectType::Rectangle:
        return "Rectangle";
    case IObject::ObjectType::Line:
        return "Line";
    case IObject::ObjectType::Text:
        return "Text";
    case IObject::ObjectType::Image:
        return "Image";
    default:
        return "Unknown";
    }
}

std::string ListCommand::getColorString(const utility::Color& color)
{
    std::ostringstream oss;
    oss << "RGB("
        << static_cast<int>(color.getRed()) << ", "
        << static_cast<int>(color.getGreen()) << ", "
        << static_cast<int>(color.getBlue());

    if (color.getAlpha() < 255) {
        oss << ", " << static_cast<int>(color.getAlpha());
    }

    oss << ")";
    return oss.str();
}