#include "Presentation.h"
#include <stdexcept>

void Presentation::addSlide(size_t pos, SlidePtr newSlide)
{
	slides.insert(slides.begin() + pos, std::move(newSlide));
}

void Presentation::removeSlide(size_t pos)
{
	slides.erase(slides.begin() + pos);
}

SlidePtr Presentation::getSlide(size_t pos) const
{
	if (pos >= slides.size())
		throw std::out_of_range("Invalid slide index.");

	return slides[pos];
}

const SlideList& Presentation::getSlides() const
{
	/*if (slides.empty())
		throw std::out_of_range("Presentation has no slides.");*/
	return slides;
}

size_t Presentation::getSlideCount() const
{
	return slides.size();
}
