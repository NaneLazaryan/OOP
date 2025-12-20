#pragma once
#include "Command.h"
#include "../Models/objects/Image.h"
#include "../Models/Presentation.h"

// Legacy command - not currently used in the command system
class AddImageCommand {
public:
    AddImageCommand(double x, double y, double width, double height, const std::string& imagePath);
    bool execute(Presentation& presentation);
    bool undo(Presentation& presentation);
    std::string getDescription() const;

private:
    double x_, y_, width_, height_;
    std::string imagePath_;
    int slideIndex_, shapeIndex_;
};


