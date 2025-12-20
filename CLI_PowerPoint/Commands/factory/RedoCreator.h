#pragma once

#include "ICmdCreator.h"
#include "../RedoCommand.h"

namespace cli::cmd::factory
{
	class RedoCreator : public ICmdCreator
	{
	public:
		CommandPtr createCommand(const ArgMap& args)
		{
			return std::make_unique<RedoCommand>();
		}
	};
}