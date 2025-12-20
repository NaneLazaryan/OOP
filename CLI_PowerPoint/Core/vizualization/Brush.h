#pragma once

#include "../../Models/utility/Color.h"

namespace vizualization
{
	class Brush
	{
	public:
		Brush() : m_color(document::shapes::utility::Color::White()) {}

		Brush(const document::shapes::utility::Color& color) : m_color(color) {}

		const document::shapes::utility::Color& getColor() const { return m_color; }
		void setColor(const document::shapes::utility::Color& color) { m_color = color; }

	private:
		document::shapes::utility::Color m_color;
	};
}