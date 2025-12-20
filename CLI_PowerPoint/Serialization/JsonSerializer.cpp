#include "JsonSerializer.h"
#include "../Models/objects/Circle.h"
#include "../Models/objects/Rectangle.h"
#include "../Models/objects/Text.h"
#include "../Models/objects/Image.h"
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace serialization;
using namespace document;
using namespace document::shapes;

bool JsonSerializer::saveToFile(const document::Presentation& pres, const std::string& filename)
{
    std::string jsonContent = serialize(pres);

    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        return false;
    }

    outFile << jsonContent;
    outFile.close();

    return true;
}

std::string JsonSerializer::serialize(const document::Presentation& pres)
{
    std::ostringstream oss;

    oss << "{\n";
    oss << "  \"slides\": [\n";

    size_t slideCount = pres.getSlideCount();
    for (size_t i = 0; i < slideCount; ++i) {
        auto slide = pres.getSlide(i);
        oss << serializeSlide(slide, 4);

        if (i < slideCount - 1) {
            oss << ",";
        }
        oss << "\n";
    }

    oss << "  ]\n";
    oss << "}\n";

    return oss.str();
}

std::string JsonSerializer::serializeSlide(const Slide& slide, size_t indent)
{
    std::ostringstream oss;
    std::string indentStr(indent, ' ');
    std::string innerIndent(indent + 2, ' ');

    oss << indentStr << "{\n";
    oss << innerIndent << "\"id\": " << slide.getId() << ",\n";
    oss << innerIndent << "\"shapes\": [\n";

    size_t shapeCount = slide.getShapeCount();
    for (size_t i = 0; i < shapeCount; ++i) {
        const shapes::IObject& shape = slide.getShape(i);
        oss << serializeShape(shape , indent + 4);

        if (i < shapeCount - 1) {
            oss << ",";
        }
        oss << "\n";
    }

    oss << innerIndent << "]\n";
    oss << indentStr << "}";

    return oss.str();
}

std::string JsonSerializer::serializeShape(const shapes::IObject& shape, size_t indent)
{
    std::ostringstream oss;
    std::string indentStr(indent, ' ');
    std::string innerIndent(indent + 2, ' ');

    oss << indentStr << "{\n";
    oss << innerIndent << "\"type\": \"" << shape.getName() << "\",\n";
    oss << innerIndent << "\"geometry\": " << serializeGeometry(shape.getGeometry());

    // Serialize type-specific properties
    if (shape.getType() == IObject::ObjectType::Circle ||
        shape.getType() == IObject::ObjectType::Rectangle) {

        if (shape.hasFillColor()) {
            const auto& color = shape.getFillColor();
            oss << ",\n" << innerIndent << "\"fillColor\": {"
                << "\"r\": " << (int)color.getRed() << ", "
                << "\"g\": " << (int)color.getGreen() << ", "
                << "\"b\": " << (int)color.getBlue() << ", "
                << "\"a\": " << (int)color.getAlpha() << "}";
        }

        if (shape.hasBorder()) {
            const auto& border = shape.getBorder();
            const auto& borderColor = border.getColor();
            oss << ",\n" << innerIndent << "\"border\": {"
                << "\"color\": {"
                << "\"r\": " << (int)borderColor.getRed() << ", "
                << "\"g\": " << (int)borderColor.getGreen() << ", "
                << "\"b\": " << (int)borderColor.getBlue() << ", "
                << "\"a\": " << (int)borderColor.getAlpha() << "}, "
                << "\"thickness\": " << border.getThickness() << ", "
                << "\"visible\": " << (border.isVisible() ? "true" : "false") << "}";
        }
    }
    else if (shape.getType() == IObject::ObjectType::Text) {
        const Text* textShape = dynamic_cast<const Text*>(&shape);
        if (textShape) {
            oss << ",\n" << innerIndent << "\"text\": \"" << textShape->getText() << "\"";

            const auto& textColor = textShape->getTextColor();
            oss << ",\n" << innerIndent << "\"textColor\": {"
                << "\"r\": " << (int)textColor.getRed() << ", "
                << "\"g\": " << (int)textColor.getGreen() << ", "
                << "\"b\": " << (int)textColor.getBlue() << ", "
                << "\"a\": " << (int)textColor.getAlpha() << "}";

            if (textShape->hasFillColor()) {
                const auto& bgColor = textShape->getFillColor();
                oss << ",\n" << innerIndent << "\"backgroundColor\": {"
                    << "\"r\": " << (int)bgColor.getRed() << ", "
                    << "\"g\": " << (int)bgColor.getGreen() << ", "
                    << "\"b\": " << (int)bgColor.getBlue() << ", "
                    << "\"a\": " << (int)bgColor.getAlpha() << "}";
            }
        }
    }
    else if (shape.getType() == IObject::ObjectType::Image) {
        const Image* imageShape = dynamic_cast<const Image*>(&shape);
        if (imageShape) {
            oss << ",\n" << innerIndent << "\"imagePath\": \"" << imageShape->getImagePath() << "\"";

            if (imageShape->hasBorder()) {
                const auto& border = imageShape->getBorder();
                const auto& borderColor = border.getColor();
                oss << ",\n" << innerIndent << "\"border\": {"
                    << "\"color\": {"
                    << "\"r\": " << (int)borderColor.getRed() << ", "
                    << "\"g\": " << (int)borderColor.getGreen() << ", "
                    << "\"b\": " << (int)borderColor.getBlue() << ", "
                    << "\"a\": " << (int)borderColor.getAlpha() << "}, "
                    << "\"thickness\": " << border.getThickness() << ", "
                    << "\"visible\": " << (border.isVisible() ? "true" : "false") << "}";
            }
        }
    }

    oss << "\n" << indentStr << "}";

    return oss.str();
}

std::string JsonSerializer::serializeGeometry(const shapes::utility::Geometry& geometry)
{
    std::ostringstream oss;

    auto topLeft = geometry.getTopLeft();
    auto bottomRight = geometry.getBottomRight();

    oss << "{\"x1\": " << topLeft.x << ", "
        << "\"y1\": " << topLeft.y << ", "
        << "\"x2\": " << bottomRight.x << ", "
        << "\"y2\": " << bottomRight.y << "}";

    return oss.str();
}