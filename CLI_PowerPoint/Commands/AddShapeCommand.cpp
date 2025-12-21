#include "AddShapeCommand.h"
#include "../Core/action/AddShapeAction.h"
#include "../Models/objects/Circle.h"
#include "../Models/objects/Image.h"
#include "../Models/objects/Rectangle.h"
#include "../Models/objects/Text.h"
#include "../Models/objects/Line.h"
#include <stdexcept>

using namespace cli::cmd;

AddShapeCommand::AddShapeCommand(int slideId,
    IObject::ObjectType shapeType,
    const utility::Geometry& geometry,
    const utility::Color& fillColor,
    const utility::Border& border)
    : m_slideId(slideId),
    m_shapeType(shapeType),
    m_geometry(geometry),
    m_fillColor(fillColor),
    m_border(border)
{}

void AddShapeCommand::execute(editing::Editor& editor)
{
    std::unique_ptr<IObject> shape = createShape();
    auto action = std::make_unique<editing::AddShapeAction>(m_slideId, std::move(shape));
    editor.doAction(std::move(action));
}

std::unique_ptr<IObject> AddShapeCommand::createShape()
{
    switch (m_shapeType) {
    case IObject::ObjectType::Circle:
        return std::make_unique<Circle>(m_geometry, m_fillColor, m_border);
    case IObject::ObjectType::Rectangle:
        return std::make_unique<Rectangle>(m_geometry, m_fillColor, m_border);
    case IObject::ObjectType::Text:
        return std::make_unique<Text>(m_geometry, "", m_fillColor, utility::Color::Transparent(), m_border);
    case IObject::ObjectType::Image:
        return std::make_unique<Image>(m_geometry, "", m_border);
    case IObject::ObjectType::Line:
        return std::make_unique<Line>(m_geometry, m_fillColor, m_border.getThickness());
    default:
        throw std::invalid_argument("Unknown shape type");
    }
}