#pragma once

#include "../../Models/Presentation.h"

namespace editing
{
	class IAction
	{
	public:
		virtual ~IAction() = default;
		virtual void doAction(document::Presentation& presentation) = 0;
		virtual std::unique_ptr<IAction> createInverse() = 0;
	};
}