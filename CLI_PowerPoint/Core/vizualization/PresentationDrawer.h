#pragma once

#include "IPainter.h"
#include "DrawingVisitor.h"
#include "../../Models/Presentation.h"
#include <memory>

namespace vizualization
{
	class PresentationDrawer
	{
	public:
		PresentationDrawer(std::unique_ptr<IPainter> painter);

		void draw(const document::Presentation& presentation);
		void drawSlide(const document::Slide& slide);
		void drawSingleSlide(const document::Slide& slide);

		// Get output (format-specific, e.g., SVG string)
		std::string getOutput() const;

	private:
		std::unique_ptr<IPainter> m_painter;
		DrawingVisitor m_visitor;
	};
}
