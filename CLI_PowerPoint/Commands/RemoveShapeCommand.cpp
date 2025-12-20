#include "RemoveShapeCommand.h"
#include "../Core/action/RemoveShapeAction.h"

using namespace cli::cmd;

RemoveShapeCommand::RemoveShapeCommand(int slideId, size_t shapeIndex)
    : m_slideId(slideId), m_shapeIndex(shapeIndex)
{}

void RemoveShapeCommand::execute(editing::Editor& editor)
{
    auto action = std::make_unique<editing::RemoveShapeAction>(m_slideId, m_shapeIndex);
    editor.doAction(std::move(action));
}