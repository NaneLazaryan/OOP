#include "SerializerFactory.h"
#include "JsonSerializer.h"
#include "JsonDeserializer.h"
#include <algorithm>
#include <stdexcept>

using namespace serialization;

std::string SerializerFactory::normalizeExtension(const std::string& ext)
{
	std::string normalized = ext;

	size_t dotPos = normalized.find_last_of('.');
	if (dotPos != std::string::npos) {
		normalized = normalized.substr(dotPos);
	}

	if (!normalized.empty() && normalized[0] != '.') {
		normalized = "." + normalized;
	}

	std::transform(normalized.begin(), normalized.end(), normalized.begin(),
		[](unsigned char c) { return std::tolower(c); });

	return normalized;
}

std::unique_ptr<ISerializer> SerializerFactory::createSerializer(const std::string& extension)
{
	std::string ext = normalizeExtension(extension);

	if (ext == ".json") {
		return std::make_unique<JsonSerializer>();
	}

	// Other serializers

	throw std::invalid_argument("Unsupported file format: " + ext);
}

std::unique_ptr<IDeserializer> SerializerFactory::createDeserializer(const std::string& extension)
{
	std::string ext = normalizeExtension(extension);

	if (ext == ".json") {
		return std::make_unique<JsonDeserializer>();
	}

	// Other deserializers

	throw std::invalid_argument("Unsupported file format: " + ext);
}