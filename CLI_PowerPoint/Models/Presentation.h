#pragma once
#include "Slide.h"
#include <vector>
#include <memory>
#include <string>


namespace document
{
	class Presentation
	{
	public:
		using SlideIterator = std::vector<std::shared_ptr<Slide>>::const_iterator;

		Presentation() = default;
		~Presentation() = default;

		void addSlide(size_t pos, std::shared_ptr<Slide> slide);
		void removeSlide(size_t pos);

		void clear() { m_slides.clear(); }

		Slide& getSlide(size_t pos);
		const Slide& getSlide(size_t pos) const;
		size_t getSlideCount() const;

		SlideIterator begin() const { return m_slides.begin(); }
		SlideIterator end() const { return m_slides.end(); }
	private:
		std::vector<std::shared_ptr<Slide>> m_slides;
	};
}
