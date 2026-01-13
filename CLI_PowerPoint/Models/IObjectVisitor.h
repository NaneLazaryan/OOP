#pragma once

namespace document::shapes {
	class Rectangle;
	class Circle;
	class Line;
	class Text;
	class Image;
}

namespace document::shapes
{
	// Visitor interface 
	class IObjectVisitor
	{
	public:
		virtual ~IObjectVisitor() = default;
		virtual void visit(const document::shapes::Rectangle& rect) = 0;
		virtual void visit(const document::shapes::Circle& circle) = 0;
		virtual void visit(const document::shapes::Text& text) = 0;
		virtual void visit(const document::shapes::Image& image) = 0;
		virtual void visit(const document::shapes::Line& line) = 0;
	};
}
