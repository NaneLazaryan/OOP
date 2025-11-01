#pragma once

#include  <string>
#include <sstream>
#include <vector>
#include <memory>

#include "Presentation.h"
#include "ShapeFactory.h"

using namespace document::shapes;

namespace document
{
	class JsonSerializer
	{
	public:
        // Save presentation to JSON file
        static bool saveToFile(const Presentation& pres, const std::string& filename);
        // Load presentation from JSON file
        static bool loadFromFile(Presentation& pres, const std::string& filename);
        // Serialize presentation to JSON 
        static std::string serialize(const Presentation& pres);
        // Deserialize JSON to presentation
        static bool deserialize(Presentation& pres, const std::string& jsonStr);

	private:
        static std::string serializeSlide(const Slide& slide, size_t indent = 2);
        static std::string serializeShape(const IShape& shape, size_t indent = 4);
        static std::string serializePoint(const Point& point);
        static std::string serializeBoundingBox(const BoundingBox& box);

        static SlidePtr deserializeSlide(const std::string& jsonStr, size_t& pos);
        static ShapePtr deserializeShape(const std::string& jsonStr, size_t& pos);
        static Point deserializePoint(const std::string& jsonStr, size_t& pos);

        // JSON parsing utilities
        static std::string getIndent(size_t level);
        static std::string escapeString(const std::string& str);
        static std::string unescapeString(const std::string& str);

        static void skipWhitespace(const std::string& str, size_t& pos);
        static bool expectChar(const std::string& str, size_t& pos, char expected);
        static std::string parseString(const std::string& str, size_t& pos);
        static int parseInt(const std::string& str, size_t& pos);
        static float parseFloat(const std::string& str, size_t& pos);
        static std::string parseKey(const std::string& str, size_t& pos);
	};
}
