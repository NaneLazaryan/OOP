#pragma once

#include "IDeserializer.h"
#include "../Models/Slide.h"
#include <memory>
#include <stdexcept>

using namespace document;
using namespace document::shapes;
using namespace document::shapes::utility;

namespace serialization
{
	class JsonDeserializer : public IDeserializer
	{
	public:
		bool loadFromFile(document::Presentation& pres, const std::string& filename);
		bool deserialize(Presentation& pres, const std::string& jsonStr);
	private:
		static std::shared_ptr<Slide> deserializeSlide(const std::string& jsonStr, size_t& pos);
		static std::unique_ptr<IObject> deserializeShape(const std::string& jsonStr, size_t& pos);
	};
}

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
