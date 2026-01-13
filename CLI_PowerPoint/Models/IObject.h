#pragma once
#include <string>
#include <memory>
#include "utility/Geometry.h"
#include "utility/Color.h"
#include "utility/Border.h"

namespace document::shapes { class IObjectVisitor; }

namespace document::shapes
{
    // Interface for all shapes (text, image, etc.)
    class IObject {
    public:
        enum class ObjectType
        {
            Circle,
            Rectangle,
            Line,
            Text,
            Image
        };

        virtual ~IObject() = default;

        // Basic properties
        virtual std::string getName() const = 0;
        virtual ObjectType getType() const = 0;
        virtual const utility::Geometry& getGeometry() const = 0;
        virtual void setGeometry(const utility::Geometry& geometry) = 0;

        // Style properties
        virtual bool hasFillColor() const { return false; }
        virtual const utility::Color& getFillColor() const 
        {
            static utility::Color empty;
            return empty;
        }
        virtual void setFillColor(const utility::Color&) {}

        virtual bool hasBorder() const { return false; }
        virtual const utility::Border& getBorder() const 
        {
            static utility::Border empty;
            return empty;
        }
        virtual void setBorder(const utility::Border&) {}

        // Visitor pattern - accept a visitor
        virtual void accept(IObjectVisitor& visitor) const = 0;

        // Cloning
        virtual std::unique_ptr<IObject> clone() const = 0;
    };
}

