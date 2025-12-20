#pragma once

#include "ICmdCreator.h"
#include "../RemoveSlideCommand.h"

namespace cli::cmd::factory
{
	class RemoveSlideCreator : public ICmdCreator
	{
	public:
		CommandPtr createCommand(const ArgMap& args)
		{

            size_t pos = 0;

            // Try to get position from -at flag
            auto it = args.find("-at");
            if (it != args.end()) {
                pos = std::get<int>(it->second);
            }

            //else {
            //    // Try to get from first positional argument
            //    auto it0 = args.find("$0");
            //    if (it0 != args.end()) {
            //        pos = std::get<int>(it0->second);
            //    }
            //}

            return std::make_unique<RemoveSlideCommand>(pos);
		}
	};
}