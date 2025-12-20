#pragma once

#include "IDeserializer.h"
#include "../Models/Slide.h"
#include <memory>
#include <stdexcept>

using namespace document;
using namespace document::shapes;
using namespace document::shapes::utility;

namespace serialization
{
	class JsonDeserializer : public IDeserializer
	{
	public:
		bool loadFromFile(document::Presentation& pres, const std::string& filename);
		bool deserialize(Presentation& pres, const std::string& jsonStr);
	private:
		static std::shared_ptr<Slide> deserializeSlide(const std::string& jsonStr, size_t& pos);
		static std::unique_ptr<IObject> deserializeShape(const std::string& jsonStr, size_t& pos);
	};
}

