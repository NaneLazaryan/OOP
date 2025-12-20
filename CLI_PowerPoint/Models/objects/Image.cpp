#include "Image.h"
#include "../../Core/vizualization/IVisitor.h"

namespace document::shapes
{
    Image::Image(const utility::Geometry& geometry, const std::string& imagePath)
        : Object(ObjectType::Image, geometry),
        m_imagePath(imagePath),
        m_border(utility::Border(utility::Color::Black(), 0.0, false))
    {}

    Image::Image(const utility::Geometry& geometry, const std::string& imagePath, const utility::Border& border)
        : Object(ObjectType::Image, geometry),
        m_imagePath(imagePath),
        m_border(border)
    {}

    std::string Image::getName() const
    {
        return "Image";
    }

    std::unique_ptr<IObject> Image::clone() const
    {
        return std::make_unique<Image>(m_geometry, m_imagePath, m_border);
    }

    void Image::accept(vizualization::IVisitor& visitor) const
    {
        visitor.visit(*this);
    }
}
