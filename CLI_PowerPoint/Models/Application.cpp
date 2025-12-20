#include "Appliaction.h"
#include "../Commands/factory/AddShapeCreator.h"
#include "../Commands/factory/AddSlideCreator.h"
#include "../Commands/factory/RemoveShapeCreator.h"
#include "../Commands/factory/RemoveSlideCreator.h"
#include "../Commands/factory/SavaCreator.h"
#include "../Commands/factory/LoadCreator.h"
#include "../Commands/factory/RenderCreator.h"
#include "../Commands/factory/UndoCreator.h"
#include "../Commands/factory/RedoCreator.h"

using namespace cli;

void Application::run()
{
    document::Presentation pres;
    editing::Editor editor(pres);

    CommandRegister registry;
    registerCommands(registry);

    Controller controller(registry, editor);
    controller.run();
}

void Application::registerCommands(CommandRegister& registry)
{
    // Slide commands
    registry.registerCommand("add slide", std::make_shared<cmd::factory::AddSlideCreator>());
    registry.registerCommand("add shape", std::make_shared<cmd::factory::AddShapeCreator>());

    // Shape commands
    registry.registerCommand("remove slide", std::make_shared<cmd::factory::RemoveSlideCreator>());
    registry.registerCommand("remove shape", std::make_shared<cmd::factory::RemoveShapeCreator>());

    // File I/O commands
    registry.registerCommand("save", std::make_shared<cmd::factory::SaveCreator>());
    registry.registerCommand("load", std::make_shared<cmd::factory::LoadCreator>());

    // Rendering commands
    registry.registerCommand("render", std::make_shared<cmd::factory::RenderCreator>());

    // Undo/Redo commands
    registry.registerCommand("undo", std::make_shared<cmd::factory::UndoCreator>());
    registry.registerCommand("redo", std::make_shared<cmd::factory::RedoCreator>());
}