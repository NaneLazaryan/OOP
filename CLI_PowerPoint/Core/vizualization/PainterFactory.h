#pragma once

#include "IPainter.h"
#include "SVGPainter.h"
#include <memory>
#include <string>
#include <algorithm>

namespace vizualization
{
	class PainterFactory
	{
	public:
		static std::unique_ptr<IPainter> createPainter(const std::string& format)
		{
			std::string lowerFormat = format;
			std::transform(lowerFormat.begin(), lowerFormat.end(), lowerFormat.begin(),
				[](unsigned char c) { return std::tolower(c); });

			if (!lowerFormat.empty() && lowerFormat[0] == '.') {
				lowerFormat = lowerFormat.substr(1);
			}

			if (lowerFormat == "svg") {
				return std::make_unique<SVGPainter>();
			}

			// Default to SVG if format is not recognized
			return std::make_unique<SVGPainter>();
		}
	};
}