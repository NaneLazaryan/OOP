#pragma once

#include "../../Models/utility/Geometry.h"
#include "../../Models/objects/Rectangle.h"
#include "../../Models/objects/Circle.h"
#include "../../Models/objects/Text.h"
#include "../../Models/objects/Image.h"

namespace vizualization
{
	class IPainter
	{
	public:
		virtual ~IPainter() = default;

		// Drawing
		virtual void drawLine(document::shapes::utility::Point a, document::shapes::utility::Point b) = 0;
		virtual void drawRectangle(document::shapes::Rectangle rect) = 0;
		virtual void drawCircle(document::shapes::Circle circle) = 0;
		virtual void drawText(document::shapes::Text text) = 0;
		virtual void drawImage(document::shapes::Image image) = 0;
		//virtual void drawEllipse() = 0;
	};

}
