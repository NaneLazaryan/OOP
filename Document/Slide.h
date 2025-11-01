#pragma once

#include "Shape.h"

namespace document
{
	class Slide
	{
	public:
		Slide() : m_id(0) {}
		explicit Slide(int id) : m_id(id) {}

		~Slide() = default;

		void addShape(shapes::ShapePtr);
		void removeShape(size_t);

		shapes::IShape* getShape(size_t);
		const shapes::IShape* getShape(size_t) const;

		int getId() const;
		void setId(int id);

		size_t getShapeCount() const;
		const shapes::ShapeList& getShapes() const;

	private:
		int m_id;
		shapes::ShapeList objects;
	};

	using SlidePtr = std::shared_ptr<Slide>;
	using SlideList = std::vector<SlidePtr>;
}
