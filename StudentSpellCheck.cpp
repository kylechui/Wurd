#include "StudentSpellCheck.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
using namespace std;
SpellCheck* createSpellCheck()
{
	return new StudentSpellCheck;
}

StudentSpellCheck::~StudentSpellCheck() {
	// TODO
}

bool StudentSpellCheck::load(std::string dictionaryFile) {
	ifstream infile(dictionaryFile);
	// Return false if no file could be found
	if (!infile)
		return false;
	// Iterate through the lines of the text, TODO: remove carriage returns
	string curLine;
	while (getline(infile, curLine))
		m_trie.addString(m_trie.head, curLine);
	return true;
}

bool StudentSpellCheck::spellCheck(std::string word, int max_suggestions, std::vector<std::string>& suggestions) {
	return false; // TODO
}

void StudentSpellCheck::spellCheckLine(const std::string& line, std::vector<SpellCheck::Position>& problems) {
	// TODO
}

void StudentSpellCheck::Trie::addString(StudentSpellCheck::Node* start, std::string s)
{
	if (s.empty())
		return;
	char c = tolower(s[0]);
	if (start == nullptr)
	{
		Node* n = new Node(c);
		start = n;
		addString(n, s.substr(1));
	}
	else
	{
		vector<Node*>::iterator it = (start->m_children).begin();
		while (it != (start->m_children).end())
		{
			if ((*it)->m_value == c)
			{
				addString(*it, s.substr(1));
				return;
			}
			it++;
		}
		if (it == (start->m_children).end())
		{
			Node* n = new Node(c);
			(start->m_children).push_back(n);
			addString(n, s.substr(1));
		}
	}
}
