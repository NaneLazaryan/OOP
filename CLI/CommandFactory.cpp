#include "CommandFactory.h"
#include "IncludsCommands.h"

std::unique_ptr<ICommand> CommandFactory::createCommand(Keyword action, Keyword target)
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

std::unique_ptr<ICommand> CommandFactory::createAddCommand(Keyword target)
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

std::unique_ptr<ICommand> CommandFactory::createRemoveCommand(Keyword target)
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

std::unique_ptr<ICommand> CommandFactory::createSetCommand(Keyword target)
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
std::unique_ptr<ICommand> CommandFactory::createEditCommand(Keyword target)
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