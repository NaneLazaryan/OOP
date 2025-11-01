#include "Serializer.h"
#include <fstream>
#include <stdexcept>

using namespace document;

bool JsonSerializer::saveToFile(const Presentation& pres, const std::string& filename)
{
	try {
		std::string jsonStr = serialize(pres);

		std::ofstream file(filename);
		if (!file.is_open()) {
			return false;
		}

		file << jsonStr;
		file.close();

		return true;
	}
	catch (const std::exception&) {
		return false;
	}
}

bool JsonSerializer::loadFromFile(Presentation& pres, const std::string& filename)
{
	try {
		std::ifstream file(filename);
		if (!file.is_open()) {
			return false;
		}

		std::string jsonStr((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		file.close();

		if (jsonStr.empty()) {
			return false; // Empty file
		}

		return deserialize(pres, jsonStr);
	}
	catch (const std::exception&) {
		return false;
	}
}


std::string JsonSerializer::serialize(const Presentation& pres)
{
	std::ostringstream oss;

	oss << "{\n";
	oss << getIndent(1) << "\"slideCount\": " << pres.getSlideCount() << ",\n";
	oss << getIndent(1) << "\"slides\": [\n";

	const auto& slides = pres.getSlides();
	for (size_t i = 0; i < slides.size(); i++) {
		oss << serializeSlide(*slides[i], 2);
		if (i < slides.size() - 1) {
			oss << ",";
		}
		oss << "\n";
	}

	oss << getIndent(1) << "]\n";
	oss << "}";

	return oss.str();
}

bool JsonSerializer::deserialize(Presentation& pres, const std::string& jsonStr)
{
	try {
		size_t pos = 0;
		skipWhitespace(jsonStr, pos);

		if (!expectChar(jsonStr, pos, '{')) {
			return false;
		}

		// Parse slideCount and slides array
		while (pos < jsonStr.length()) {
			skipWhitespace(jsonStr, pos);

			if (pos >= jsonStr.length()) {
				break;
			}

			if (jsonStr[pos] == '}') {
				pos++;
				break;
			}

			std::string key = parseKey(jsonStr, pos);
			skipWhitespace(jsonStr, pos);

			if (!expectChar(jsonStr, pos, ':')) {
				return false;
			}

			skipWhitespace(jsonStr, pos);

			if (key == "slideCount") {
				parseInt(jsonStr, pos); 
			}
			else if (key == "slides") {
				if (!expectChar(jsonStr, pos, '[')) {
					return false;
				}

				while (pres.getSlideCount() > 0) {
					pres.removeSlide(0);
				}

				// Parse slides
				while (pos < jsonStr.length()) {
					skipWhitespace(jsonStr, pos);

					if (pos >= jsonStr.length()) {
						return false;
					}

					if (jsonStr[pos] == ']') {
						pos++;
						break;
					}

					SlidePtr slide = deserializeSlide(jsonStr, pos);
					if (!slide) {
						return false; 
					}
					pres.addSlide(pres.getSlideCount(), slide);

					skipWhitespace(jsonStr, pos);
					if (pos < jsonStr.length() && jsonStr[pos] == ',') {
						pos++;
					}
				}
			}

			skipWhitespace(jsonStr, pos);
			if (pos < jsonStr.length() && jsonStr[pos] == ',') {
				pos++;
			}
		}

		return true;
	}
	catch (const std::exception&) {
		return false;
	}
}



std::string JsonSerializer::serializeSlide(const Slide& slide, size_t indent)
{
	std::ostringstream oss;
	std::string ind = getIndent(indent);

	oss << ind << "{\n";
	oss << ind << "  \"id\": " << slide.getId() << ",\n";
	oss << ind << "  \"shapeCount\": " << slide.getShapeCount() << ",\n";
	oss << ind << "  \"shapes\": [\n";

	const auto& shapes = slide.getShapes();
	for (size_t i = 0; i < shapes.size(); ++i) {
		oss << serializeShape(*shapes[i], indent + 2);
		if (i < shapes.size() - 1) {
			oss << ",";
		}
		oss << "\n";
	}

	oss << ind << "  ]\n";
	oss << ind << "}";

	return oss.str();
}

std::string JsonSerializer::serializeShape(const IShape& shape, size_t indent)
{
	std::ostringstream oss;
	std::string ind = getIndent(indent);

	oss << ind << "{\n";

	// Serialize shape type
	std::string shapeType;
	switch (shape.getType()) {
	case IShape::ShapeType::Circle:
		shapeType = "Circle";
		break;
	case IShape::ShapeType::Rectangle:
		shapeType = "Rectangle";
		break;
	case IShape::ShapeType::Text:
		shapeType = "Text";
		break;
	default:
		shapeType = "Unknown";
	}

	oss << ind << "  \"type\": \"" << shapeType << "\",\n";
	oss << ind << "  \"position\": " << serializePoint(shape.getPosition()) << ",\n";
	oss << ind << "  \"boundingBox\": " << serializeBoundingBox(shape.getBoundingBox());

	// Serialize properties
	if (shape.getType() == IShape::ShapeType::Circle) {
		const Circle* circle = dynamic_cast<const Circle*>(&shape);
		if (circle) {
			oss << ",\n" << ind << "  \"radius\": " << circle->getRadius();
		}
	}
	else if (shape.getType() == IShape::ShapeType::Rectangle) {
		const Rectangle* rect = dynamic_cast<const Rectangle*>(&shape);
		if (rect) {
			oss << ",\n" << ind << "  \"width\": " << rect->getWidth();
			oss << ",\n" << ind << "  \"height\": " << rect->getHeight();
		}
	}
	else if (shape.getType() == IShape::ShapeType::Text) {
		const Text* text = dynamic_cast<const Text*>(&shape);
		if (text) {
			oss << ",\n" << ind << "  \"content\": \"" << escapeString(text->getContent()) << "\"";
			oss << ",\n" << ind << "  \"fontSize\": " << text->getFontSize();
		}
	}

	oss << "\n" << ind << "}";

	return oss.str();
}

std::string JsonSerializer::serializePoint(const Point& point)
{
	std::ostringstream oss;
	oss << "{\"x\": " << point.x << ", \"y\": " << point.y << "}";
	return oss.str();
}

std::string JsonSerializer::serializeBoundingBox(const BoundingBox& box)
{
	std::ostringstream oss;
	oss << "{\"topLeft\": " << serializePoint(box.getTopLeft())
		<< ", \"bottomRight\": " << serializePoint(box.getBottomRight()) << "}";
	return oss.str();
}

SlidePtr JsonSerializer::deserializeSlide(const std::string& jsonStr, size_t& pos)
{
	skipWhitespace(jsonStr, pos);

	if (!expectChar(jsonStr, pos, '{')) {
		return nullptr;
	}

	auto slide = std::make_shared<Slide>();

	while (pos < jsonStr.length()) {
		skipWhitespace(jsonStr, pos);

		if (pos >= jsonStr.length()) {
			break;
		}

		if (jsonStr[pos] == '}') {
			pos++;
			break;
		}

		std::string key = parseKey(jsonStr, pos);
		skipWhitespace(jsonStr, pos);

		if (!expectChar(jsonStr, pos, ':')) {
			return nullptr;
		}

		skipWhitespace(jsonStr, pos);

		if (key == "id") {
			int id = parseInt(jsonStr, pos);
			slide->setId(id);
		}
		else if (key == "shapeCount") {
			parseInt(jsonStr, pos); // Skip for now
		}
		else if (key == "shapes") {
			if (!expectChar(jsonStr, pos, '[')) {
				return nullptr;
			}

			while (pos < jsonStr.length()) {
				skipWhitespace(jsonStr, pos);

				if (pos >= jsonStr.length()) {
					return nullptr;
				}

				if (jsonStr[pos] == ']') {
					pos++;
					break;
				}

				ShapePtr shape = deserializeShape(jsonStr, pos);
				if (!shape) {
					return nullptr; 
				}
				slide->addShape(std::move(shape));

				skipWhitespace(jsonStr, pos);
				if (pos < jsonStr.length() && jsonStr[pos] == ',') {
					pos++;
				}
			}
		}

		skipWhitespace(jsonStr, pos);
		if (jsonStr[pos] == ',') {
			pos++;
		}
	}

	return slide;
}

ShapePtr JsonSerializer::deserializeShape(const std::string& jsonStr, size_t& pos)
{
	skipWhitespace(jsonStr, pos);

	if (!expectChar(jsonStr, pos, '{')) {
		return nullptr;
	}

	std::string shapeType;
	Point position;
	float radius = 10.0f;
	float width = 10.0f;
	float height = 10.0f;
	std::string content;
	float fontSize = 12.0f;

	while (pos < jsonStr.length()) {
		skipWhitespace(jsonStr, pos);

		if (pos >= jsonStr.length()) {
			break;
		}

		if (jsonStr[pos] == '}') {
			pos++;
			break;
		}

		std::string key = parseKey(jsonStr, pos);
		skipWhitespace(jsonStr, pos);

		if (!expectChar(jsonStr, pos, ':')) {
			return nullptr;
		}

		skipWhitespace(jsonStr, pos);

		if (key == "type") {
			shapeType = parseString(jsonStr, pos);
		}
		else if (key == "position") {
			position = deserializePoint(jsonStr, pos);
		}
		else if (key == "boundingBox") {
			// Skip bounding box 
			int braceCount = 1;
			pos++; // skip opening brace
			while (pos < jsonStr.length() && braceCount > 0) {
				if (jsonStr[pos] == '{') braceCount++;
				else if (jsonStr[pos] == '}') braceCount--;
				pos++;
			}
		}
		else if (key == "radius") {
			radius = parseFloat(jsonStr, pos);
		}
		else if (key == "width") {
			width = parseFloat(jsonStr, pos);
		}
		else if (key == "height") {
			height = parseFloat(jsonStr, pos);
		}
		else if (key == "content") {
			content = parseString(jsonStr, pos);
		}
		else if (key == "fontSize") {
			fontSize = parseFloat(jsonStr, pos);
		}

		skipWhitespace(jsonStr, pos);
		if (pos < jsonStr.length() && jsonStr[pos] == ',') {
			pos++;
		}
	}

	// Create shape based on type
	ShapePtr shape;
	if (shapeType == "Circle") {
		shape = std::make_unique<Circle>(radius, position.x, position.y);
	}
	else if (shapeType == "Rectangle") {
		shape = std::make_unique<Rectangle>(width, height, position.x, position.y);
	}
	else if (shapeType == "Text") {
		shape = std::make_unique<Text>(content, fontSize, position.x, position.y);
	}

	return shape;
}

Point JsonSerializer::deserializePoint(const std::string& jsonStr, size_t& pos)
{
	Point point{ 0.0f, 0.0f };

	skipWhitespace(jsonStr, pos);
	if (!expectChar(jsonStr, pos, '{')) {
		return point;
	}

	while (pos < jsonStr.length()) {
		skipWhitespace(jsonStr, pos);

		if (pos >= jsonStr.length()) {
			break;
		}

		if (jsonStr[pos] == '}') {
			pos++;
			break;
		}

		std::string key = parseKey(jsonStr, pos);
		skipWhitespace(jsonStr, pos);

		if (!expectChar(jsonStr, pos, ':')) {
			return point;
		}

		skipWhitespace(jsonStr, pos);

		if (key == "x") {
			point.x = parseFloat(jsonStr, pos);
		}
		else if (key == "y") {
			point.y = parseFloat(jsonStr, pos);
		}

		skipWhitespace(jsonStr, pos);
		if (pos < jsonStr.length() && jsonStr[pos] == ',') {
			pos++;
		}
	}

	return point;
}

// Utilities
std::string JsonSerializer::getIndent(size_t level)
{
	return std::string(level * 2, ' ');
}

std::string JsonSerializer::escapeString(const std::string& str)
{
	std::string result;
	for (char c : str) {
		switch (c) {
		case '"':  result += "\\\""; break;
		case '\\': result += "\\\\"; break;
		case '\n': result += "\\n"; break;
		case '\r': result += "\\r"; break;
		case '\t': result += "\\t"; break;
		default:   result += c;
		}
	}
	return result;
}

std::string JsonSerializer::unescapeString(const std::string& str)
{
	std::string result;
	for (size_t i = 0; i < str.length(); ++i) {
		if (str[i] == '\\' && i + 1 < str.length()) {
			switch (str[i + 1]) {
			case '"':  result += '"'; i++; break;
			case '\\': result += '\\'; i++; break;
			case 'n':  result += '\n'; i++; break;
			case 'r':  result += '\r'; i++; break;
			case 't':  result += '\t'; i++; break;
			default:   result += str[i];
			}
		}
		else {
			result += str[i];
		}
	}
	return result;
}

void JsonSerializer::skipWhitespace(const std::string& str, size_t& pos)
{
	while (pos < str.length() && std::isspace(static_cast<unsigned char>(str[pos]))) {
		pos++;
	}
}

bool JsonSerializer::expectChar(const std::string& str, size_t& pos, char expected)
{
	skipWhitespace(str, pos);
	if (pos >= str.length() || str[pos] != expected) {
		return false;
	}
	pos++;
	return true;
}

std::string JsonSerializer::parseString(const std::string& str, size_t& pos)
{
	skipWhitespace(str, pos);

	if (pos >= str.length() || str[pos] != '"') {
		throw std::runtime_error("Expected string");
	}

	pos++; // skip opening quote
	std::string result;

	while (pos < str.length() && str[pos] != '"') {
		if (str[pos] == '\\' && pos + 1 < str.length()) {
			pos++;
			switch (str[pos]) {
			case '"':  result += '"'; break;
			case '\\': result += '\\'; break;
			case 'n':  result += '\n'; break;
			case 'r':  result += '\r'; break;
			case 't':  result += '\t'; break;
			default:   result += str[pos];
			}
		}
		else {
			result += str[pos];
		}
		pos++;
	}

	if (pos < str.length() && str[pos] == '"') pos++;  // skip closing quote

	return result;
}

int JsonSerializer::parseInt(const std::string& str, size_t& pos)
{
	skipWhitespace(str, pos);

	std::string numStr;
	if (str[pos] == '-') {
		numStr += str[pos++];
	}

	while (pos < str.length() && std::isdigit(static_cast<unsigned char>(str[pos]))) {
		numStr += str[pos++];
	}

	return std::stoi(numStr);
}

float JsonSerializer::parseFloat(const std::string& str, size_t& pos)
{
	skipWhitespace(str, pos);

	std::string numStr;
	if (str[pos] == '-') {
		numStr += str[pos++];
	}

	while (pos < str.length() && (std::isdigit(static_cast<unsigned char>(str[pos])) || str[pos] == '.')) {
		numStr += str[pos++];
	}

	return std::stof(numStr);
}

std::string JsonSerializer::parseKey(const std::string& str, size_t& pos)
{
	return parseString(str, pos);
}