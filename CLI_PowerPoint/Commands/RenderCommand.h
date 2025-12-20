#pragma once

#include "Command.h"

namespace cli::cmd
{
	class RenderCommand : public ICommand
	{
	public:
		RenderCommand();
		RenderCommand(const std::string& format, const std::string& outputFile = "");
		~RenderCommand() = default;
		void execute(editing::Editor& editor);

	private:
		std::string m_format;
		std::string m_outputFile;
	};
}