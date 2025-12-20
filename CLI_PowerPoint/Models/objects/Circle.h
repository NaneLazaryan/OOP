#pragma once
#include "../Object.h"

namespace document::shapes
{
    class Circle : public Object
    {
    public:
        Circle(const utility::Geometry& geometry);
        Circle(const utility::Geometry& geometry, const utility::Color& fillColor, const utility::Border& border);

        std::string getName() const;
        std::unique_ptr<IObject> clone() const;
        void accept(vizualization::IVisitor& visitor) const;

        // Style support
        bool hasFillColor() const { return true; }
        const utility::Color& getFillColor() const { return m_fillColor; }
        void setFillColor(const utility::Color& color) { m_fillColor = color; }

        bool hasBorder() const { return true; }
        const utility::Border& getBorder() const { return m_border; }
        void setBorder(const utility::Border& border) { m_border = border; }

    private:
        utility::Color m_fillColor;
        utility::Border m_border;
    };
}