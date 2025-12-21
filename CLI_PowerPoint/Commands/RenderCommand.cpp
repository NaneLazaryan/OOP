#include "RenderCommand.h"
#include "../Core/vizualization/PainterFactory.h"
#include "../Core/vizualization/PresentationDrawer.h"
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>

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
		const document::Presentation& presentation = editor.getPresentation();
		size_t slideCount = presentation.getSlideCount();

		if (slideCount == 0) {
			std::cout << "No slides to render." << std::endl;
			return;
		}

		if (m_outputFile.empty()) {
			renderToConsole(presentation);
			return;
		}

		// Render each slide to a separate file
		for (size_t i = 0; i < slideCount; ++i) {
			renderSlideToFile(presentation, i);
		}

		std::cout << "Rendered " << slideCount << " slide(s) successfully." << std::endl;
	}

	void RenderCommand::renderToConsole(const document::Presentation& presentation)
	{
		auto painter = vizualization::PainterFactory::createPainter(m_format);
		if (!painter) {
			throw std::runtime_error("Failed to create painter for format: " + m_format);
		}

		vizualization::PresentationDrawer drawer(std::move(painter));
		drawer.draw(presentation);

		std::string output = drawer.getOutput();
		std::cout << output;
	}

	void RenderCommand::renderSlideToFile(const document::Presentation& presentation, size_t slideIndex)
	{
		auto painter = vizualization::PainterFactory::createPainter(m_format);
		if (!painter) {
			throw std::runtime_error("Failed to create painter for format: " + m_format);
		}

		vizualization::PresentationDrawer drawer(std::move(painter));
		const document::Slide& slide = presentation.getSlide(slideIndex);

		// Draw single slide
		drawer.drawSingleSlide(slide);

		std::string filename = generateFilename(slideIndex);

		std::string output = drawer.getOutput();
		std::ofstream file(filename);
		if (!file.is_open()) {
			throw std::runtime_error("Failed to open output file: " + filename);
		}

		file << output;
		file.close();

		std::cout << "Slide " << (slideIndex + 1) << " rendered to: " << filename << std::endl;
	}

	std::string RenderCommand::generateFilename(size_t slideIndex)
	{
		size_t dotPos = m_outputFile.find_last_of('.');
		std::string baseName;
		std::string extension;

		if (dotPos != std::string::npos) {
			baseName = m_outputFile.substr(0, dotPos);
			extension = m_outputFile.substr(dotPos);
		}
		else {
			baseName = m_outputFile;
			extension = "." + m_format;
		}

		std::ostringstream oss;
		oss << baseName << "_slide" << (slideIndex + 1) << extension;
		return oss.str();
	}
}


