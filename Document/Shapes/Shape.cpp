#include "Shape.h"

using namespace document::shapes;

// ----------------------- Circle -----------------------
void Circle::draw()
{
	std::cout << "Drawing Circle at (" << center.x << ", " << center.y << ") with radius " << radius << std::endl;
}


void Circle::setPosition(float x, float y)
{
	center.x = x;
	center.y = y;
	updateBoundingBox();
}

void Circle::setRadius(float r)
{
	if (r <= 0) {
		throw std::invalid_argument("Radius must be positive");
	}
	radius = r;
	updateBoundingBox();
}

void Circle::updateBoundingBox()
{
	box.setXTopLeft(center.x - radius);
	box.setYTopLeft(center.y + radius);
	box.setXBottomRight(center.x + radius);
	box.setYBottomRight(center.y - radius);
}


// ----------------------- Rectangle -----------------------
void Rectangle::draw()
{
	Point topLeft = box.getTopLeft();
	std::cout << "Drawing Rectangle at (" << topLeft.x << ", " << topLeft.y	<< ") with width " << width << " and height " << height << std::endl;
}
void Rectangle::setPosition(float x, float y)
{
	box.setXTopLeft(x);
	box.setYTopLeft(y);
	updateBoundingBox();
}

void Rectangle::setWidth(float w)
{
	if (w <= 0) {
		throw std::invalid_argument("Width must be positive");
	}
	width = w;
	updateBoundingBox();
}

void Rectangle::setHeight(float h)
{
	if (h <= 0) {
		throw std::invalid_argument("Height must be positive");
	}
	height = h;
	updateBoundingBox();
}

void Rectangle::updateBoundingBox()
{
	Point topLeft = box.getTopLeft();
	box.setXBottomRight(topLeft.x + width);
	box.setYBottomRight(topLeft.y - height);
}

// ----------------------- Text ----------------------- 

void Text::draw()
{
	Point topLeft = box.getTopLeft();
	std::cout << "Drawing Text \"" << content << "\" at (" << topLeft.x << ", " << topLeft.y << ") with font size " << fontSize << std::endl;
}

void Text::setPosition(float x, float y)
{
	box.setXTopLeft(x);
	box.setYTopLeft(y);
	updateBoundingBox();
}

void Text::setContent(const std::string& text)
{
	content = text;
	updateBoundingBox();
}

void Text::setFontSize(float size)
{
	if (size <= 0) {
		throw std::invalid_argument("Font size must be positive");
	}
	fontSize = size;
	updateBoundingBox();
}

void Text::updateBoundingBox()
{
	// Approximate bounding box based on content length and font size
	Point topLeft = box.getTopLeft();
	float boxWidth = content.length() * fontSize * 0.6f;
	float boxHeight = fontSize * 1.2f;
	
	box.setXBottomRight(topLeft.x + boxWidth);
	box.setYBottomRight(topLeft.y - boxHeight);
}