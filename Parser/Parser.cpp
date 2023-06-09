#include "pch.h"
#include "Parser.h"
#include "Grammar.h"
#include "LexicalAnalyzer.h"

Parser::Parser(S_PTR(Grammar) grammar)
{
	this->grammar = std::move(grammar);
}

void Parser::GenerateAnalyseTable()
{
	int I_cnt = grammar->GetI().size();
	Item accItem = { 1,"$$$$->" + grammar->GetStart(),true };
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
		//Shift
		if (grammar->IsTerminal(symbol))
		{
			analyseTable[MK_PAIR(k, symbol)] = { ItemType::Shift,"",j };
		}
	}

	//Reduce
	for (auto &[index,i] : grammar->GetI())
	{
		auto items = i.GetAllItem();
		for (auto item : items)
		{
			if (item == accItem)
			{
				analyseTable[MK_PAIR(index, "####")] = { ItemType::Accept,"",-1 };
			}
			else if (item.fullyExtended)
			{
				Production production = grammar->GetProduction(item.production);
				std::set<std::string> follow = grammar->GetFollow(production.GetHead());
				for (auto symbol : follow)
				{
					analyseTable[MK_PAIR(index, symbol)] = { ItemType::Reduce,item.production,-1 };
				}
			}
		}
	}
}

void Parser::PrintAnalyseTable(const int space) const
{
	std::set<std::string> terminal = grammar->GetTerminal();
	terminal.insert("####");
	std::set<std::string> nonterminal = grammar->GetNonterminal();
	nonterminal.erase("$$$$");
	std::cout << std::setw(space)<<" ";
	for (std::string t : terminal)
	{
		std::cout << std::setw(space) << t;
	}
	for (std::string n : nonterminal)
	{
		std::cout << std::setw(space) << n;
	}
	std::cout << std::endl;
	for (int i = 0; i < grammar->GetI().size(); i++)
	{
		std::cout << std::setw(space) << i;
		for (std::string t : terminal)
		{
			auto it = analyseTable.find(MK_PAIR(i, t));
			if (it == analyseTable.end())
			{
				std::cout << std::setw(space) << " ";
				continue;
			}
			TableItem item = it->second;
			if (item.type == ItemType::Shift)
			{
				std::cout << std::setw(space) << "S_" << item.index;
			}
			else if (item.type == ItemType::Reduce)
			{
				std::cout << std::setw(space) << "R_(" << item.production + ")";
			}
			else if (item.type == ItemType::Accept)
			{
				std::cout << std::setw(space) << "ACC";
			}
		}
		for (std::string n : nonterminal)
		{
			auto it = analyseTable.find(MK_PAIR(i, n));
			if (it == analyseTable.end())
			{
				std::cout << std::setw(space) << " ";
				continue;
			}
			TableItem item = it->second;
			if (item.type == ItemType::Jump)
			{
				std::cout << std::setw(space) << item.index;
			}
		}
		std::cout << std::endl;
	}
}

void Parser::Parsing(S_PTR(std::vector<std::string>) words) const
{
	int p = 0;
	words->push_back("####");
	S_PTR(std::stack<std::string>, symbolStack) = MK_SPTR(std::stack<std::string>);
	S_PTR(std::stack<int>, statusStack) = MK_SPTR(std::stack<int>);
	symbolStack->push("####");
	statusStack->push(0);
	while (true)
	{
		std::string symbol = (*words)[p];
		auto it = analyseTable.find(MK_PAIR(statusStack->top(),symbol));
		if (it == analyseTable.end())
		{
			std::cout << "Error" << std::endl;
			return;
		}
		else
		{
			TableItem item = it->second;
			switch (item.type)
			{
			case ItemType::Shift:
				std::cout << "Shift:" << symbol << std::endl;
				symbolStack->push(symbol);
				statusStack->push(item.index);
				p++;
				break;
			case ItemType::Accept:
				std::cout << "Done!!" << std::endl;
				return;
			case ItemType::Reduce:
				Production production = grammar->GetProduction(item.production);
				for (int i = 0; i < production.GetLength(); i++)
				{
					symbolStack->pop();
					statusStack->pop();
				}
				symbolStack->push(production.GetHead());
				production.InvokeSubFunc((void*)production.ToString().c_str());
				auto gotoIt = analyseTable.find(MK_PAIR(statusStack->top(), production.GetHead()));
				if (gotoIt != analyseTable.end())
				{
					statusStack->push(gotoIt->second.index);
				}
				else
				{
					std::cout<<"Error" << std::endl;
					return;
				}
				break;
			}
		}
	}
}
