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

bool Slide::moveShape(const std::string& name, int x, int y)
{
    for (auto& obj : m_objects) {
        if (obj->getName() == name) {
            utility::Geometry geom = shapes::utility::move(obj->getGeometry(), x, y);

            obj->setGeometry(geom);
            return true;
        }
    }
    return false; // shape not found
}

bool Slide::resizeShape(const std::string& name, int width, int height)
{
    if (width <= 0 || height <= 0) {
        return false;
    }

    for (auto& obj : m_objects) {
        if (obj->getName() == name) {
            utility::Geometry geom = shapes::utility::resize(obj->getGeometry(), width, height);

            obj->setGeometry(geom);
            return true;
        }
    }
    return false; // shape not found
}
