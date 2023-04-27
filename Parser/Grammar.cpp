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
	return Item{ item.dotPlace + 1,item.production,item.dotPlace+1 >= GetProduction(item.production).GetLength() };
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

	if (item.dotPlace == production.GetLength())
	{
		*sstream << ".";
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
		if (item.fullyExtended)
		{
			continue;
		}
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

std::string Grammar::GetUUID(ItemSet itemSet) const
{
	S_PTR(std::stringstream, sstream) = MK_SPTR(std::stringstream, );
	for (auto g : itemSet.generator)
	{
		*sstream << GetUUID(g) << " ";
	}
	//*sstream << "$";
	for (auto i : itemSet.items)
	{
		*sstream << GetUUID(i) << " ";
	}
	return sstream->str();
}

void Grammar::GenerateGOandI(ItemSet closure)
{
	//I_0=Closure({"$$$$$$$$$$$$$$$$"->start})
	I[0] = closure;

	std::map<std::string,int> in;
	in[GetUUID(closure)] = 0;
	int cnt = 1, index = 0;

	while (index < cnt)
	{
		std::cout << "index=" << index << " cnt=" << cnt << std::endl;
		ItemSet now = I[index];
		std::map<std::string, std::list<Item>> temp_table;
		for (auto& g : now.generator)
		{
			if (!g.fullyExtended)
			{
				std::string symbol = GetDotSymbol(g);
				auto it = temp_table.find(symbol);
				if (it == temp_table.end())
				{
					std::list<Item> list;
					list.push_back(g);
					temp_table.insert(MK_PAIR(symbol, list));
				}
				else
				{
					it->second.push_back(g);
				}
			}
		}
		for (auto& i : now.items)
		{
			if (!i.fullyExtended)
			{
				std::string symbol = GetDotSymbol(i);
				auto it = temp_table.find(symbol);
				if (it == temp_table.end())
				{
					std::list<Item> list;
					list.push_back(i);
					temp_table.insert(MK_PAIR(symbol, list));
				}
				else
				{
					it->second.push_back(i);
				}
			}
		}
		if (temp_table.size())
		{
			for (auto& [key, list] : temp_table)
			{
				ItemSet itemSet;
				for (auto item : list)
				{
					itemSet.generator.insert(GetNxt(item));
				}
				Extend(itemSet);
				auto it = in.find(GetUUID(itemSet));
				if (it != in.end())
				{
					GO[MK_PAIR(index, key)] = it->second;
				}
				else
				{
					GO[MK_PAIR(index, key)] = cnt;
					in[GetUUID(itemSet)] = cnt;
					I.insert(MK_PAIR(cnt++, itemSet));
				}
			}
		}
		index++;
	}
}

void Grammar::PrintGO() const
{
	for (auto& [key, value] : GO)
	{
		std::cout << "go(" << key.first << "," << key.second << ")=" << value << std::endl;
	}
}

void Grammar::PrintI() const
{
	for (auto i : I)
	{
		std::cout << "I_" << i.first << "=" << GetUUID(i.second) << std::endl;
	}
}

void Grammar::GenerateFirstAndFollow()
{
	for (auto p : productions)
	{
		std::string firstSymbol = p.second.GetSymbol(0);
		if (IsTerminal(firstSymbol))
		{
			first[p.second.head].insert(firstSymbol);
		}
	}

	bool added = false;
	do
	{
		added = false;
		for (auto p : productions)
		{
			std::string firstSymbol = p.second.GetSymbol(0);
			if (IsNonTerminal(firstSymbol))
			{
				int sizeBefore = first[p.second.head].size();
				first[p.second.head].insert(first[firstSymbol].begin(),first[firstSymbol].end());
				if (sizeBefore != first[p.second.head].size())
				{
					added = true;
				}
			}
		}
	} while (added);

	follow["$$$$$$$$$$$$$$$$"].insert("################");
	for (auto p : productions)
	{
		int length = p.second.GetLength();
		for (int i = 0; i < length - 1; i++)
		{
			std::string symbol = p.second.GetSymbol(i);
			std::string nxtSymbol = p.second.GetSymbol(i + 1);
			if (IsNonTerminal(symbol) && IsTerminal(nxtSymbol))
			{
				follow[symbol].insert(nxtSymbol);
			}
		}
	}

	do
	{
		added = false;
		for (auto p : productions)
		{
			int sizeBefore;
			int length = p.second.GetLength();
			for (int i = 0; i < length - 1; i++)
			{
				std::string symbol = p.second.GetSymbol(i);
				std::string nxtSymbol = p.second.GetSymbol(i + 1);
				if (IsNonTerminal(symbol) && IsNonTerminal(nxtSymbol))
				{
					sizeBefore = follow[symbol].size();
					follow[symbol].insert(first[nxtSymbol].begin(), first[nxtSymbol].end());
					if (sizeBefore != follow[symbol].size())
					{
						added = true;
					}
				}
			}

			std::string lastSymbol = p.second.GetSymbol(p.second.GetLength() - 1);
			if (IsNonTerminal(lastSymbol))
			{
				sizeBefore = follow[lastSymbol].size();
				follow[lastSymbol].insert(follow[p.second.head].begin(), follow[p.second.head].end());
				if (sizeBefore != follow[lastSymbol].size())
				{
					added = true;
				}
			}
		}
	} while (added);
}

void Grammar::PrintFirst() const
{
	for (auto it : first)
	{
		std::cout << "first[" << it.first << "]=" << "{";
		for (auto s : it.second)
		{
			std::cout << s << " ";
		}
		std::cout << "}" << std::endl;
	}
}

void Grammar::PrintFollow() const
{
	for (auto it : follow)
	{
		std::cout << "follow[" << it.first << "]=" << "{";
		for (auto s : it.second)
		{
			std::cout << s << " ";
		}
		std::cout << "}" << std::endl;
	}
}