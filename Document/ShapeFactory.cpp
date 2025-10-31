#include "ShapeFactory.h"

ShapeFactory::ShapeFactory()
{
	registerShape("Circle", []() { return std::make_unique<Circle>(); });
	registerShape("Rectangle", []() { return std::make_unique<Rectangle>(); });
	registerShape("Text", []() { return std::make_unique<Text>(); });
}

void ShapeFactory::registerShape(const std::string& typeName, std::function<ShapePtr()> creator)
{
	creators[typeName] = creator;
}

std::unique_ptr<IShape> ShapeFactory::createShape(IShape::ShapeType type)
{
	// Create with default arguments
	switch (type)
	{
	case IShape::ShapeType::Circle:
		return std::make_unique<Circle>();
	case IShape::ShapeType::Rectangle:
		return std::make_unique<Rectangle>();
	case IShape::ShapeType::Text:
		return std::make_unique<Text>();
	default:
		throw std::invalid_argument("Unknown shape type");
	}
}

std::unique_ptr<IShape> ShapeFactory::createShape(const std::string& typeName)
{
	auto& creators = instance().creators;
	auto it = creators.find(typeName);
	if (it != creators.end())
	{
		return it->second();
	}
	throw std::invalid_argument("Unknown shape type: " + typeName);
}

// Create with some arguments
std::unique_ptr<IShape> ShapeFactory::createCircle(float radius, float x, float y)
{
	return std::make_unique<Circle>(radius, x, y);
}

std::unique_ptr<IShape> ShapeFactory::createRectangle(float width, float height, float x, float y)
{
	return std::make_unique<Rectangle>(width, height, x, y);
}

std::unique_ptr<IShape> ShapeFactory::createText(const std::string& content, float fontSize, float x, float y)
{
	return std::make_unique<Text>(content, fontSize, x, y);
}

//std::unique_ptr<IShape> ShapeFactory::createImage()
//{
//
//}