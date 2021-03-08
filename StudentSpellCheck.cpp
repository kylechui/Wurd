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
	// Initialise some variables
	//		hasLetter tells whether you are in a consecutive string of letters/apostrophes
	//		start is the position of the start of the word
	//		temp holds a string that builds into the word
	bool hasLetter = false;
	int start = 0;
	string temp = "";
	// Iterate through the characters in the line
	for (int i = 0; i < line.size(); i++)
	{
		// Get the ith character and store it into c
		char c = line[i];
		// If c is a letter
		if (isalpha(c))
		{
			// Convert c to lowercase immediately
			c = tolower(c);
			// If not already in a valid word, set start to the current position
			// and reset temp
			if (!hasLetter)
			{
				start = i;
				temp = "";
			}
			// We are in a word now, so hasLetter is true
			// add the character to temp
			hasLetter = true;
			temp += c;
			// If at the end of the line, try to find the word in the dictionary
			// If found, push the start and end positions into problems
			if (i == line.size() - 1 && !findString(temp))
				problems.push_back({ start, i });
		}
		// If the character is an apostrophe, add it to temp
		else if (line[i] == '\'')
			temp += c;
		// If the character is not a valid part of a word
		else
		{
			// If the cursor was in a valid word
			if (hasLetter)
			{
				// Search for the word in the dictionary and
				// if found, push its start and end positions into problems
				if (!findString(temp))
					problems.push_back({ start, i });
				// Reset hasLetter because the cursor is no longer in a valid word
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
	// If the pointer is null, create a new node and set the pointer to point to our node
	if (start == nullptr)
		start = new Node();
	// If the string is empty, set the nodes value to true
	if (s.empty())
	{
		start->m_value = true;
		return;
	}
	// Get the first character of the string and set it to lowercase, if applicable
	char c = s[0];
	if (isalpha(c))
		c = tolower(c);
	// If the character is nonalpha and not an apostrophe, just return
	else if (c != '\'')
		return;
	// Repeat the process recursively, using the first 26 children to represent the letters of the alphabet,
	// and the 27th child to represent the presence of an apostrophe
	if (c == '\'')
		addString(start->m_children[26], s.substr(1));
	else
		addString(start->m_children[c - 'a'], s.substr(1));
}

bool StudentSpellCheck::findString(string s)
{
	// If the head pointer of our trie is null, return
	if (m_trie->head == nullptr)
		return false;
	Node* ptr = m_trie->head;
	// Navigate through the tree, using the characters of the string s as indices for navigation
	for (int i = 0; i < s.size(); i++)
	{
		if (s[i] == '\'')
			ptr = ptr->m_children[26];
		else if (isalpha(s[i]))
			ptr = ptr->m_children[tolower(s[i]) - 'a'];
		// If we reach a null pointer we return false
		if (ptr == nullptr)
			return false;
	}
	// Return the value of the final node
	return ptr->m_value;
}

StudentSpellCheck::Trie::~Trie()
{
	// Free the trie
	freeNodes(head);
}

void StudentSpellCheck::Trie::freeNodes(Node* node)
{
	// If the node is null, return
	if (node == nullptr)
		return;
	// Free all of the children of the node,
	for (int i = 0; i < 27; i++)
		freeNodes((node->m_children)[i]);
	// and then delete the node
	delete node;
}


