#pragma once

#include "../Models/IObjectVisitor.h"
#include "../Models/IObject.h"
#include <sstream>
#include <string>

class JsonSerializationVisitor : public document::shapes::IObjectVisitor
{
public:
    explicit JsonSerializationVisitor(size_t indent = 4);

    void visit(const document::shapes::Rectangle& rect);
    void visit(const document::shapes::Circle& circle);
    void visit(const document::shapes::Text& text);
    void visit(const document::shapes::Image& image);
    void visit(const document::shapes::Line& line);

    std::string getResult() { return m_result.str(); }
    void reset();

private:
	std::ostringstream m_result;
    size_t m_indent;

    std::string getIndentStr() const;
    std::string getInnerIndentStr() const;

    void serializeCommonProperties(const document::shapes::IObject& shape);
    void serializeGeometry(const document::shapes::utility::Geometry& geometry);
    void serializeColor(const document::shapes::utility::Color& color, const std::string& key);
    void serializeBorder(const document::shapes::utility::Border& border);
};