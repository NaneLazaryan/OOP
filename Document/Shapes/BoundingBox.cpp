#include "BoundingBox.h"

Point BoundingBox::getTopLeft() const
{
	return topLeft;
}

Point BoundingBox::getBottomRight() const
{
	return bottomRight;
}

void BoundingBox::setXTopLeft(float x)
{
	topLeft.x = x;
}

void BoundingBox::setYTopLeft(float y)
{
	topLeft.y = y;
}

void BoundingBox::setXBottomRight(float x)
{
	bottomRight.x = x;
}

void BoundingBox::setYBottomRight(float y)
{
	bottomRight.y = y;
}