#include "RemoveSlideCommand.h"
#include "../Core/action/RemoveSlideAction.h"

using namespace cli::cmd;

RemoveSlideCommand::RemoveSlideCommand(size_t pos) : m_pos(pos) {}

void RemoveSlideCommand::execute(editing::Editor& editor)
{
	auto action = std::make_unique<editing::RemoveSlideAction>(m_pos);
	editor.doAction(std::move(action));
}