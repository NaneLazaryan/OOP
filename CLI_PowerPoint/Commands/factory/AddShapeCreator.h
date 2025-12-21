#pragma once

#include "ICmdCreator.h"
#include "../AddShapeCommand.h"
#include "../../Models/IObject.h"
#include "../../Models/utility/Geometry.h"
#include "../../Models/utility/Color.h"
#include "../../Models/utility/Border.h"
#include <string>
#include <algorithm>
#include <sstream>

namespace cli::cmd::factory
{
	class AddShapeCreator : public ICmdCreator
	{
	public:
        CommandPtr createCommand(const ArgMap& args)
        {
            using namespace document::shapes;

            int slideId = -1;
            IObject::ObjectType shapeType = IObject::ObjectType::Rectangle;
            utility::Geometry geometry;
            utility::Color fillColor = utility::Color::White();
            utility::Color borderColor = utility::Color::Black();
            double borderThickness = 1.0;
            bool hasBorder = false;
            bool hasFill = false;

            // Get slide ID (required)
            auto itSlide = args.find("-slide");
            if (itSlide != args.end()) {
                slideId = std::get<int>(itSlide->second);
            }
            else {
                throw std::invalid_argument("Slide ID is required (-slide)");
            }

            // Get shape type (required)
            auto itType = args.find("-type");
            if (itType != args.end()) {
                std::string typeStr = std::get<std::string>(itType->second);
                shapeType = getObjectType(typeStr);
            }
            else {
                throw std::invalid_argument("Shape type is required (-type)");
            }

            // Get geometry
            geometry = getGeometry(args);

            // Parse style attributes
            auto itColor = args.find("-color");
            if (itColor != args.end()) {
                fillColor = getColor(std::get<std::string>(itColor->second));
                hasFill = true;
            }

            auto itBorderColor = args.find("-border-color");
            if (itBorderColor != args.end()) {
                borderColor = getColor(std::get<std::string>(itBorderColor->second));
                hasBorder = true;
            }

            auto itBorderThickness = args.find("-border-thickness");
            if (itBorderThickness != args.end()) {
                borderThickness = std::get<int>(itBorderThickness->second);
                hasBorder = true;
            }

            utility::Border border(borderColor, borderThickness, hasBorder);

            return std::make_unique<AddShapeCommand>(slideId, shapeType, geometry, fillColor, border);
        }

    private:
        IObject::ObjectType getObjectType(const std::string& typeStr)
        {
            using namespace document::shapes;

            std::string lower = typeStr;
            std::transform(lower.begin(), lower.end(), lower.begin(),
                [](unsigned char c) { return std::tolower(c); });

            if (lower == "circle")
                return IObject::ObjectType::Circle;
            else if (lower == "rectangle" || lower == "rect")
                return IObject::ObjectType::Rectangle;
            else if (lower == "text")
                return IObject::ObjectType::Text;
            else if (lower == "image")
                return IObject::ObjectType::Image;
            else if (lower == "line")
                return IObject::ObjectType::Line;
            else
                throw std::invalid_argument("Unknown shape type: " + typeStr);
        }

        utility::Geometry getGeometry(const ArgMap& args)
        {
            using namespace document::shapes::utility;

            int x1 = 0, y1 = 0, x2 = 100, y2 = 100;

            auto itX = args.find("-x");
            if (itX != args.end()) {
                x1 = std::get<int>(itX->second);
            }

            auto itY = args.find("-y");
            if (itY != args.end()) {
                y1 = std::get<int>(itY->second);
            }

            auto itWidth = args.find("-width");
            if (itWidth != args.end()) {
                int width = std::get<int>(itWidth->second);
                x2 = x1 + width;
            }

            auto itHeight = args.find("-height");
            if (itHeight != args.end()) {
                int height = std::get<int>(itHeight->second);
                y2 = y1 + height;
            }

            // x2/y2 for lines
            auto itX2 = args.find("-x2");
            if (itX2 != args.end()) {
                x2 = std::get<int>(itX2->second);
            }

            auto itY2 = args.find("-y2");
            if (itY2 != args.end()) {
                y2 = std::get<int>(itY2->second);
            }

            return Geometry(Point(x1, y1), Point(x2, y2));
        }

        utility::Color getColor(const std::string& colorStr)
        {
            using namespace document::shapes::utility;

            std::string lower = colorStr;
            std::transform(lower.begin(), lower.end(), lower.begin(),
                [](unsigned char c) { return std::tolower(c); });

            // Named colors
            if (lower == "white") return Color::White();
            if (lower == "black") return Color::Black();
            if (lower == "red") return Color::Red();
            if (lower == "green") return Color::Green();
            if (lower == "blue") return Color::Blue();

            // RGB format: "r,g,b" or "r,g,b,a"
            if (colorStr.find(',') != std::string::npos) {
                std::istringstream iss(colorStr);
                int r, g, b, a = 255;
                char comma;

                iss >> r >> comma >> g >> comma >> b;
                if (iss >> comma >> a) {
                    return Color(r, g, b, a);
                }
                return Color(r, g, b);
            }

            // Default to white
            return Color::White();
        }
	};
}