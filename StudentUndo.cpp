#include "StudentUndo.h"

Undo* createUndo()
{
	return new StudentUndo;
}

void StudentUndo::submit(const Action action, int row, int col, char ch) {
	m_stack.push(new MyAction(action, row, col, ch));
}

StudentUndo::Action StudentUndo::get(int &row, int &col, int& count, std::string& text) {
	// If the stack is empty, return an error
	if (m_stack.empty())
		return Undo::Action::ERROR;
	// Get the values from the top of the stack and pop the top off
	Action act = m_stack.top()->m_action;
	char ch = m_stack.top()->m_char;
	row = m_stack.top()->m_row;
	col = m_stack.top()->m_col;
	m_stack.pop();
	// If the most recent action is an insertion
	if (act == Action::INSERT)
	{
		// Check for more insertions right after the current one
		count++;
		while (!m_stack.empty() &&
			m_stack.top()->m_action == Action::INSERT &&
			m_stack.top()->m_row == row &&
			m_stack.top()->m_col + 1 == col)
		{
			count++;
			col = m_stack.top()->m_col;
			// Get the values from the top of the stack and pop the top off
			act = m_stack.top()->m_action;
			row = m_stack.top()->m_row;
			col = m_stack.top()->m_col;
			ch = m_stack.top()->m_col;
			m_stack.pop();
		}
		col += count - 1;
		return Undo::Action::DELETE;
	}
	else if (act == Action::DELETE)
	{
		text = ch;
		count++;
		while (!m_stack.empty() &&
			m_stack.top()->m_action == Action::DELETE &&
			m_stack.top()->m_row == row &&
			m_stack.top()->m_col - 1 == col)
		{
			count++;
			col = m_stack.top()->m_col;
			ch = m_stack.top()->m_char;
			text += ch;
			// Get the values from the top of the stack and pop the top off
			act = m_stack.top()->m_action;
			row = m_stack.top()->m_row;
			col = m_stack.top()->m_col;
			ch = m_stack.top()->m_col;
			m_stack.pop();
		}
		col -= count - 1;
		return Undo::Action::INSERT;
	}
	else if (act == Action::JOIN)
	{
		count = 1;
		return Undo::Action::SPLIT;
	}
	else if (act == Action::SPLIT)
	{
		count = 1;
		return Undo::Action::JOIN;
	}
	return Undo::Action::ERROR;
}

void StudentUndo::clear() {
	// Empty the stack
	while (!m_stack.empty())
	{
		delete m_stack.top();
		m_stack.pop();
	}
}
