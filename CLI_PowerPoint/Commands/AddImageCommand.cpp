#include "AddImageCommand.h"
#include "../Models/Presentation.h"
#include "../Models/Slide.h"
#include "../Models/objects/Image.h"
#include "../Models/utility/Geometry.h"
#include <memory>

using namespace document::shapes;

AddImageCommand::AddImageCommand(double x, double y, double width, double height, const std::string& imagePath)
    : x_(x), y_(y), width_(width), height_(height), imagePath_(imagePath), slideIndex_(-1), shapeIndex_(-1) {
}

bool AddImageCommand::execute(Presentation& presentation) {
    Slide* slide = presentation.getCurrentSlide();
    if (!slide) {
        return false;
    }

    slideIndex_ = presentation.getCurrentSlideIndex();
    utility::Geometry geometry(utility::Point(static_cast<int>(x_), static_cast<int>(y_)), 
                               utility::Point(static_cast<int>(x_ + width_), static_cast<int>(y_ + height_)));
    auto imageShape = std::make_unique<Image>(geometry, imagePath_);
    slide->addShape(std::move(imageShape));
    shapeIndex_ = slide->getShapeCount() - 1;
    return true;
}

bool AddImageCommand::undo(Presentation& presentation) {
    if (slideIndex_ >= 0 && shapeIndex_ >= 0) {
        Slide* slide = presentation.getSlide(slideIndex_);
        if (slide) {
            slide->removeShape(shapeIndex_);
            return true;
        }
    }
    return false;
}

std::string AddImageCommand::getDescription() const {
    return "Add image: \"" + imagePath_ + "\"";
}

