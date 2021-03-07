#include "StudentTextEditor.h"
#include "Undo.h"
#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <vector>
using namespace std;

TextEditor* createTextEditor(Undo* un)
{
	return new StudentTextEditor(un);
}

StudentTextEditor::StudentTextEditor(Undo* undo)
 : TextEditor(undo), m_row(0), m_col(0), curRow(m_text.begin())
{
}

StudentTextEditor::~StudentTextEditor()
{
	m_text.clear();
}

bool StudentTextEditor::load(std::string file) {
	ifstream infile(file);
	// Return false if no file could be found
	if (!infile)
		return false;
	// Reset the editor
	reset();
	// Iterate through the lines of the text, TODO: remove carriage returns
	string curLine;
	while (getline(infile, curLine))
	{
		m_text.push_back(curLine);
	}
	curRow = m_text.begin();
	return true;
}

bool StudentTextEditor::save(std::string file) {
	return true;  // TODO
}

void StudentTextEditor::reset() {
	//TODO: reset undo operations as well
	m_text.clear();
	m_row = 0;
	m_col = 0;
}

void StudentTextEditor::move(Dir dir) {
	switch (dir)
	{
	// If up is pressed, move up only if not in the first row
	case (Dir::UP):
		if (m_row > 0)
		{
			m_row--;
			curRow--;
			if (m_col > (*curRow).size())
				m_col = (*curRow).size();
		}
		break;
	// If down is pressed, move down only if not in the last row
	case (Dir::DOWN):
		if (m_row < m_text.size() - 1)
		{
			m_row++;
			curRow++;
			if (m_col > (*curRow).size())
				m_col = (*curRow).size();
		}
		break;
	// If left is pressed, move to the left if possible,
	// Otherwise go the end of the previous row if possible,
	// Otherwise do nothing
	case (Dir::LEFT):
		if (m_col != 0)
			m_col--;
		else if (m_row != 0)
		{
			m_row--;
			curRow--;
			m_col = (*curRow).size();
		}
		break;
	// If right is pressed, move to the right if possible,
	// Otherwise go to the beginning of the next row if possible,
	// Otherwise do nothing
	case (Dir::RIGHT):
		if (m_col != (*curRow).size())
			m_col++;
		else if (m_row != m_text.size() - 1)
		{
			m_row++;
			curRow++;
			m_col = 0;
		}
		break;
	// If home is pressed, move to the beginning of the line
	case (Dir::HOME):
		m_col = 0;
		break;
	// If end is pressed, move to the end of the line
	case (Dir::END):
		m_col = (*curRow).size();
		break;
	default:
		break;
	}
}

void StudentTextEditor::del() {
	// If not deleting at the end of a line, remove the character under the cursor
	if (m_col < (*curRow).size())
		*curRow = (*curRow).substr(0, m_col) + (*curRow).substr(m_col + 1);
	else
	{
		// If deleting at the end of a line and not in the last row,
		// combine the two rows into the current row and delete the next row
		if (m_row != m_text.size() - 1)
		{
			list<string>::iterator temp = curRow;
			temp++;
			*curRow += *temp;
			m_text.erase(temp);
		}
	}
}

void StudentTextEditor::backspace() {
	// If not deleting at the beginning of a line, remove the character
	// and decrement the column
	if (m_col > 0)
	{
		*curRow = (*curRow).substr(0, m_col - 1) + (*curRow).substr(m_col);
		m_col--;
	}
	else
	{
		// If deleting at the beginning of a line and not in the first row,
		// combine the two rows into the one above and delete the current row
		if (m_row != 0)
		{
			string thisRow = *curRow;
			list<string>::iterator temp = curRow;
			curRow--;
			m_text.erase(temp);
			m_col = (*curRow).size();
			m_row--;
			*curRow += thisRow;
		}
	}
}

void StudentTextEditor::insert(char ch) {
	string s;
	// If the character is a tab, insert four spaces instead
	if (ch == 9)
		s = "    ";
	else
		s = ch;
	// If the current row is at the end (typically a new file) then
	// add a new row that's empty
	if (curRow == m_text.end())
	{
		m_text.push_back("");
		curRow = m_text.begin();
	}
	// Insert the character(s)
	*curRow = (*curRow).substr(0, m_col) + s + (*curRow).substr(m_col);
	// If the character is a tab, move forward by 4 spaces
	// Otherwise just move forward by one
	if (ch == 9)
		m_col += 4;
	else
		m_col++;
	getUndo()->submit(Undo::Action::INSERT, m_row, m_col, ch);
}

void StudentTextEditor::enter() {
	// Get the part of the string after the cursor
	string nextRow = (*curRow).substr(m_col);
	// Set this row to be the part of the string before the cursor
	*curRow = (*curRow).substr(0, m_col);
	// Jump to the next line and insert the next row there
	curRow++;
	m_text.insert(curRow, nextRow);
	// Go back to the previous row
	curRow--;
	// Update the position of the cursor
	m_row++;
	m_col = 0;
}

void StudentTextEditor::getPos(int& row, int& col) const {
	row = m_row;
	col = m_col;
}

int StudentTextEditor::getLines(int startRow, int numRows, std::vector<std::string>& lines) const {
	if (startRow < 0 || numRows < 0 || startRow > m_text.size() - 1)
		return -1;
	lines.clear();
	// Navigate a pointer to startRow
	list<string>::iterator it = curRow;
	int tempRow = m_row;
	int count = 0;
	while (tempRow != startRow)
	{
		if (tempRow > startRow)
		{
			tempRow--;
			it--;
		}
		else
		{
			tempRow++;
			it++;
		}
	}
	// Copy the text over to lines
	while (count < numRows)
	{
		if (it == m_text.end())
			break;
		lines.push_back(*it);
		it++;
		count++;
	}
	return count;
}

void StudentTextEditor::undo() {
	int count = 0;
	string s = "";
	Undo::Action act = getUndo()->get(m_row, m_col, count, s);
	if (act == Undo::Action::DELETE)
	{
		*curRow = (*curRow).substr(0, m_col - count) + (*curRow).substr(m_col);
		m_col -= count;
	}
}
