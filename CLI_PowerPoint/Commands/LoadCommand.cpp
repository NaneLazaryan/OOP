#include "LoadCommand.h"
#include "../Serialization/SerializerFactory.h"
#include <stdexcept>
#include <iostream>

using namespace cli::cmd;

LoadCommand::LoadCommand(const std::string& filename)
    : m_filename(filename)
{
}

void LoadCommand::execute(editing::Editor& editor)
{
    // Extract file extension
    size_t dotPos = m_filename.find_last_of('.');
    if (dotPos == std::string::npos) {
        throw std::invalid_argument("File must have an extension (e.g., .json)");
    }

    std::string extension = m_filename.substr(dotPos);

    auto deserializer = serialization::SerializerFactory::createDeserializer(extension);
    document::Presentation& presentation = editor.getPresentation();

    // Clear existing presentation 
    presentation.clear();

    // Load the presentation
    bool success = deserializer->loadFromFile(presentation, m_filename);

    if (!success) {
        throw std::runtime_error("Failed to load presentation from file: " + m_filename);
    }

    std::cout << "Presentation loaded from: " << m_filename << std::endl;
}