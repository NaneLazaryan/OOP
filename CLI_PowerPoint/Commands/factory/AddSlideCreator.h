#pragma once

#include "ICmdCreator.h"
#include "../AddSlideCommand.h"

namespace cli::cmd::factory
{
	class AddSlideCreator : public ICmdCreator
	{
	public:
		CommandPtr createCommand(const ArgMap& args)
		{

			size_t index = 0;

			auto it = args.find("-at");
			if (it != args.end()) {
				int temp = std::get<int>(it->second);
				if (temp < 0) {
					index = 0;
				}
				else {
					index = static_cast<size_t>(temp);
				}
			}


			return std::make_unique<AddSlideCommand>(index);
		}
	};
}