#pragma once

#include "IPainter.h"
#include "Pen.h"
#include "Brush.h"
#include <string>

namespace vizualization
{
	class SVGPainter : public IPainter
	{
	public:
		void setPen(const Pen& pen);
		void setBrush(const Brush& brush);

		void drawLine(document::shapes::utility::Point a, document::shapes::utility::Point b);
		void drawRectangle(document::shapes::Rectangle rect);
		void drawCircle(document::shapes::Circle circle);
		void drawText(document::shapes::Text text);
		void drawImage(document::shapes::Image image);

		// SVG-specific methods
		std::string getSVG() const { return m_svgContent; }
		void clear() { m_svgContent.clear(); }
		void beginSVG(int width, int height);
		void endSVG();

	private:
		Pen m_pen;
		Brush m_brush;
		std::string m_svgContent;
	};
}