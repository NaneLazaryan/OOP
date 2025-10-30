#pragma once
#include <iostream>
#include <memory>
#include <vector>

#include "BoundingBox.h"

class Shape
{
public:
	enum class ShapeType
	{
		Circle,
		Rectangle,
		Text,
		Image
	};

	Shape(ShapeType type) : type(type) {}
	virtual ~Shape() = default;
	
	// Getters
	virtual ShapeType getType() const = 0;
	virtual const BoundingBox& getBoundingBox() const = 0;
	virtual const Point& getPosition() const = 0;

	virtual void draw() = 0;
	virtual void setPosition(float x, float y) = 0;

protected:
	ShapeType type;
	BoundingBox box;
};

using ShapePtr = std::shared_ptr<Shape>;
using ShapeList = std::vector<ShapePtr>;


// ----------------------- Circle -----------------------

class Circle : public Shape
{
public:
	Circle(int r = 10.0f, float x = 0.0f, float y = 0.0f) 
		: Shape(ShapeType::Circle), radius(r) 
	{
		updateBoundingBox();
	}

	~Circle() = default;

	ShapeType getType() const
	{
		return type;
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
private:
	void updateBoundingBox();
};


// ----------------------- Rectangle -----------------------

class Rectangle : public Shape
{
public:
	// x, y for position(top-left corner)
	Rectangle(float w = 10.0f, float h = 10.0f, float x = 0.0f, float y = 0.0f)
		:Shape(ShapeType::Rectangle), width(w), height(h)
	{
		box = BoundingBox(Point(x, y), Point(x + w, y - h));
	}

	~Rectangle() = default;

	ShapeType getType() const
	{
		return type;
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
private:
	void updateBoundingBox();
};


// ----------------------- Text ----------------------- 

class Text : public Shape
{
public:
	Text(const std::string& content = "", float fontSize = 12.0f, float x = 0.0f, float y = 0.0f)
		: Shape(ShapeType::Text), content(content), fontSize(fontSize)
	{
		box.setXTopLeft(x);
		box.setXTopLeft(y);
		updateBoundingBox();
	}

	~Text() = default;

	ShapeType getType() const
	{
		return type;
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
private:
	void updateBoundingBox();
};