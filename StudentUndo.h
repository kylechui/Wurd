#ifndef STUDENTUNDO_H_
#define STUDENTUNDO_H_

#include "Undo.h"

class StudentUndo : public Undo {
public:

	void submit(Action action, int row, int col, char ch = 0);
	Action get(int& row, int& col, int& count, std::string& text);
	void clear();

private:

};

#endif // STUDENTUNDO_H_
