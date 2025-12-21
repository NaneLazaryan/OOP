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
				int temp = std::get<int>(it->second);
				if (temp < 0) {
					pos = 0;
				}
				else {
					pos = static_cast<size_t>(temp);
				}
			}

            return std::make_unique<RemoveSlideCommand>(pos);
		}
	};
}