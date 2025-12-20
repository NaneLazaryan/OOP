#pragma once

namespace document::shapes::utility
{
	struct Point
	{
		int x;
		int y;

		Point() = default;
		Point(int x, int y) : x(x), y(y) {}
	};

	class Geometry
	{
	public:
		Geometry() : topLeft(0,0), bottomRight(0,0) {}
		Geometry(Point coord1, Point coord2)
			: topLeft(coord1), bottomRight(coord2) {}

		// Getters
		Point getTopLeft() const { return topLeft; }
		Point getBottomRight() const { return bottomRight; }
		int getWidth() { return bottomRight.x - topLeft.x; }
		int getHeight() const { return bottomRight.y - topLeft.y; }

		// Setters
		void setTopLeft(int x, int y)
		{
			topLeft.x = x;
			topLeft.y = y;
		}

		void setBottomRight(int x, int y)
		{
			bottomRight.x = x;
			bottomRight.y = y;
		}

	private:
		Point topLeft;
		Point bottomRight;
	};

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

