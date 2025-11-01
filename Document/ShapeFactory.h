#pragma once

#include "Shape.h"
#include <memory>
#include <map>
#include <functional>

namespace document
{
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

		static std::unique_ptr<shapes::IShape> createShape(shapes::IShape::ShapeType type);
		static std::unique_ptr<shapes::IShape> createShape(const std::string& typeName);

		void registerShape(const std::string& typeName, std::function<shapes::ShapePtr()> creator);

	private:
		std::map<std::string, std::function<shapes::ShapePtr()>> creators;
	private:
		ShapeFactory();

		static std::unique_ptr<shapes::IShape> createCircle(float radius, float x = 0.0f, float y = 0.0f);
		static std::unique_ptr<shapes::IShape> createRectangle(float width, float height, float x = 0.0f, float y = 0.0f);
		static std::unique_ptr<shapes::IShape> createText(const std::string& content, float fontSize = 12.0f, float x = 0.0f, float y = 0.0f);
		//static std::unique_ptr<IShape> createImage();
	};
}
