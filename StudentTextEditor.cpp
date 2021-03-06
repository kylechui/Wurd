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
	// TODO
}

void StudentTextEditor::del() {
	// TODO
}

void StudentTextEditor::backspace() {
	// If deleting in the middle of a line, remove the character
	// and decrement the column
	if (m_col > 0)
	{
		*curRow = (*curRow).substr(0, m_col - 1) + (*curRow).substr(m_col);
		m_col--;
	}
	else
	{
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
	// TODO
	string s;
	switch (ch)
	{
	case 9:
		s = "    ";
		break;
	default:
		s = ch;
		break;
	}
	if (curRow == m_text.end())
	{
		m_text.push_back("");
		curRow = m_text.begin();
	}
	*curRow = (*curRow).substr(0, m_col) + s + (*curRow).substr(m_col);
	if (ch == 9)
		m_col += 3;
	m_col++;
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
	if (startRow < 0 || numRows < 0 || startRow > m_text.size())
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
	// TODO
}
