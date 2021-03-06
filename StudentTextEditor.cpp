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
	// Reset the file
	reset();
	// Iterate through the lines of the text, TODO: remove carriage returns
	string curLine;
	while (getline(infile, curLine))
	{
		m_text.push_back(curLine);
	}
	return true;
}

bool StudentTextEditor::save(std::string file) {
	return true;  // TODO
}

void StudentTextEditor::reset() {
	//TODO: reset undo operations as well
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
	// TODO
}

void StudentTextEditor::insert(char ch) {
	// TODO
}

void StudentTextEditor::enter() {
	// TODO
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
	for (int i = 0; i < numRows; i++)
	{
		if (i >= m_text.size() - startRow)
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
