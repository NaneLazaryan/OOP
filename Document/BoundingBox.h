#pragma once

struct Point
{
	float x;
	float y;

	Point() = default;
	Point(float x, float y) : x(x), y(y) {}
};

class BoundingBox
{
public:
	BoundingBox() : topLeft({ 0.f, 0.f }), bottomRight({ 0.f, 0.f }) {}
	BoundingBox(Point coord1, Point coord2) 
		: topLeft(coord1), bottomRight(coord2) {}

	Point getTopLeft() const;
	Point getBottomRight() const;

	void setXTopLeft(float);
	void setYTOPLeft(float);

	void setXBottomRight(float);
	void setYBottomRight(float);
private:
	Point topLeft;
	Point bottomRight;
};
