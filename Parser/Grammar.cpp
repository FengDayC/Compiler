#include "pch.h"
#include "Grammar.h"

Item::Item(int dotPlace, std::string production, bool fullyExtended)
	:dotPlace(dotPlace),production(production),fullyExtended(fullyExtended)
{

}

bool Item::operator==(const Item& b) const
{
	return production == b.production && dotPlace == b.dotPlace;
}

bool Item::operator<(const Item& b) const
{
	if (production == b.production)
	{
		return dotPlace < b.dotPlace;
	}
	return production < b.production;
}

bool ItemSet::operator==(const ItemSet& b) const
{
	for (auto item : generator)
	{
		if (generator.find(item) == generator.end())
		{
			return false;
		}
	}
	return true;
}

Production::Production(std::string head, std::vector<std::string> body)
	:head(head),body(body),length(body.size())
{

}

Production::Production(std::vector<std::string> items)
{
	auto it = items.begin();
	head = *it;
	body.assign(it + 1, items.end());
	length = body.size();
}

std::string Production::ToString() const
{
	S_PTR(std::stringstream, sstream) = MK_SPTR(std::stringstream, );
	*sstream << head << "->";
	for (auto item : body)
	{
		*sstream << item;
	}
	return sstream->str();
}

Item Production::GetFirstCanonnialCollection() const
{
	return Item{ 0,ToString(),false };
}

std::string Production::GetSymbol(int index) const
{
	return body[index];
}

std::string Production::GetHead() const
{
	return head;
}

int Production::GetLength() const
{
	return length;
}

Grammar::Grammar(std::string start, std::set<std::string> terminal, std::set<std::string> nonterminal, std::map<std::string, std::vector<std::vector<std::string>>> productionsTable)
	:start(start),terminal(terminal),nonterminal(nonterminal)
{
	for (auto p : productionsTable)
	{
		std::vector<std::string> productionSameHead;
		for (auto pp : p.second)
		{
			Production production(p.first, pp);
			productionSameHead.push_back(production.ToString());
			this->productions.insert(MK_PAIR(production.ToString(), production));
		}
		this->productionsTable.insert(MK_PAIR(p.first, productionSameHead));
	}
}

ItemSet Grammar::GetClosure()
{
	ItemSet closure;
	//add an additional start
	nonterminal.insert("$$$$$$$$$$$$$$$$");
	Production additionalStart("$$$$$$$$$$$$$$$$", std::vector<std::string>{start});
	productions.insert(MK_PAIR(additionalStart.ToString(), additionalStart));
	productionsTable.insert(MK_PAIR(additionalStart.GetHead(), std::vector<std::string>{additionalStart.ToString()}));
	Item startItem{ 0,additionalStart.ToString(),false };
	closure.generator.insert(startItem);
	Extend(closure);
	return closure;
}

void Grammar::PrintAllProduction() const
{
	for (auto p : productions)
	{
		std::cout << p.second.ToString() << std::endl;
	}
}

Production Grammar::GetProduction(std::string key) const
{
	return productions.at(key);
}

std::string Grammar::GetDotSymbol(Item item) const
{
	static std::map<Item, std::string> cache;
	auto it = cache.find(item);
	if (it == cache.end())
	{
		std::string symbol = GetProduction(item.production).GetSymbol(item.dotPlace);
		cache.insert(MK_PAIR(item, symbol));
		return symbol;
	}
	else
	{
		return it->second;
	}
}

Item Grammar::GetNxt(Item item) const
{
	return Item{ item.dotPlace + 1,item.production,item.dotPlace >= GetProduction(item.production).GetLength() };
}

std::string Grammar::GetUUID(Item item) const
{
	Production production = GetProduction(item.production);
	S_PTR(std::stringstream, sstream) = MK_SPTR(std::stringstream, );
	*sstream << production.GetHead() << "->";
	for (int i = 0; i < production.GetLength(); i++)
	{
		if (item.dotPlace == i)
		{
			*sstream << ".";
		}
		*sstream << production.GetSymbol(i);
	}

	return sstream->str();
}

bool Grammar::IsTerminal(std::string symbol) const
{
	return terminal.find(symbol) != terminal.end();
}

bool Grammar::IsNonTerminal(std::string symbol) const
{
	return nonterminal.find(symbol) != nonterminal.end();
}

void Grammar::Extend(ItemSet &itemSet) const
{
	for (auto item : itemSet.generator)
	{
		std::string dotSymbol = GetDotSymbol(item);
		if (!item.fullyExtended && IsNonTerminal(dotSymbol))
		{
			std::vector<std::string> productions = productionsTable.at(dotSymbol);
			for (auto production : productions)
			{
				Item firstItem = GetProduction(production).GetFirstCanonnialCollection();
				itemSet.items.insert(firstItem);
			}
		}
	}
	std::set<Item> delta;
	do
	{
		delta.clear();

		for (auto item : itemSet.items)
		{
			std::string dotSymbol = GetDotSymbol(item);
			if (!item.fullyExtended && IsNonTerminal(dotSymbol))
			{
				std::vector<std::string> productions = productionsTable.at(dotSymbol);
				for (auto production : productions)
				{
					Item firstItem = GetProduction(production).GetFirstCanonnialCollection();
					if (itemSet.items.find(firstItem) == itemSet.items.end() && delta.find(firstItem) == delta.end())
					{
						delta.insert(firstItem);
					}
				}
			}
		}

		itemSet.items.insert(delta.begin(), delta.end());
	} while (delta.size());
}

void Grammar::PrintItemSet(ItemSet itemSet) const
{
	for (auto g : itemSet.generator)
	{
		std::cout << GetUUID(g) << std::endl;
	}
	for (auto i : itemSet.items)
	{
		std::cout << GetUUID(i) << std::endl;
	}
}