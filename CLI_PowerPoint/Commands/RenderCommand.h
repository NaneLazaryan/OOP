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

	private:
		void renderToConsole(const document::Presentation& presentation);
		void renderSlideToFile(const document::Presentation& presentation, size_t slideIndex);
		std::string generateFilename(size_t slideIndex);
	};
}