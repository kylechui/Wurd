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
	// Iterate through the lines of the text
	string curLine;
	while (getline(infile, curLine))
	{
		// Strip carriage return characters
		if (curLine.size() > 0 && curLine[curLine.size() - 1] == '\r')
			curLine = curLine.substr(1, curLine.size() - 1);
		m_text.push_back(curLine);
	}
	curRow = m_text.begin();
	return true;
}

bool StudentTextEditor::save(std::string file) {
	ofstream outfile(file);
	// Return false if no file could be found
	if (!outfile)
		return false;
	// Iterate through the text and push it into the file, adding \n after every line
	list<string>::iterator it = m_text.begin();
	while (it != m_text.end())
	{
		outfile << *it << '\n';
		it++;
	}
	return true;
}

void StudentTextEditor::reset() {
	// Clear the text from the editor and the undo stack
	m_text.clear();
	getUndo()->clear();
	// Reset the cursor to (0, 0)
	m_row = 0;
	m_col = 0;
}

void StudentTextEditor::move(TextEditor::Dir dir) {
	switch (dir)
	{
	// If up is pressed, move up only if not in the first row
	case (TextEditor::Dir::UP):
		if (m_row > 0)
		{
			m_row--;
			curRow--;
			// Make sure not to go past the end of the line
			if (m_col > (*curRow).size())
				m_col = (*curRow).size();
		}
		break;
	// If down is pressed, move down only if not in the last row
	case (TextEditor::Dir::DOWN):
		if (m_row < m_text.size() - 1)
		{
			m_row++;
			curRow++;
			// Make sure not to go past the end of the line
			if (m_col > (*curRow).size())
				m_col = (*curRow).size();
		}
		break;
	// If left is pressed, move to the left if possible,
	// Otherwise go the end of the previous row if possible,
	// Otherwise do nothing
	case (TextEditor::Dir::LEFT):
		// Move left if possible
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
	case (TextEditor::Dir::RIGHT):
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
	case (TextEditor::Dir::HOME):
		m_col = 0;
		break;
	// If end is pressed, move to the end of the line
	case (TextEditor::Dir::END):
		m_col = (*curRow).size();
		break;
	default:
		break;
	}
}

void StudentTextEditor::del() {
	char ch;
	// If not deleting at the end of a line, remove the character under the cursor
	if (m_col < (*curRow).size())
	{
		// Save the character that's about to be deleted
		ch = (*curRow)[m_col];
		*curRow = (*curRow).substr(0, m_col) + (*curRow).substr(m_col + 1);
		getUndo()->submit(Undo::Action::DELETE, m_row, m_col, ch);
	}
	else
	{
		// If deleting at the end of a line and not in the last row,
		// combine the two rows into the current row and delete the next row
		if (m_row != m_text.size() - 1)
		{
			ch = '\n';
			list<string>::iterator temp = curRow;
			temp++;
			*curRow += *temp;
			m_text.erase(temp);
			getUndo()->submit(Undo::Action::JOIN, m_row, m_col, ch);
		}
	}
}

void StudentTextEditor::backspace() {
	char ch;
	// If not deleting at the beginning of a line,
	if (m_col > 0)
	{
		// Save the character that's about to be deleted
		ch = (*curRow)[m_col - 1];
		// Remove the character and decrement the column
		*curRow = (*curRow).substr(0, m_col - 1) + (*curRow).substr(m_col);
		m_col--;
		getUndo()->submit(Undo::Action::DELETE, m_row, m_col, ch);
	}
	// If deleting at the beginning of a line
	else
	{	
		// If not in the first row,
		if (m_row != 0)
		{
			// Combine the two rows into the one above and delete the current row
			ch = '\n';
			string thisRow = *curRow;
			list<string>::iterator temp = curRow;
			curRow--;
			m_text.erase(temp);
			m_col = (*curRow).size();
			m_row--;
			*curRow += thisRow;
			getUndo()->submit(Undo::Action::JOIN, m_row, m_col, ch);
		}
	}
}

void StudentTextEditor::insert(char ch) {
	// Handle the special tab case
	string s(1,ch);
	if (ch == '\t')
		s = "    ";
	// If the current row is at the end of the file then
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
	if (ch == '\t')
	{
		for (int i = 0; i < 4; i++)
		{
			m_col++;
			getUndo()->submit(Undo::Action::INSERT, m_row, m_col, ' ');
		}
	}
	else
	{
		m_col++;
		getUndo()->submit(Undo::Action::INSERT, m_row, m_col, ch);
	}
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
	// Store the cursor position in the parameter variables
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
	// While the cursor is not on the same row as the startRow,
	// Continually move it closer to startRow (by going up or down)
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
	// Initialise some variables
	int count = 0;
	int row;
	string s = "";
	// Get the action from the top of the undo stack
	Undo::Action act = getUndo()->get(row, m_col, count, s);
	// If the stack is empty, do nothing
	if (act == Undo::Action::ERROR)
		return;
	// Move the cursor to the correct row
	while (m_row != row)
	{
		if (m_row < row)
		{
			m_row++;
			curRow++;
		}
		else
		{
			m_row--;
			curRow--;
		}
	}
	// If the action is delete
	if (act == Undo::Action::DELETE)
	{
		// Delete count characters from the string and move the column back count times
		*curRow = (*curRow).substr(0, m_col - count) + (*curRow).substr(m_col);
		m_col -= count;
	}
	// If the action is insert
	else if (act == Undo::Action::INSERT)
	{
		// Insert the string into the current row and move the column forward count times
		*curRow = (*curRow).substr(0, m_col) + s + (*curRow).substr(m_col);
		m_col += count;
	}
	// If the action is split
	else if (act == Undo::Action::SPLIT)
	{
		// Copy the text after the cursor
		string nextRow = (*curRow).substr(m_col);
		// Set this row to be the part of the string before the cursor
		*curRow = (*curRow).substr(0, m_col);
		// Jump to the next line and insert the next row there
		curRow++;
		m_text.insert(curRow, nextRow);
		// Go back to the original row
		curRow--;
		curRow--;
		// Update the position of the cursor
		m_col = (*curRow).size();
	}
	// If the action is join
	else if (act == Undo::Action::JOIN)
	{
		// Copy the text from the next row and delete it
		// and add the text to the current row
		string thisRow = *curRow;
		list<string>::iterator temp = curRow;
		curRow--;
		m_text.erase(temp);
		m_col = (*curRow).size();
		m_row--;
		*curRow += thisRow;
	}
}
