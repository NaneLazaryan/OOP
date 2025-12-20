#include "SaveCommand.h"
#include "../Serialization/SerializerFactory.h"
#include <stdexcept>
#include <iostream>

using namespace cli::cmd;

SaveCommand::SaveCommand(const std::string& filename)
    : m_filename(filename)
{}

void SaveCommand::execute(editing::Editor& editor)
{
    // Extract file extension
    size_t dotPos = m_filename.find_last_of('.');
    if (dotPos == std::string::npos) {
        throw std::invalid_argument("File must have an extension (e.g., .json)");
    }

    std::string extension = m_filename.substr(dotPos);

    auto serializer = serialization::SerializerFactory::createSerializer(extension);
    const document::Presentation& presentation = editor.getPresentation();

    // Save the presentation
    bool success = serializer->saveToFile(presentation, m_filename);

    if (!success) {
        throw std::runtime_error("Failed to save presentation to file: " + m_filename);
    }

    std::cout << "Presentation saved to: " << m_filename << std::endl;
}