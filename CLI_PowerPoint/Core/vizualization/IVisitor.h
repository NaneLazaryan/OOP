#pragma once

namespace document::shapes {
	class Rectangle;
	class Circle;
	class Text;
	class Image;
}

namespace vizualization
{
	// Visitor interface for drawing operations
	class IVisitor
	{
	public:
		virtual ~IVisitor() = default;
		virtual void visit(const document::shapes::Rectangle& rect) = 0;
		virtual void visit(const document::shapes::Circle& circle) = 0;
		virtual void visit(const document::shapes::Text& text) = 0;
		virtual void visit(const document::shapes::Image& image) = 0;
	};
}
