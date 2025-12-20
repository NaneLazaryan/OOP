#include "JsonDeserializer.h"
#include "../Models/objects/Circle.h"
#include "../Models/objects/Rectangle.h"
#include "../Models/objects/Text.h"
#include "../Models/objects/Image.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cctype>

// Helper functions for JSON parsing
namespace serialization::utility
{
    void skipWhitespace(const std::string& json, size_t& pos) {
        while (pos < json.length() && std::isspace(json[pos])) {
            ++pos;
        }
    }

    void expectChar(const std::string& json, size_t& pos, char expected) {
        skipWhitespace(json, pos);
        if (pos >= json.length() || json[pos] != expected) {
            throw std::runtime_error(std::string("Expected '") + expected + "'");
        }
        ++pos;
    }

    std::string parseString(const std::string& json, size_t& pos) {
        skipWhitespace(json, pos);
        expectChar(json, pos, '"');

        std::string result;
        while (pos < json.length() && json[pos] != '"') {
            if (json[pos] == '\\' && pos + 1 < json.length()) {
                ++pos;
                result += json[pos];
            }
            else {
                result += json[pos];
            }
            ++pos;
        }

        expectChar(json, pos, '"');
        return result;
    }

    int parseInt(const std::string& json, size_t& pos) {
        skipWhitespace(json, pos);

        bool negative = false;
        if (pos < json.length() && json[pos] == '-') {
            negative = true;
            ++pos;
        }

        int value = 0;
        while (pos < json.length() && std::isdigit(json[pos])) {
            value = value * 10 + (json[pos] - '0');
            ++pos;
        }

        return negative ? -value : value;
    }

    double parseDouble(const std::string& json, size_t& pos) {
        skipWhitespace(json, pos);

        size_t start = pos;
        if (pos < json.length() && json[pos] == '-') {
            ++pos;
        }

        while (pos < json.length() && (std::isdigit(json[pos]) || json[pos] == '.')) {
            ++pos;
        }

        std::string numStr = json.substr(start, pos - start);
        return std::stod(numStr);
    }

    bool parseBool(const std::string& json, size_t& pos) {
        skipWhitespace(json, pos);

        if (json.substr(pos, 4) == "true") {
            pos += 4;
            return true;
        }
        else if (json.substr(pos, 5) == "false") {
            pos += 5;
            return false;
        }

        throw std::runtime_error("Expected boolean value");
    }

    std::string parseKey(const std::string& json, size_t& pos) {
        std::string key = parseString(json, pos);
        expectChar(json, pos, ':');
        return key;
    }

    Color parseColor(const std::string& json, size_t& pos) {
        expectChar(json, pos, '{');

        uint8_t r = 0, g = 0, b = 0, a = 255;

        while (true) {
            skipWhitespace(json, pos);
            if (json[pos] == '}') {
                ++pos;
                break;
            }

            std::string key = parseKey(json, pos);

            if (key == "r") {
                r = static_cast<uint8_t>(parseInt(json, pos));
            }
            else if (key == "g") {
                g = static_cast<uint8_t>(parseInt(json, pos));
            }
            else if (key == "b") {
                b = static_cast<uint8_t>(parseInt(json, pos));
            }
            else if (key == "a") {
                a = static_cast<uint8_t>(parseInt(json, pos));
            }

            skipWhitespace(json, pos);
            if (json[pos] == ',') {
                ++pos;
            }
        }

        return Color(r, g, b, a);
    }

    Border parseBorder(const std::string& json, size_t& pos) {
        expectChar(json, pos, '{');

        Color color = Color::Black();
        double thickness = 1.0;
        bool visible = true;

        while (true) {
            skipWhitespace(json, pos);
            if (json[pos] == '}') {
                ++pos;
                break;
            }

            std::string key = parseKey(json, pos);

            if (key == "color") {
                color = parseColor(json, pos);
            }
            else if (key == "thickness") {
                thickness = parseDouble(json, pos);
            }
            else if (key == "visible") {
                visible = parseBool(json, pos);
            }

            skipWhitespace(json, pos);
            if (json[pos] == ',') {
                ++pos;
            }
        }

        return Border(color, thickness, visible);
    }

    Geometry parseGeometry(const std::string& json, size_t& pos) {
        expectChar(json, pos, '{');

        int x1 = 0, y1 = 0, x2 = 0, y2 = 0;

        while (true) {
            skipWhitespace(json, pos);
            if (json[pos] == '}') {
                ++pos;
                break;
            }

            std::string key = parseKey(json, pos);

            if (key == "x1") {
                x1 = parseInt(json, pos);
            }
            else if (key == "y1") {
                y1 = parseInt(json, pos);
            }
            else if (key == "x2") {
                x2 = parseInt(json, pos);
            }
            else if (key == "y2") {
                y2 = parseInt(json, pos);
            }

            skipWhitespace(json, pos);
            if (json[pos] == ',') {
                ++pos;
            }
        }

        return Geometry(Point(x1, y1), Point(x2, y2));
    }
}

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