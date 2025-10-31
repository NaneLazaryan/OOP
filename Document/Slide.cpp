#include "Slide.h"

void Slide::addShape(ShapePtr shape)
{
	objects.push_back(std::move(shape));
}

void Slide::removeShape(size_t pos)
{
	objects.erase(objects.begin() + pos);
}

IShape* Slide::getShape(size_t pos)
{
	return objects[pos].get();
}

const IShape* Slide::getShape(size_t pos) const
{
	return objects[pos].get();
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