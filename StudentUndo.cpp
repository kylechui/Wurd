#include "StudentUndo.h"

Undo* createUndo()
{
	return new StudentUndo;
}

void StudentUndo::submit(const Action action, int row, int col, char ch) {
}

StudentUndo::Action StudentUndo::get(int &row, int &col, int& count, std::string& text) {
	// If the stack is empty, return an error
	if (m_stack.empty())
		return Undo::Action::ERROR;
	// Get the values from the top of the stack
	Action act = m_stack.top().m_action;
	char ch = m_stack.top().m_char;
	row = m_stack.top().m_row;
	col = m_stack.top().m_col;
	// If the most recent action was an insert
	if (act == Action::INSERT)
	{
	}
}

void StudentUndo::clear() {
	// Empty the stack
	while (!m_stack.empty())
		m_stack.pop();
}
