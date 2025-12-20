#include "JsonDeserializer.h"
#include "../Models/objects/Circle.h"
#include "../Models/objects/Rectangle.h"
#include "../Models/objects/Text.h"
#include "../Models/objects/Image.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cctype>

using namespace serialization;
using namespace serialization::utility;

bool JsonDeserializer::loadFromFile(document::Presentation& pres, const std::string& filename)
{
    std::ifstream inFile(filename);
    if (!inFile.is_open()) {
        return false;
    }

    std::stringstream buffer;
    buffer << inFile.rdbuf();
    std::string jsonContent = buffer.str();

    inFile.close();

    return deserialize(pres, jsonContent);
}

bool JsonDeserializer::deserialize(Presentation& pres, const std::string& jsonStr)
{
    try {
        size_t pos = 0;
        expectChar(jsonStr, pos, '{');

        skipWhitespace(jsonStr, pos);
        std::string key = parseKey(jsonStr, pos);

        if (key != "slides") {
            throw std::runtime_error("Expected 'slides' key");
        }

        expectChar(jsonStr, pos, '[');

        while (true) {
            skipWhitespace(jsonStr, pos);
            if (jsonStr[pos] == ']') {
                ++pos;
                break;
            }

            auto slide = deserializeSlide(jsonStr, pos);
            pres.addSlide(pres.getSlideCount(), std::move(slide));

            skipWhitespace(jsonStr, pos);
            if (jsonStr[pos] == ',') {
                ++pos;
            }
        }

        return true;
    }
    catch (const std::exception&) {
        return false;
    }
}

std::shared_ptr<Slide> JsonDeserializer::deserializeSlide(const std::string& jsonStr, size_t& pos)
{
    expectChar(jsonStr, pos, '{');

    int slideId = 0;
    auto slide = std::make_unique<Slide>();

    while (true) {
        skipWhitespace(jsonStr, pos);
        if (jsonStr[pos] == '}') {
            ++pos;
            break;
        }

        std::string key = parseKey(jsonStr, pos);

        if (key == "id") {
            slideId = parseInt(jsonStr, pos);
            slide->setId(slideId);
        }
        else if (key == "shapes") {
            expectChar(jsonStr, pos, '[');

            while (true) {
                skipWhitespace(jsonStr, pos);
                if (jsonStr[pos] == ']') {
                    ++pos;
                    break;
                }

                auto shape = deserializeShape(jsonStr, pos);
                if (shape) {
                    slide->addShape(std::move(shape));
                }

                skipWhitespace(jsonStr, pos);
                if (jsonStr[pos] == ',') {
                    ++pos;
                }
            }
        }

        skipWhitespace(jsonStr, pos);
        if (jsonStr[pos] == ',') {
            ++pos;
        }
    }

    return slide;
}

std::unique_ptr<IObject> JsonDeserializer::deserializeShape(const std::string& jsonStr, size_t& pos)
{
    expectChar(jsonStr, pos, '{');

    std::string shapeType;
    Geometry geometry;
    Color fillColor = Color::White();
    Color textColor = Color::Black();
    Color backgroundColor = Color::Transparent();
    Border border;
    std::string text;
    std::string imagePath;
    bool hasFill = false;
    bool hasBorderData = false;

    while (true) {
        skipWhitespace(jsonStr, pos);
        if (jsonStr[pos] == '}') {
            ++pos;
            break;
        }

        std::string key = parseKey(jsonStr, pos);

        if (key == "type") {
            shapeType = parseString(jsonStr, pos);
        }
        else if (key == "geometry") {
            geometry = parseGeometry(jsonStr, pos);
        }
        else if (key == "fillColor") {
            fillColor = parseColor(jsonStr, pos);
            hasFill = true;
        }
        else if (key == "textColor") {
            textColor = parseColor(jsonStr, pos);
        }
        else if (key == "backgroundColor") {
            backgroundColor = parseColor(jsonStr, pos);
        }
        else if (key == "border") {
            border = parseBorder(jsonStr, pos);
            hasBorderData = true;
        }
        else if (key == "text") {
            text = parseString(jsonStr, pos);
        }
        else if (key == "imagePath") {
            imagePath = parseString(jsonStr, pos);
        }

        skipWhitespace(jsonStr, pos);
        if (jsonStr[pos] == ',') {
            ++pos;
        }
    }

    // Create appropriate shape based on type
    if (shapeType == "Circle") {
        if (hasBorderData) {
            return std::make_unique<Circle>(geometry, fillColor, border);
        }
        return std::make_unique<Circle>(geometry);
    }
    else if (shapeType == "Rectangle") {
        if (hasBorderData) {
            return std::make_unique<Rectangle>(geometry, fillColor, border);
        }
        return std::make_unique<Rectangle>(geometry);
    }
    else if (shapeType == "Text") {
        return std::make_unique<Text>(geometry, text, textColor, backgroundColor, border);
    }
    else if (shapeType == "Image") {
        if (hasBorderData) {
            return std::make_unique<Image>(geometry, imagePath, border);
        }
        return std::make_unique<Image>(geometry, imagePath);
    }

    return nullptr;
}