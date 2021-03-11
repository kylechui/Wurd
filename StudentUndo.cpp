#include <string>
#include "StudentUndo.h"
using namespace std;
Undo* createUndo()
{
	return new StudentUndo;
}

void StudentUndo::submit(const Action action, int row, int col, char ch) {
	// Push the action onto the stack
	m_stack.push(new MyAction(action, row, col, ch));
}

StudentUndo::Action StudentUndo::get(int &row, int &col, int& count, std::string& text) {
	// If the stack is empty, return an error
	if (m_stack.empty())
		return Undo::Action::ERROR;
	// Initialise some values
	Action act = m_stack.top()->m_action;
	char ch;
	// If the most recent action is an insertion
	if (act == Action::INSERT)
	{
		// Keep looping while the next action is consecutive insertion
		do
		{
			// Get the values from the top of the stack and pop the top off
			act = m_stack.top()->m_action;
			row = m_stack.top()->m_row;
			col = m_stack.top()->m_col;
			ch = m_stack.top()->m_char;
			m_stack.pop();
			// Increase count
			count++;
		} 
		while (!m_stack.empty() &&
				m_stack.top()->m_action == Action::INSERT &&
				m_stack.top()->m_row == row &&
				m_stack.top()->m_col + 1 == col &&
				(isalpha(m_stack.top()->m_char) || m_stack.top()->m_char == '\''));
		// Move the cursor to the correct position and return
		col += count - 1;
		return Undo::Action::DELETE;
	}
	// If the most recent action is a deletion
	else if (act == Action::DELETE)
	{
		// Initialise some variables to keep track of what is deleted by backspace versus delete
		int numMoves = 0;
		string delStr = "";
		text = "";
		do
		{
			// Get the values from the top of the stack and pop the top off
			act = m_stack.top()->m_action;
			row = m_stack.top()->m_row;
			col = m_stack.top()->m_col;
			ch = m_stack.top()->m_char;
			m_stack.pop();
			// Increment count
			count++;
			// Add the character to text or delString depending on if it was deleted by a backspace or delete
			if (!m_stack.empty() && m_stack.top()->m_col != col)
			{
				numMoves++;
				text += ch;
			}
			else
				delStr.insert(delStr.begin(), ch);
		}
		while (!m_stack.empty() &&
				m_stack.top()->m_action == Action::DELETE &&
				m_stack.top()->m_row == row &&
				(m_stack.top()->m_col == col || m_stack.top()->m_col - 1 == col));
		// Move the column back by count - 1 positions, to the correct spot to insert text
		col -= numMoves - 1;
		// Append the characters deleted by the delete key to the string deleted by backspace and return
		text += delStr;
		return Undo::Action::INSERT;
	}
	// If the most recent action joins two lines
	else if (act == Action::JOIN)
	{
		m_stack.pop();
		// Return that editor should split lines
		count = 1;
		return Undo::Action::SPLIT;
	}
	// If the most recent action splits two lines
	else if (act == Action::SPLIT)
	{
		m_stack.pop();
		// Return that editor should join lines
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
