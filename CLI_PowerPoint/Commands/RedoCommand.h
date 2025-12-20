#pragma once

#include "Command.h"

namespace cli::cmd
{
	class RedoCommand : public ICommand
	{
	public:
		RedoCommand() = default;
		~RedoCommand() = default;
		void execute(editing::Editor& editor)
		{
			editor.redo();
		}
	};
}