#include "SVGPainter.h"
#include <sstream>

namespace vizualization
{
	void SVGPainter::drawLine(document::shapes::Line line)
	{
		const auto& geometry = line.getGeometry();
		const auto& start = geometry.getTopLeft();
		const auto& end = geometry.getBottomRight();
		const auto& color = line.getLineColor();

		std::ostringstream oss;
		oss << "<line x1=\"" << start.x << "\" y1=\"" << start.y
			<< "\" x2=\"" << end.x << "\" y2=\"" << end.y
			<< "\" stroke=\"rgb(" << static_cast<int>(color.getRed())
			<< "," << static_cast<int>(color.getGreen())
			<< "," << static_cast<int>(color.getBlue()) << ")\""
			<< " stroke-width=\"" << line.getThickness() << "\"";

		if (color.getAlpha() < 255) {
			oss << " stroke-opacity=\"" << (color.getAlpha() / 255.0) << "\"";
		}

		oss << " />\n";
		m_svgContent += oss.str();
	}

	void SVGPainter::drawRectangle(document::shapes::Rectangle rect)
	{
		const auto& geometry = rect.getGeometry();
		const auto& topLeft = geometry.getTopLeft();
		const auto& bottomRight = geometry.getBottomRight();

		int x = topLeft.x;
		int y = topLeft.y;
		int width = bottomRight.x - topLeft.x;
		int height = bottomRight.y - topLeft.y;

		std::ostringstream oss;

		// Draw fill
		if (rect.hasFillColor()) {
			const auto& fillColor = rect.getFillColor();
			oss << "<rect x=\"" << x << "\" y=\"" << y
				<< "\" width=\"" << width << "\" height=\"" << height
				<< "\" fill=\"rgb(" << static_cast<int>(fillColor.getRed())
				<< "," << static_cast<int>(fillColor.getGreen())
				<< "," << static_cast<int>(fillColor.getBlue()) << ")\"";

			if (fillColor.getAlpha() < 255) {
				oss << " fill-opacity=\"" << (fillColor.getAlpha() / 255.0) << "\"";
			}
			oss << " />\n";
		}

		// Draw border
		if (rect.hasBorder() && rect.getBorder().isVisible()) {
			const auto& border = rect.getBorder();
			const auto& borderColor = border.getColor();
			oss << "<rect x=\"" << x << "\" y=\"" << y
				<< "\" width=\"" << width << "\" height=\"" << height
				<< "\" fill=\"none\""
				<< " stroke=\"rgb(" << static_cast<int>(borderColor.getRed())
				<< "," << static_cast<int>(borderColor.getGreen())
				<< "," << static_cast<int>(borderColor.getBlue()) << ")\""
				<< " stroke-width=\"" << border.getThickness() << "\" />\n";
		}

		m_svgContent += oss.str();
	}

	void SVGPainter::drawCircle(document::shapes::Circle circle)
	{
		const auto& geometry = circle.getGeometry();
		const auto& topLeft = geometry.getTopLeft();
		const auto& bottomRight = geometry.getBottomRight();

		// Calculate center and radius
		int centerX = (topLeft.x + bottomRight.x) / 2;
		int centerY = (topLeft.y + bottomRight.y) / 2;
		int radiusX = std::abs(bottomRight.x - topLeft.x) / 2;
		int radiusY = std::abs(bottomRight.y - topLeft.y) / 2;
		int radius = std::max(radiusX, radiusY);

		std::ostringstream oss;

		// Draw fill
		if (circle.hasFillColor()) {
			const auto& fillColor = circle.getFillColor();
			oss << "<circle cx=\"" << centerX << "\" cy=\"" << centerY
				<< "\" r=\"" << radius
				<< "\" fill=\"rgb(" << static_cast<int>(fillColor.getRed())
				<< "," << static_cast<int>(fillColor.getGreen())
				<< "," << static_cast<int>(fillColor.getBlue()) << ")\"";

			if (fillColor.getAlpha() < 255) {
				oss << " fill-opacity=\"" << (fillColor.getAlpha() / 255.0) << "\"";
			}
			oss << " />\n";
		}

		// Draw border
		if (circle.hasBorder() && circle.getBorder().isVisible()) {
			const auto& border = circle.getBorder();
			const auto& borderColor = border.getColor();
			oss << "<circle cx=\"" << centerX << "\" cy=\"" << centerY
				<< "\" r=\"" << radius
				<< "\" fill=\"none\""
				<< " stroke=\"rgb(" << static_cast<int>(borderColor.getRed())
				<< "," << static_cast<int>(borderColor.getGreen())
				<< "," << static_cast<int>(borderColor.getBlue()) << ")\""
				<< " stroke-width=\"" << border.getThickness() << "\" />\n";
		}

		m_svgContent += oss.str();
	}

	void SVGPainter::drawText(document::shapes::Text text)
	{
		const auto& geometry = text.getGeometry();
		const auto& topLeft = geometry.getTopLeft();

		int x = topLeft.x;
		int y = topLeft.y + 20; 

		std::ostringstream oss;

		// Draw background 
		if (text.hasFillColor() && text.getFillColor().getAlpha() > 0) {
			const auto& bgColor = text.getFillColor();
			const auto& bottomRight = geometry.getBottomRight();
			int width = bottomRight.x - topLeft.x;
			int height = bottomRight.y - topLeft.y;

			oss << "<rect x=\"" << topLeft.x << "\" y=\"" << topLeft.y
				<< "\" width=\"" << width << "\" height=\"" << height
				<< "\" fill=\"rgb(" << static_cast<int>(bgColor.getRed())
				<< "," << static_cast<int>(bgColor.getGreen())
				<< "," << static_cast<int>(bgColor.getBlue()) << ")\"";

			if (bgColor.getAlpha() < 255) {
				oss << " fill-opacity=\"" << (bgColor.getAlpha() / 255.0) << "\"";
			}
			oss << " />\n";
		}

		// Draw text 
		const auto& textColor = text.getTextColor();
		oss << "<text x=\"" << x << "\" y=\"" << y
			<< "\" fill=\"rgb(" << static_cast<int>(textColor.getRed())
			<< "," << static_cast<int>(textColor.getGreen())
			<< "," << static_cast<int>(textColor.getBlue()) << ")\"";

		if (textColor.getAlpha() < 255) {
			oss << " fill-opacity=\"" << (textColor.getAlpha() / 255.0) << "\"";
		}

		oss << ">";

		// Escape XML special characters in text content
		std::string escapedText = text.getText();
		std::string result;
		for (char c : escapedText) {
			switch (c) {
			case '&': result += "&amp;"; break;
			case '<': result += "&lt;"; break;
			case '>': result += "&gt;"; break;
			case '"': result += "&quot;"; break;
			case '\'': result += "&apos;"; break;
			default: result += c; break;
			}
		}

		oss << result << "</text>\n";

		// Draw border
		if (text.hasBorder() && text.getBorder().isVisible()) {
			const auto& border = text.getBorder();
			const auto& borderColor = border.getColor();

			const auto& bottomRight = geometry.getBottomRight();
			int width = bottomRight.x - topLeft.x;
			int height = bottomRight.y - topLeft.y;

			oss << "<rect x=\"" << topLeft.x << "\" y=\"" << topLeft.y
				<< "\" width=\"" << width << "\" height=\"" << height
				<< "\" fill=\"none\""
				<< " stroke=\"rgb(" << static_cast<int>(borderColor.getRed())
				<< "," << static_cast<int>(borderColor.getGreen())
				<< "," << static_cast<int>(borderColor.getBlue()) << ")\""
				<< " stroke-width=\"" << border.getThickness() << "\" />\n";
		}

		m_svgContent += oss.str();
	}

	void SVGPainter::drawImage(document::shapes::Image image)
	{
		const auto& geometry = image.getGeometry();
		const auto& topLeft = geometry.getTopLeft();
		const auto& bottomRight = geometry.getBottomRight();

		int x = topLeft.x;
		int y = topLeft.y;
		int width = bottomRight.x - topLeft.x;
		int height = bottomRight.y - topLeft.y;

		std::ostringstream oss;

		// Draw image
		oss << "<image x=\"" << x << "\" y=\"" << y
			<< "\" width=\"" << width << "\" height=\"" << height
			<< "\" href=\"" << image.getImagePath() << "\" />\n";

		// Draw border
		if (image.hasBorder() && image.getBorder().isVisible()) {
			const auto& border = image.getBorder();
			const auto& borderColor = border.getColor();
			oss << "<rect x=\"" << x << "\" y=\"" << y
				<< "\" width=\"" << width << "\" height=\"" << height
				<< "\" fill=\"none\""
				<< " stroke=\"rgb(" << static_cast<int>(borderColor.getRed())
				<< "," << static_cast<int>(borderColor.getGreen())
				<< "," << static_cast<int>(borderColor.getBlue()) << ")\""
				<< " stroke-width=\"" << border.getThickness() << "\" />\n";
		}

		m_svgContent += oss.str();
	}

	void SVGPainter::beginSVG(int width, int height)
	{
		std::ostringstream oss;
		oss << "<svg width=\"" << width << "\" height=\"" << height
			<< "\" xmlns=\"http://www.w3.org/2000/svg\">\n";
		m_svgContent = oss.str();
	}

	void SVGPainter::endSVG()
	{
		m_svgContent += "</svg>\n";
	}
}