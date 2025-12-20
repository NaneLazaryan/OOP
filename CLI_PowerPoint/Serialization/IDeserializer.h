#pragma once

#include "../Models/Presentation.h"

namespace serialization
{
	class IDeserializer
	{
	public:
		virtual ~IDeserializer() = default;

		virtual bool loadFromFile(document::Presentation& pres, const std::string& filename) = 0;
	};
}