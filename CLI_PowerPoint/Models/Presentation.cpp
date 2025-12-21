#include <algorithm>
#include "Presentation.h"
#include <stdexcept>

#include <iostream>

using namespace document;

void Presentation::addSlide(size_t pos, std::shared_ptr<Slide> slide)
{
    if (pos > m_slides.size()) {
        pos = m_slides.size();
    }

    std::cout << "Presentation index: " << pos << std::endl;
    m_slides.insert(m_slides.begin() + pos, slide);
}

void Presentation::removeSlide(size_t pos)
{
    if (pos >= m_slides.size()) {
        throw std::out_of_range("Invalid slide position");
    }
    m_slides.erase(m_slides.begin() + pos);
}

const Slide& Presentation::getSlide(size_t pos) const
{
    if (pos >= m_slides.size()) {
        throw std::out_of_range("Invalid slide index");
    }
    return *m_slides[pos];
}

Slide& Presentation::getSlide(size_t pos)
{
    if (pos >= m_slides.size())
        throw std::out_of_range("Invalid slide index.");

    return *m_slides[pos];
}


size_t Presentation::getSlideCount() const
{
    return m_slides.size();
}
