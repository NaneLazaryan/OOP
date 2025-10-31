#pragma once
#include <iostream>
#include <memory>
#include <vector>

#include "BoundingBox.h"

// Interface
class IShape
{
public:
	enum class ShapeType
	{
		Circle,
		Rectangle,
		Text,
		Image
	};

	virtual ~IShape() = default;
	
	// Getters
	virtual ShapeType getType() const = 0;
	virtual const BoundingBox& getBoundingBox() const = 0;
	virtual const Point& getPosition() const = 0;

	virtual void draw() = 0;
	virtual void setPosition(float x, float y) = 0;
};

using ShapePtr = std::unique_ptr<IShape>;
using ShapeList = std::vector<ShapePtr>;


// ----------------------- Circle -----------------------

class Circle : public IShape
{
public:
	Circle(float r = 10.0f, float x = 0.0f, float y = 0.0f) 
		: radius(r), center(x, y)
	{
		updateBoundingBox();
	}

	~Circle() = default;

	ShapeType getType() const
	{
		return ShapeType::Circle;
	}

	const BoundingBox& getBoundingBox() const
	{
		return box;
	}

	const Point& getPosition() const
	{
		return center;
	}

	void draw();
	void setPosition(float x, float y);

	void setRadius(float r);

	float getRadius() const
	{
		return radius;
	}

private:
	float radius;
	Point center;
	BoundingBox box;
private:
	void updateBoundingBox();
};


// ----------------------- Rectangle -----------------------

class Rectangle : public IShape
{
public:
	// x, y for position(top-left corner)
	Rectangle(float w = 10.0f, float h = 10.0f, float x = 0.0f, float y = 0.0f)
		: width(w), height(h)
	{
		box = BoundingBox(Point(x, y), Point(x + w, y - h));
	}

	~Rectangle() = default;

	ShapeType getType() const
	{
		return ShapeType::Rectangle;
	}

	const BoundingBox& getBoundingBox() const
	{
		return box;
	}

	const Point& getPosition() const
	{
		return box.getTopLeft();
	}

	void draw();
	void setPosition(float x, float y);

	void setWidth(float w);
	void setHeight(float h);


	float getWidth() const
	{
		return width;
	}

	float getHeight() const
	{
		return height;
	}
private:
	float width;
	float height;
	BoundingBox box;
private:
	void updateBoundingBox();
};


// ----------------------- Text ----------------------- 

class Text : public IShape
{
public:
	Text(const std::string& content = "", float fontSize = 12.0f, float x = 0.0f, float y = 0.0f)
		: content(content), fontSize(fontSize)
	{
		box.setXTopLeft(x);
		box.setYTopLeft(y);
		updateBoundingBox();
	}

	~Text() = default;

	ShapeType getType() const
	{
		return ShapeType::Text;
	}

	const BoundingBox& getBoundingBox() const
	{
		return box;
	}

	const Point& getPosition() const
	{
		return box.getTopLeft();
	}

	void draw();
	void setPosition(float x, float y);

	void setContent(const std::string& text);
	void setFontSize(float size);

	const std::string& getContent() const
	{
		return content;
	}

	float getFontSize() const
	{
		return fontSize;
	}

private:
	std::string content;
	float fontSize;
	BoundingBox box;
private:
	void updateBoundingBox();
};