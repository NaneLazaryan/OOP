#pragma once

#include "Slide.h"
#include <vector>

class Presentation
{
public:
	Presentation() = default;
	~Presentation() = default;

	void addSlide(size_t, SlidePtr);
	void removeSlide(size_t);

	SlidePtr getSlide(size_t) const;
	const SlideList& getSlides() const;

	size_t getSlideCount() const;

private:
	SlideList slides;
};