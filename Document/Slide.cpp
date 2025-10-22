#include "Slide.h"

void Slide::addShape(ShapePtr shape)
{
	objects.push_back(shape);
}

void Slide::removeShape(size_t pos)
{
	objects.erase(objects.begin() + pos);
}

ShapePtr Slide::getShape(size_t pos)
{
	return objects[pos];
}

const ShapePtr Slide::getShape(size_t pos) const
{
	return objects[pos];
}

int Slide::getId() const
{
	return m_id;
}

void Slide::setId(int id)
{
	m_id = id;
}

size_t Slide::getShapeCount() const
{
	return objects.size();
}

const ShapeList& Slide::getShapes() const
{
	return objects;
}