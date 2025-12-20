#pragma once

#include "ICmdCreator.h"
#include "../RemoveShapeCommand.h"

namespace cli::cmd::factory
{
    class RemoveShapeCreator : public ICmdCreator
    {
    public:
        CommandPtr createCommand(const ArgMap& args) override
        {
            int slideId = -1;
            size_t shapeIndex = 0;

            // Get slide ID (required)
            auto itSlide = args.find("-slide");
            if (itSlide != args.end()) {
                slideId = std::get<int>(itSlide->second);
            }
            else {
                throw std::invalid_argument("Slide ID is required (-slide)");
            }

            // Get shape index
            auto itIndex = args.find("-index");
            if (itIndex != args.end()) {
                shapeIndex = static_cast<size_t>(std::get<int>(itIndex->second));
            }
            else {
                // Try to get from first positional argument
                auto it0 = args.find("$0");
                if (it0 != args.end()) {
                    shapeIndex = static_cast<size_t>(std::get<int>(it0->second));
                }
                else {
                    throw std::invalid_argument("Shape index is required (-index or positional argument)");
                }
            }

            return std::make_unique<RemoveShapeCommand>(slideId, shapeIndex);
        }
    };
}