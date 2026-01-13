#pragma once

#include "ISerializer.h"
#include "../Models/Slide.h"
#include "../Models/utility/Geometry.h"

using namespace document;

namespace serialization
{
	class JsonSerializer : public ISerializer
	{
	public:
		bool saveToFile(const document::Presentation& pres, const std::string& filename);
		std::string serialize(const document::Presentation& pres);
	private:
		static std::string serializeSlide(const Slide& slide, size_t indent = 2);
		static std::string serializeShape(const shapes::IObject & shape, size_t indent = 4);
		//static std::string serializeGeometry(const shapes::utility::Geometry& box);
	};
}