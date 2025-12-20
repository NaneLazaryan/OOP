#include "Geometry.h"

namespace document::shapes::utility
{
	Geometry move(const Geometry& geometry, int x, int y)
	{
		Point p1 = geometry.getBottomRight();
		Point p2 = geometry.getTopLeft();

		p1.x += x;
		p2.x += x;
		p1.y += y;
		p2.y += y;

		return Geometry(p1, p2);
	}

	Geometry resize(const Geometry& geometry, int width, int height)
	{
		Point p1 = geometry.getTopLeft();
		Point p2(p1.x + width, p1.y - height);

		return Geometry(p1, p2);
	}
}