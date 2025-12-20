#pragma once

#include <memory>
#include "ISerializer.h"
#include "IDeserializer.h"

namespace serialization
{
    class SerializerFactory
    {
    public:
        // Get serializer based on file extension
        static std::unique_ptr<ISerializer> createSerializer(const std::string& extension);

        // Get deserializer based on file extension
        static std::unique_ptr<IDeserializer> createDeserializer(const std::string& extension);

    private:
        static std::string normalizeExtension(const std::string& ext);
    };
}