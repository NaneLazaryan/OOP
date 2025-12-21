#include "DrawingVisitor.h"

namespace vizualization
{
	DrawingVisitor::DrawingVisitor(IPainter& painter)
		: m_painter(painter)
	{}

	void DrawingVisitor::visit(const document::shapes::Rectangle& rect)
	{
		m_painter.drawRectangle(rect);
	}

	void DrawingVisitor::visit(const document::shapes::Circle& circle)
	{
		m_painter.drawCircle(circle);
	}

	void DrawingVisitor::visit(const document::shapes::Text& text)
	{
		m_painter.drawText(text);
	}

	void DrawingVisitor::visit(const document::shapes::Image& image)
	{
		m_painter.drawImage(image);
	}

	void DrawingVisitor::visit(const document::shapes::Line& line)
	{
		m_painter.drawLine(line);
	}
}
