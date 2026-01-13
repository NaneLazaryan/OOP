#pragma once
#include "../Object.h"
#include <string>

namespace document::shapes
{
    class Text : public Object
    {
    public:
        Text(const utility::Geometry& geometry, const std::string& text);
        Text(const utility::Geometry& geometry, const std::string& text, const utility::Color& textColor, const utility::Color& backgroundColor, const utility::Border& border);

        std::string getName() const;
        std::unique_ptr<IObject> clone() const;
        void accept(IObjectVisitor& visitor) const;

        // Text-specific properties
        const std::string& getText() const { return m_text; }
        void setText(const std::string& text) { m_text = text; }

        const utility::Color& getTextColor() const { return m_textColor; }
        void setTextColor(const utility::Color& color) { m_textColor = color; }

        // Style support
        bool hasFillColor() const { return true; }
        const utility::Color& getFillColor() const { return m_backgroundColor; }
        void setFillColor(const utility::Color& color) { m_backgroundColor = color; }

        bool hasBorder() const { return true; }
        const utility::Border& getBorder() const { return m_border; }
        void setBorder(const utility::Border& border) { m_border = border; }

    private:
        std::string m_text;
        utility::Color m_textColor;
        utility::Color m_backgroundColor;
        utility::Border m_border;
    };
}