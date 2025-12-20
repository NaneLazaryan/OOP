#pragma once

#include <memory>
#include <stack>
#include "../Core/action/IAction.h"
#include "Presentation.h"

namespace editing
{
	class Editor
	{
	public:
		Editor(document::Presentation& presentation) : m_presentation(presentation) {}

		Editor(const Editor&) = delete;
		Editor& operator=(const Editor&) = delete;

		void doAction(std::unique_ptr<IAction> action);
		void undo();
		void redo();
		
		document::Presentation& getPresentation() { return m_presentation; }
		const document::Presentation& getPresentation() const { return m_presentation; }

	private:
		std::stack<std::unique_ptr<IAction>> m_undoStack;
		std::stack<std::unique_ptr<IAction>> m_redoStack;
		document::Presentation& m_presentation;
	};
}