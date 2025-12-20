#include "PresentationDrawer.h"
#include "SVGPainter.h"
#include "../../Models/Slide.h"
#include <stdexcept>

namespace vizualization
{
	PresentationDrawer::PresentationDrawer(std::unique_ptr<IPainter> painter)
		: m_painter(std::move(painter)), m_visitor(*m_painter)
	{
		if (!m_painter) {
			throw std::invalid_argument("Painter cannot be null");
		}
	}

	void PresentationDrawer::draw(const document::Presentation& presentation)
	{
		// For SVG
		auto* svgPainter = dynamic_cast<SVGPainter*>(m_painter.get());
		if (svgPainter) {
			// Default canvas size
			svgPainter->beginSVG(1920, 1080);
		}

		// Draw each slide
		for (auto it = presentation.begin(); it != presentation.end(); ++it) {
			drawSlide(**it);
		}

		if (svgPainter) {
			svgPainter->endSVG();
		}
	}

	void PresentationDrawer::drawSlide(const document::Slide& slide)
	{
		// Draw all objects on the slide using visitor pattern
		for (size_t i = 0; i < slide.getShapeCount(); ++i) {
			const auto& object = slide.getShape(i);
			object.accept(m_visitor);
		}
	}

	std::string PresentationDrawer::getOutput() const
	{
		auto* svgPainter = dynamic_cast<SVGPainter*>(m_painter.get());
		if (svgPainter) {
			return svgPainter->getSVG();
		}
		return "";
	}
}
