#pragma once

#include "IPainter.h"
#include <string>

namespace vizualization
{
	class SVGPainter : public IPainter
	{
	public:

		void drawLine(document::shapes::Line line);
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
		std::string m_svgContent;
	};
}