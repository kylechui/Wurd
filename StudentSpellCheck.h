#ifndef STUDENTSPELLCHECK_H_
#define STUDENTSPELLCHECK_H_

#include "SpellCheck.h"

#include <string>
#include <vector>

class StudentSpellCheck : public SpellCheck {
public:
	StudentSpellCheck() { m_trie = new Trie(); };
	virtual ~StudentSpellCheck();
	bool load(std::string dict_file);
	bool spellCheck(std::string word, int maxSuggestions, std::vector<std::string>& suggestions);
	void spellCheckLine(const std::string& line, std::vector<Position>& problems);

private:
	bool findString(std::string s);
	struct Node
	{
		Node();
		~Node();
		bool m_value;
		Node* m_children[27];
	};
	struct Trie
	{
		Trie() { head = new Node(); };
		~Trie();
		void addString(Node*& start, std::string s);
		void freeNodes(Node* node);
		Node* head;
	};
	Trie* m_trie;
};

#endif  // STUDENTSPELLCHECK_H_
