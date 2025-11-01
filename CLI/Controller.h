#pragma once

#include "Presentation.h"
#include "Parser.h"


namespace cli
{
	class Controller
	{
	public:
		Controller() : presentation(), m_exit(false) {}
		~Controller() = default;

		void run();
		void exit();

		void handleError(const std::string& message);
	private:
		document::Presentation presentation;
		bool m_exit;
	};
}
