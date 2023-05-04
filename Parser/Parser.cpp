#include "pch.h"
#include "Parser.h"
#include "Grammar.h"

Parser::Parser(S_PTR(Grammar) grammar)
{
	this->grammar = std::move(grammar);
}

void Parser::GenerateAnalyseTable()
{
	int I_cnt = grammar->GetI().size();
	Item accItem = { 1,"$$$$$$$$$$$$$$$$->" + grammar->GetStart(),true };
	for (auto &[pair,j] : grammar->GetGO())
	{
		int k = pair.first;
		std::string symbol = pair.second;
		ItemSet I_j = grammar->GetI()[j];
		ItemSet I_k = grammar->GetI()[k];
		//GOTO
		if (grammar->IsNonTerminal(symbol))
		{
			analyseTable[MK_PAIR(k, symbol)] = { ItemType::Jump,"",j };
		}
	}

}
