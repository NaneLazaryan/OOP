#pragma once

#include "IObject.h"

namespace document::shapes
{
	class Object : public IObject
	{
    public:
        Object(ObjectType type, const utility::Geometry& geometry)
            : m_type(type), m_geometry(geometry)
        {}

        ObjectType getType() const { return m_type; }
        const utility::Geometry& getGeometry() const { return m_geometry; }
        void setGeometry(const utility::Geometry& geometry) { m_geometry = geometry; }

    protected:
        ObjectType m_type;
        utility::Geometry m_geometry;
	};
}