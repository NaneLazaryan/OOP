#include "Slide.h"
#include <sstream>

using namespace document;

void Slide::addShape(std::unique_ptr<IObject> shape) 
{
    m_objects.push_back(std::move(shape));
}

void Slide::removeShape(size_t index)
{
    if (index >= m_objects.size()) {
        throw std::out_of_range("Invalid shape index");
    }
    m_objects.erase(m_objects.begin() + index);
}

const IObject& Slide::getShape(size_t pos) const
{
    if (pos >= m_objects.size()) {
        throw std::out_of_range("Shape index out of range");
    }
    return *m_objects[pos];
}


IObject& Slide::getShape(size_t pos)
{
    if (pos >= m_objects.size()) {
        throw std::out_of_range("Shape index out of range");
    }
    return *m_objects[pos];
}

bool Slide::moveShape(int x, int y)
{
    // TODO: Implement
    return false;
}

bool Slide::resizeShape(int width, int height)
{
    // TODO: Implement
    return false;
}
