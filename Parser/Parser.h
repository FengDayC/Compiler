#pragma once

class Grammar;

struct Word;

enum class ItemType
{
	Shift,Reduce,Jump,Accept
};

struct TableItem
{
	ItemType type;
	std::string production;
	int index;
};

class Parser
{
public:
	Parser(S_PTR(Grammar) grammar);

	void GenerateAnalyseTable();

	void Parsing(S_PTR(std::vector<Word>) words) const;

public:

	void PrintAnalyseTable(const int space) const;

private:
	S_PTR(Grammar) grammar;

	std::map<std::pair<int,std::string>,TableItem> analyseTable;
};