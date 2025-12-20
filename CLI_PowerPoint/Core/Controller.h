#pragma once

#include "../Models/Presentation.h"
#include "../Models/Editor.h"
#include "CommandRegister.h"
#include "Parser.h"


namespace cli
{
	class Controller
	{
	public:
		Controller(CommandRegister& registry, editing::Editor& editor) 
			:  m_registry(registry), m_editor(editor), m_exit(false) {}
		~Controller() = default;

		void run();
		void exit();

		void handleError(const std::string& message);
	private:
		CommandRegister& m_registry;
		editing::Editor& m_editor;
		bool m_exit;
	};
}
