#pragma once

class Grammar;

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

private:
	S_PTR(Grammar) grammar;

	std::map<std::pair<int,std::string>,TableItem> analyseTable;
};