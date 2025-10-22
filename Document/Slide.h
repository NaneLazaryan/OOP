#pragma once

#include "Shape.h"

class Slide
{
public:
	Slide() = default;
	explicit Slide(int id) : m_id(id) {}

	~Slide() = default;

	void addShape(ShapePtr);
	void removeShape(size_t);

	ShapePtr getShape(size_t);
	const ShapePtr getShape(size_t) const;

	int getId() const;
	void setId(int id);

	size_t getShapeCount() const;
	const ShapeList& getShapes() const;

private:
	int m_id;
	ShapeList objects;
};

using SlidePtr = std::shared_ptr<Slide>;
using SlideList = std::vector<SlidePtr>;