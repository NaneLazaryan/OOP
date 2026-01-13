#include "JsonSerializationVisitor.h"
#include "../Models/objects/Circle.h"
#include "../Models/objects/Rectangle.h"
#include "../Models/objects/Text.h"
#include "../Models/objects/Image.h"
#include "../Models/objects/Line.h"

JsonSerializationVisitor::JsonSerializationVisitor(size_t indent)
	: m_indent(indent)
{}

std::string JsonSerializationVisitor::getIndentStr() const
{
	return std::string(m_indent, ' ');
}

std::string JsonSerializationVisitor::getInnerIndentStr() const
{
	return std::string(m_indent + 2, ' ');
}

void JsonSerializationVisitor::reset()
{
	m_result.str("");
	m_result.clear();
}

void JsonSerializationVisitor::serializeCommonProperties(const document::shapes::IObject& shape)
{
	std::string indentStr = getIndentStr();
	std::string innerIndent = getInnerIndentStr();

	m_result << indentStr << "{\n";
	m_result << innerIndent << "\"type\": \"" << shape.getName() << "\",\n";
	m_result << innerIndent << "\"geometry\": ";
	serializeGeometry(shape.getGeometry());
}

void JsonSerializationVisitor::serializeGeometry(const document::shapes::utility::Geometry& geometry)
{
	auto topLeft = geometry.getTopLeft();
	auto bottomRight = geometry.getBottomRight();

	m_result << "{\"x1\": " << topLeft.x << ", "
		<< "\"y1\": " << topLeft.y << ", "
		<< "\"x2\": " << bottomRight.x << ", "
		<< "\"y2\": " << bottomRight.y << "}";
}

void JsonSerializationVisitor::serializeColor(const document::shapes::utility::Color& color, const std::string& key)
{
	std::string innerIndent = getInnerIndentStr();
	m_result << ",\n" << innerIndent << "\"" << key << "\": {"
		<< "\"r\": " << (int)color.getRed() << ", "
		<< "\"g\": " << (int)color.getGreen() << ", "
		<< "\"b\": " << (int)color.getBlue() << ", "
		<< "\"a\": " << (int)color.getAlpha() << "}";
}

void JsonSerializationVisitor::serializeBorder(const document::shapes::utility::Border& border)
{
	std::string innerIndent = getInnerIndentStr();
	const auto& borderColor = border.getColor();
	m_result << ",\n" << innerIndent << "\"border\": {"
		<< "\"color\": {"
		<< "\"r\": " << (int)borderColor.getRed() << ", "
		<< "\"g\": " << (int)borderColor.getGreen() << ", "
		<< "\"b\": " << (int)borderColor.getBlue() << ", "
		<< "\"a\": " << (int)borderColor.getAlpha() << "}, "
		<< "\"thickness\": " << border.getThickness() << ", "
		<< "\"visible\": " << (border.isVisible() ? "true" : "false") << "}";
}

void JsonSerializationVisitor::visit(const document::shapes::Rectangle& rect)
{
	serializeCommonProperties(rect);

	if (rect.hasBorder()) serializeBorder(rect.getBorder());
	if (rect.hasFillColor()) serializeColor(rect.getFillColor(), "fillColor");
	
	m_result << "\n" << getIndentStr() << "}";
}

void JsonSerializationVisitor::visit(const document::shapes::Circle& circle)
{
	serializeCommonProperties(circle);

	if (circle.hasFillColor()) {
		serializeColor(circle.getFillColor(), "fillColor");
	}

	if (circle.hasBorder()) {
		serializeBorder(circle.getBorder());
	}

	m_result << "\n" << getIndentStr() << "}";
}

void JsonSerializationVisitor::visit(const document::shapes::Text& text)
{
	serializeCommonProperties(text);

	std::string innerIndent = getInnerIndentStr();
	m_result << ",\n" << innerIndent << "\"text\": \"" << text.getText() << "\"";

	serializeColor(text.getTextColor(), "textColor");

	if (text.hasFillColor()) {
		serializeColor(text.getFillColor(), "backgroundColor");
	}

	if (text.hasBorder()) {
		serializeBorder(text.getBorder());
	}

	m_result << "\n" << getIndentStr() << "}";
}

void JsonSerializationVisitor::visit(const document::shapes::Image& image)
{
	serializeCommonProperties(image);

	std::string innerIndent = getInnerIndentStr();
	m_result << ",\n" << innerIndent << "\"imagePath\": \"" << image.getImagePath() << "\"";

	if (image.hasBorder()) {
		serializeBorder(image.getBorder());
	}

	m_result << "\n" << getIndentStr() << "}";
}

void JsonSerializationVisitor::visit(const document::shapes::Line& line)
{
	serializeCommonProperties(line);

	serializeColor(line.getLineColor(), "color");

	std::string innerIndent = getInnerIndentStr();
	m_result << ",\n" << innerIndent << "\"thickness\": " << line.getThickness();

	m_result << "\n" << getIndentStr() << "}";
}