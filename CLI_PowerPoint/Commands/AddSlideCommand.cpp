#include "AddSlideCommand.h"
#include "../Core/action/AddSlideAction.h"
#include "../Models/Slide.h"
#include <memory>

using namespace cli::cmd;
using namespace document;

static int nextSlideId = 1;

AddSlideCommand::AddSlideCommand(size_t pos) : m_slideId(nextSlideId++), m_pos(pos)
{}

void AddSlideCommand::execute(editing::Editor& editor)
{
	auto newSlide = std::make_shared<Slide>(m_slideId);
	auto action = std::make_unique<editing::AddSlideAction>(m_pos, newSlide);
	editor.doAction(std::move(action));
}
