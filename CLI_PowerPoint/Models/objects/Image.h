#pragma once
#include "../Object.h"
#include <string>

namespace document::shapes
{
    class Image : public Object
    {
    public:
        Image(const utility::Geometry& geometry, const std::string& imagePath);
        Image(const utility::Geometry& geometry, const std::string& imagePath, const utility::Border& border);

        std::string getName() const;
        std::unique_ptr<IObject> clone() const;
        void accept(vizualization::IVisitor& visitor) const;

        // Image-specific properties
        const std::string& getImagePath() const { return m_imagePath; }
        void setImagePath(const std::string& path) { m_imagePath = path; }

        // Images support border but not fill color
        bool hasBorder() const { return true; }
        const utility::Border& getBorder() const { return m_border; }
        void setBorder(const utility::Border& border) { m_border = border; }

    private:
        std::string m_imagePath;
        utility::Border m_border;
    };
}