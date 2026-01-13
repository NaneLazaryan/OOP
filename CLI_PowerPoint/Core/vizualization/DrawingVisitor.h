#pragma once

#include "../../Models/IObjectVisitor.h"
#include "IPainter.h"
#include "../../Models/objects/Rectangle.h"
#include "../../Models/objects/Circle.h"
#include "../../Models/objects/Text.h"
#include "../../Models/objects/Image.h"
#include "../../Models/objects/Line.h"

namespace vizualization
{
	// Visitor that handles drawing operations
	class DrawingVisitor : public document::shapes::IObjectVisitor
	{
	public:
		explicit DrawingVisitor(IPainter& painter);

		// IVisitor interface
		void visit(const document::shapes::Rectangle& rect);
		void visit(const document::shapes::Circle& circle);
		void visit(const document::shapes::Text& text);
		void visit(const document::shapes::Image& image);
		void visit(const document::shapes::Line& line);

	private:
		IPainter& m_painter;
	};
}
