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
	// Iterate through the lines of the dictionary, and add them to the trie
	string curLine;
	while (getline(infile, curLine))
		m_trie.addString(m_trie.head, curLine);
	return true;
}

bool StudentSpellCheck::spellCheck(std::string word, int max_suggestions, std::vector<std::string>& suggestions) {
	for (int i = 0; i < word.size(); i++)
	{
		
	}
	return true;
}

void StudentSpellCheck::spellCheckLine(const std::string& line, std::vector<SpellCheck::Position>& problems) {
	// TODO
}

StudentSpellCheck::Node::Node():
	m_value(false)
{
	for (int i = 0; i < 27; i++)
		m_children[i] = nullptr;
}

void StudentSpellCheck::Trie::addString(StudentSpellCheck::Node* start, std::string s)
{
	if (s.empty())
	{
		if (start != nullptr)
		{
			start->m_value = true;
			return;
		}
		else
		{
			Node* n = new Node();
			start = n;
			n->m_value = true;
			return;
		}
	}
	char c = tolower(s[0]);
	if (start == nullptr)
	{
		Node* n = new Node();
		start = n;
		addString(n, s.substr(1));
	}
	else
	{
		if (c == '\'')
			addString(start->m_children[26], s.substr(1));
		else
			addString(start->m_children[c - 'a'], s.substr(1));
	}
}

StudentSpellCheck::Trie::~Trie()
{
	freeNodes(head);
}

void StudentSpellCheck::Trie::freeNodes(Node* node)
{
	if (node == nullptr)
		return;
	for (int i = 0; i < 27; i++)
		freeNodes(node->m_children[i]);
	delete node;
}
