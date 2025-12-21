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
#include "../Commands/factory/HelpCreator.h"
#include "../Commands/factory/ListCreator.h"
#include "../Commands/IMetaCommand.h"

using namespace cli;
using namespace cmd;

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
    // Add Slide command
    auto addSlideCmd = std::make_shared<MetaCommand>(
        "add slide",
        "Add a new slide to the presentation",
        std::make_shared<factory::AddSlideCreator>()
        );
    addSlideCmd->addArgument(ArgumentInfo("-at", false, "end"));
    registry.registerCommand("add slide", addSlideCmd);

    // Add shape command
    auto addShapeCmd = std::make_shared<cmd::MetaCommand>(
        "add shape",
        "Add a shape (circle, rectangle, text, image, line) to a slide",
        std::make_shared<cmd::factory::AddShapeCreator>()
        );
    addShapeCmd->addArgument(ArgumentInfo("-slide", true, ""));
    addShapeCmd->addArgument(ArgumentInfo("-type", true, ""));
    addShapeCmd->addArgument(ArgumentInfo("-x", false, "0"));
    addShapeCmd->addArgument(ArgumentInfo("-y", false, "0"));
    addShapeCmd->addArgument(ArgumentInfo("-width", false, "100"));
    addShapeCmd->addArgument(ArgumentInfo("-height", false, "100"));
    addShapeCmd->addArgument(ArgumentInfo("-x2", false, "for lines"));
    addShapeCmd->addArgument(ArgumentInfo("-y2", false, "for lines"));
    addShapeCmd->addArgument(ArgumentInfo("-color", false, "white"));
    addShapeCmd->addArgument(ArgumentInfo("-border-color", false, "black"));
    addShapeCmd->addArgument(ArgumentInfo("-border-thickness", false, "1"));
    addShapeCmd->addArgument(ArgumentInfo("-text", false, "required for text shapes"));
    addShapeCmd->addArgument(ArgumentInfo("-text-color", false, "black"));
    addShapeCmd->addArgument(ArgumentInfo("-path", false, "required for image shapes"));
    registry.registerCommand("add shape", addShapeCmd);

    // Remove slide command
    auto removeSlideCmd = std::make_shared<cmd::MetaCommand>(
        "remove slide",
        "Remove a slide from the presentation",
        std::make_shared<cmd::factory::RemoveSlideCreator>()
        );
    removeSlideCmd->addArgument(ArgumentInfo("-at", false, "0"));
    registry.registerCommand("remove slide", removeSlideCmd);

    // Remove shape command
    auto removeShapeCmd = std::make_shared<cmd::MetaCommand>(
        "remove shape",
        "Remove a shape from a slide",
        std::make_shared<cmd::factory::RemoveShapeCreator>()
        );
    removeShapeCmd->addArgument(ArgumentInfo("-slide", true, ""));
    removeShapeCmd->addArgument(ArgumentInfo("-index", true, ""));
    registry.registerCommand("remove shape", removeShapeCmd);

    // Save command
    auto saveCmd = std::make_shared<cmd::MetaCommand>(
        "save",
        "Save the presentation to a file",
        std::make_shared<cmd::factory::SaveCreator>()
        );
    saveCmd->addArgument(ArgumentInfo("filename", true, ""));
    registry.registerCommand("save", saveCmd);

    // Load command
    auto loadCmd = std::make_shared<cmd::MetaCommand>(
        "load",
        "Load a presentation from a file",
        std::make_shared<cmd::factory::LoadCreator>()
        );
    loadCmd->addArgument(ArgumentInfo("filename", true, ""));
    registry.registerCommand("load", loadCmd);

    // Render command
    auto renderCmd = std::make_shared<cmd::MetaCommand>(
        "render",
        "Render the presentation to SVG format",
        std::make_shared<cmd::factory::RenderCreator>()
        );
    renderCmd->addArgument(ArgumentInfo("-format", false, "svg"));
    renderCmd->addArgument(ArgumentInfo("-file", false, "console output"));
    registry.registerCommand("render", renderCmd);

    // Undo command
    auto undoCmd = std::make_shared<cmd::MetaCommand>(
        "undo",
        "Undo the last action",
        std::make_shared<cmd::factory::UndoCreator>()
        );
    registry.registerCommand("undo", undoCmd);

    // Redo command
    auto redoCmd = std::make_shared<cmd::MetaCommand>(
        "redo",
        "Redo the last undone action",
        std::make_shared<cmd::factory::RedoCreator>()
        );
    registry.registerCommand("redo", redoCmd);

    // List command
    auto listCmd = std::make_shared<cmd::MetaCommand>(
        "list",
        "Display the contents of the presentation",
        std::make_shared<cmd::factory::ListCreator>()
        );
    registry.registerCommand("list", listCmd);

    // Help command
    auto helpCmd = std::make_shared<MetaCommand>(
        "help",
        "Display help information about commands",
        std::make_shared<factory::HelpCreator>(registry)
        );
    helpCmd->addArgument(ArgumentInfo("command", false, ""));
    registry.registerCommand("help", helpCmd);
}