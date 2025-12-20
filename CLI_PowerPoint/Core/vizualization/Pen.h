#pragma once

#include "../../Models/utility/Color.h"

namespace vizualization
{
	class Pen
	{
	public:
		Pen()
			: m_color(document::shapes::utility::Color::Black()),
			m_thickness(1.0) {}

		Pen(const document::shapes::utility::Color& color, double thickness = 1.0)
			: m_color(color), m_thickness(thickness) {}

		const document::shapes::utility::Color& getColor() const { return m_color; }
		double getThickness() const { return m_thickness; }

		void setColor(const document::shapes::utility::Color& color) { m_color = color; }
		void setThickness(double thickness) { m_thickness = thickness; }

	private:
		document::shapes::utility::Color m_color;
		double m_thickness;
	};
}