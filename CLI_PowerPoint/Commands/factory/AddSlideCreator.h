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

			size_t index = -1;

			/*auto it1 = args.find("$0");
			if (it1 != args.end()) {
				index = std::get<int>(it1->second);
			}*/

			auto it = args.find("-at");
			if ( it != args.end()) {
				index = std::get<int>(it->second);
			}

			return std::make_unique<AddSlideCommand>(index);
		}
	};
}