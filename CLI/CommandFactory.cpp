#include "CommandFactory.h"
#include "IncludsCommands.h"

using namespace cli;
using namespace cli::cmd;

bool CommandFactory::isStandaloneCommand(Keyword action)
{
	return action == Keyword::LOAD || action == Keyword::SAVE;
}

CommandPtr CommandFactory::createStandaloneCommand(Keyword action)
{
	switch (action)
	{
	case Keyword::SAVE:
		return std::make_unique<SaveCommand>();
	case Keyword::LOAD:
		return std::make_unique<LoadCommand>();
	default:
		throw std::invalid_argument("Not a standalone command");;
	}
}

CommandPtr CommandFactory::createCommand(Keyword action, Keyword target)
{
	if (!isValidCombination(action, target))
		throw std::invalid_argument("Invalid command combination");

	switch (action)
	{
	case Keyword::ADD:
		return createAddCommand(target);
	case Keyword::REMOVE:
		return createRemoveCommand(target);
	case Keyword::EDIT:
		return createEditCommand(target);
	case Keyword::SET:
		return createSetCommand(target);
	default:
		throw std::invalid_argument("Unkown action keyword");
	}
}

bool CommandFactory::isValidCombination(Keyword action, Keyword target)
{
	switch (action)
	{
	case Keyword::ADD:
		return target == Keyword::SLIDE || target == Keyword::SHAPE;
	case Keyword::REMOVE:
		return target == Keyword::SLIDE || target == Keyword::SHAPE;
	case Keyword::EDIT:
		return target == Keyword::SLIDE || target == Keyword::SHAPE || target == Keyword::BULLET;
	case Keyword::SET:
		return target == Keyword::TITLE;
	default:
		return false;
	}
}

CommandPtr CommandFactory::createAddCommand(Keyword target)
{
	switch (target)
	{
	case Keyword::SLIDE:
		return std::make_unique<AddSlideCommand>();
	case Keyword::SHAPE:
		return std::make_unique<AddShapeCommand>();
	default:
		throw std::invalid_argument("Invalid target for ADD command");
	}
}

CommandPtr CommandFactory::createRemoveCommand(Keyword target)
{
	switch (target)
	{
	case Keyword::SLIDE:
		return std::make_unique<RemoveSlideCommand>();
	case Keyword::SHAPE:
		return std::make_unique<RemoveShapeCommand>();
	default:
		throw std::invalid_argument("Invalid target for REMOVE command");
	}
}

CommandPtr CommandFactory::createSetCommand(Keyword target)
{
	switch (target)
	{
	case Keyword::TITLE:
		return std::make_unique<SetTitleCommand>();

	default:
		throw std::invalid_argument("Invalid target for SET command");
	}
}

// Working...
CommandPtr CommandFactory::createEditCommand(Keyword target)
{
	switch (target)
	{
	case Keyword::SLIDE:
		throw std::invalid_argument("Edit slide not yet implemented");
	case Keyword::SHAPE:
		throw std::invalid_argument("Edit shape not yet implemented");
	case Keyword::BULLET:
		throw std::invalid_argument("Edit bullet not yet implemented");
	default:
		throw std::invalid_argument("Invalid target for EDIT command");
	}
}