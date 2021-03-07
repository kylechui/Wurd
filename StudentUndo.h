#ifndef STUDENTUNDO_H_
#define STUDENTUNDO_H_

#include <stack>
#include <string>
#include "Undo.h"

class StudentUndo : public Undo {
public:

	void submit(Action action, int row, int col, char ch = 0);
	Action get(int& row, int& col, int& count, std::string& text);
	void clear();

private:
	struct MyAction
	{
		MyAction(Action action, int row, int col, char ch):
			m_action(action),
			m_row(row),
			m_col(col),
			m_char(ch)
		{ }
		Action m_action;
		int m_row;
		int m_col;
		char m_char;
	};
	std::stack<MyAction*> m_stack;
};

#endif // STUDENTUNDO_H_
