#pragma once

#include "../Models/Presentation.h"

namespace serialization
{
	class ISerializer
	{
	public:
		virtual ~ISerializer() = default;

		virtual bool saveToFile(const document::Presentation& pres, const std::string& filename) = 0;
	};
}