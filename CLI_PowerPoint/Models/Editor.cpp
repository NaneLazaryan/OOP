#include "Editor.h"

using namespace editing;

void Editor::doAction(std::unique_ptr<IAction> action)
{
	// Inverse action for undo
	std::unique_ptr<IAction> inverseAction = action->createInverse();
	action->doAction(m_presentation);

	m_undoStack.push(std::move(inverseAction));
	
	while (!m_redoStack.empty()) {
		m_redoStack.pop();
	}
}

void Editor::undo()
{
	if (m_undoStack.empty()) return;

	std::unique_ptr<IAction> action = std::move(m_undoStack.top());
	m_undoStack.pop();

	action->doAction(m_presentation);
	// Inverse action for redo
	std::unique_ptr<IAction> inverseAction = action->createInverse();
	

	m_redoStack.push(std::move(inverseAction));
}

void Editor::redo()
{
	if (m_redoStack.empty()) return;

	std::unique_ptr<IAction> action = std::move(m_redoStack.top());
	m_redoStack.pop();

	// Inverse action for redo
	std::unique_ptr<IAction> inverseAction = action->createInverse();
	action->doAction(m_presentation);

	m_undoStack.push(std::move(inverseAction));
} 