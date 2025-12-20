#pragma once

#include "ICmdCreator.h"
#include "../UndoCommand.h"

namespace cli::cmd::factory
{
	class UndoCreator : public ICmdCreator
	{
	public:
		CommandPtr createCommand(const ArgMap& args)
		{
			return std::make_unique<UndoCommand>();
		}
	};
}