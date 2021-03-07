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
	bool foundWord = findString(word);
	int count = 0;
	if (!foundWord)
	{
		for (int i = 0; i < word.size(); i++)
		{
			for (int j = 0; j < 26; j++)
			{
				string similar = word.substr(0, i) + ('a' + i + "") + word.substr(i + 1);
				if (findString(similar))
					count++;
				if (count > max_suggestions)
					return false;
			}
			string similar = word.substr(0, i) + "'" + word.substr(i + 1);
			if (findString(similar))
				count++;
			if (count > max_suggestions)
				return false;
		}
	}
	else
		return true;
}

void StudentSpellCheck::spellCheckLine(const std::string& line, std::vector<SpellCheck::Position>& problems) {
	bool hasLetter = false;
	int start = 0;
	string temp = "";
	for (int i = 0; i < line.size(); i++)
	{
		char c =line[i];
		if (isalpha(c))
		{
			c = tolower(c);
			if (!hasLetter)
			{
				start = i;
				temp = "";
			}
			hasLetter = true;
			temp += c;
			// If at the end of the line, check for typos
			if (i == line.size() - 1 && !findString(temp))
				problems.push_back({ start, i });
		}
		else if (line[i] == '\'')
			temp += c;
		else
		{
			if (hasLetter)
			{
				if (!findString(temp))
					problems.push_back({ start, i });
				hasLetter = false;
			}
		}
	}
}

StudentSpellCheck::Node::Node():
	m_value(false)
{
	for (int i = 0; i < 27; i++)
		m_children[i] = nullptr;
}

void StudentSpellCheck::Trie::addString(StudentSpellCheck::Node*& start, std::string s)
{
	if (s.empty())
	{
		if (start == nullptr)
		{
			Node* n = new Node();
			n->m_value = true;
			start = n;
			return;
		}
		else
		{
			start->m_value = true;
			return;
		}	
	}
	char c = s[0];
	if (isalpha(c))
		c = tolower(c);
	if (start == nullptr)
	{
		Node* n = new Node();
		start = n;
		if (c == '\'')
			addString(n->m_children[26], s.substr(1));
		else
			addString(n->m_children[c - 'a'], s.substr(1));
	}
	else
	{
		if (c == '\'')
			addString(start->m_children[26], s.substr(1));
		else
			addString(start->m_children[c - 'a'], s.substr(1));
	}
}

bool StudentSpellCheck::findString(string s)
{
	Node* ptr = m_trie.head;
	if (ptr == nullptr)
		return false;
	for (int i = 0; i < s.size(); i++)
	{
		if (s[i] == '\'')
			ptr = ptr->m_children[26];
		else if (isalpha(s[i]))
			ptr = ptr->m_children[tolower(s[i]) - 'a'];
		if (ptr == nullptr)
			return false;
	}
	return ptr->m_value;
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


