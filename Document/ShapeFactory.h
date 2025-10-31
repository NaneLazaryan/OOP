#pragma once

#include "Shape.h"
#include <memory>
#include <map>
#include <functional>

using ShapeUPtr = std::unique_ptr<IShape>;

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

	void registerShape(const std::string& typeName, std::function<ShapeUPtr()> creator);

private:
	std::map<std::string, std::function<ShapeUPtr()>> creators;
private:
	ShapeFactory();

	static std::unique_ptr<IShape> createCircle(float radius, float x = 0.0f, float y = 0.0f);
	static std::unique_ptr<IShape> createRectangle(float width, float height, float x = 0.0f, float y = 0.0f);
	static std::unique_ptr<IShape> createText(const std::string& content, float fontSize = 12.0f, float x = 0.0f, float y = 0.0f);
	//static std::unique_ptr<IShape> createImage();
};