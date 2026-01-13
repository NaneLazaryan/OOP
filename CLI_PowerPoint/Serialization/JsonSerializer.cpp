#include "JsonSerializer.h"
#include "../Models/objects/Circle.h"
#include "../Models/objects/Rectangle.h"
#include "../Models/objects/Text.h"
#include "../Models/objects/Image.h"
#include "../Models/objects/Line.h"
#include "JsonSerializationVisitor.h"
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
    JsonSerializationVisitor visitor(indent);
    shape.accept(visitor);

    return visitor.getResult();
}