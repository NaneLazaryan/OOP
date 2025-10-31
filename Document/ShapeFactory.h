#pragma once

#include "Shape.h"
#include <memory>
#include <map>
#include <functional>

class ShapeFactory
{
public:
	// Singleton instance
	static ShapeFactory& instance()
	{
		static ShapeFactory factory;
		return factory;
	}

	ShapeFactory(const ShapeFactory&) = delete;
	ShapeFactory& operator=(const ShapeFactory&) = delete;

	static std::unique_ptr<IShape> createShape(IShape::ShapeType type);
	static std::unique_ptr<IShape> createShape(const std::string& typeName);

	void registerShape(const std::string& typeName, std::function<ShapePtr()> creator);

private:
	std::map<std::string, std::function<ShapePtr()>> creators;
private:
	ShapeFactory();

	static std::unique_ptr<IShape> createCircle(float radius, float x = 0.0f, float y = 0.0f);
	static std::unique_ptr<IShape> createRectangle(float width, float height, float x = 0.0f, float y = 0.0f);
	static std::unique_ptr<IShape> createText(const std::string& content, float fontSize = 12.0f, float x = 0.0f, float y = 0.0f);
	//static std::unique_ptr<IShape> createImage();
};