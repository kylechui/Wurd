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
	delete m_trie;
}

bool StudentSpellCheck::load(std::string dictionaryFile) {
	ifstream infile(dictionaryFile);
	// Return false if no file could be found
	if (!infile)
		return false;
	// Iterate through the lines of the dictionary, and add them to the trie
	string curLine;
	while (getline(infile, curLine))
		m_trie->addString(m_trie->head, curLine);
	return true;
}

bool StudentSpellCheck::spellCheck(std::string word, int max_suggestions, std::vector<std::string>& suggestions) {
	// See if the word is valid or not
	bool foundWord = findString(word);
	// Initialise the current number of suggestions to zero and clear all previous suggestions
	int count = 0;
	suggestions.clear();
	// Initialise the string that will find "close" matches
	string similar = "";
	// If the word is not in the dictionary
	if (!foundWord)
	{
		// Go through each position of the word
		for (int i = 0; i < word.size(); i++)
		{
			// Replace the ith character with all 26 letters and an apostrophe, and see if those are
			// valid words in the dictionary
			for (int j = 0; j < 26; j++)
			{
				// Create a modified word by changing one character
				string temp(1, 'a' + j);
				similar = word.substr(0, i) + temp + word.substr(i + 1);
				// If the modified word is in the dictionary, push that word into the suggestions
				if (findString(similar))
				{
					count++;
					suggestions.push_back(similar);
				}
				// If the count reaches the max, return
				if (count >= max_suggestions)
					return false;
			}
			// Modify the word by replacing the ith character with an apostrophe
			similar = word.substr(0, i) + "\'" + word.substr(i + 1);
			// If the modified word is in the dictionary, push that word into the suggestions
			if (word[i] != '\'' && findString(similar))
			{
				count++;
				suggestions.push_back(similar);
			}
			// If the count reaches the max, return
			if (count > max_suggestions)
				return false;
		}
	}
	// If the word is in the dictionary, return true
	else
		return true;
	return false;
}

void StudentSpellCheck::spellCheckLine(const std::string& line, std::vector<SpellCheck::Position>& problems) {
	bool hasLetter = false;
	int start = 0;
	string temp = "";
	for (int i = 0; i < line.size(); i++)
	{
		char c = line[i];
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
	if (start == nullptr)
		start = new Node();
	if (s.empty())
	{
		start->m_value = true;
		return;
	}
	char c = s[0];
	if (isalpha(c))
		c = tolower(c);
	// If the character is nonalpha and not an apostrophe, just return
	else if (c != '\'')
		return;
	if (c == '\'')
		addString(start->m_children[26], s.substr(1));
	else
		addString(start->m_children[c - 'a'], s.substr(1));
}

bool StudentSpellCheck::findString(string s)
{
	if (m_trie->head == nullptr)
		return false;
	Node* ptr = m_trie->head;
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
		freeNodes((node->m_children)[i]);
	delete node;
}


