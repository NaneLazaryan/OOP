#pragma once

#include "ICmdCreator.h"
#include "../RenderCommand.h"
#include "../../Core/Parser.h"

namespace cli::cmd::factory
{
	class RenderCreator : public ICmdCreator
	{
	public:
		CommandPtr createCommand(const ArgMap& args)
		{
			std::string format = "svg";
			std::string outFile = "";

			// Try to get format from -format flag
			auto itFormat = args.find("-format");
			if (itFormat != args.end()) {
				format = std::get<std::string>(itFormat->second);
			}

			// Try to get output file from -file flag
			auto itFile = args.find("-file");
			if (itFile != args.end()) {
				outFile = std::get<std::string>(itFile->second);
			}
			else {
				// Try first positional argument
				auto it0 = args.find("$0");
				if (it0 != args.end()) {
					outFile = std::get<std::string>(it0->second);
				}
			}

			return std::make_unique<RenderCommand>(format, outFile);
		}
	};
}