#include "RenderCommand.h"
#include "../Core/vizualization/PainterFactory.h"
#include "../Core/vizualization/PresentationDrawer.h"
#include <stdexcept>
#include <iostream>
#include <fstream>

namespace cli::cmd
{
	RenderCommand::RenderCommand(const std::string& format, const std::string& outputFile)
		: m_format(format.empty() ? "svg" : format), m_outputFile(outputFile)
	{}

	RenderCommand::RenderCommand()
		: m_format("svg"), m_outputFile("") 
	{}

	void RenderCommand::execute(editing::Editor& editor)
	{
		auto painter = vizualization::PainterFactory::createPainter(m_format);
		if (!painter) {
			throw std::runtime_error("Failed to create painter for format: " + m_format);
		}

		vizualization::PresentationDrawer drawer(std::move(painter));
		const document::Presentation& presentation = editor.getPresentation();

		// Draw the presentation
		drawer.draw(presentation);

		// Get output
		std::string output = drawer.getOutput();

		// Output to file or console
		if (!m_outputFile.empty()) {
			std::ofstream file(m_outputFile);
			if (!file.is_open()) {
				throw std::runtime_error("Failed to open output file: " + m_outputFile);
			}
			file << output;
			file.close();
			std::cout << "Presentation rendered to: " << m_outputFile << std::endl;
		}
		else {
			std::cout << output;
		}
	}
}