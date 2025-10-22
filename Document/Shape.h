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
	
	virtual void draw() = 0;

protected:
	ShapeType type;
	BoundingBox box;
};

using ShapePtr = std::shared_ptr<Shape>;
using ShapeList = std::vector<ShapePtr>;

class Circle : public Shape
{
public:
	Circle(int r) : Shape(ShapeType::Circle), radius(r) {}
	~Circle() = default;

	void draw()
	{
		std::cout << "Circle" << std::endl;
	}
private:
	int radius;
};

class Rectangle : public Shape
{
public:
	~Rectangle() = default;

	void draw()
	{
		std::cout << "Square" << std::endl;

	}
private:

};