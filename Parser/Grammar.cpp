#include "pch.h"
#include "Grammar.h"

bool Item::operator<(const Item& b) const
{
	return GetUUID() < b.GetUUID();
}

Production::Production(std::string head, std::vector<std::string> body)
	:head(head),body(body)
{

}

Production::Production(std::vector<std::string> items)
{
	auto it = items.begin();
	head = *it;
	body.assign(it + 1, items.end());
}

Grammar::Grammar(std::string start, std::set<std::string> terminal, std::set<std::string> nonterminal, std::map<std::string, std::vector<Production>> productions)
	:start(start),terminal(terminal),nonterminal(nonterminal),productions(productions)
{

}

std::shared_ptr<std::map<std::string, Item>> Grammar::GetClosure()
{
	std::shared_ptr<std::map<std::string, Item>> closure = std::make_shared<std::map<std::string, Item>>();
	Item start{ 0,std::string("$$$$$$$$"), std::vector<std::string>{this->start} };
	std::shared_ptr<std::map<std::string, Item>> delta = std::make_shared<std::map<std::string, Item>>();
	closure->insert(MK_PAIR(start.GetUUID(), start));
	do
	{
		closure->insert(delta->begin(), delta->end());
		delta->clear();

		for (auto collection_pair : *closure)
		{
			Item collection = collection_pair.second;
			std::string symbol = collection.GetItem();
			if (collection.Fully_Extended())
			{
				continue;
			}
			if (IsNonTerminal(symbol))
			{
				std::vector<Production> productions = this->productions[symbol];
				for (auto production : productions)
				{
					Item newCollection = production.GetFirstCanonnialCollection();
					if (closure->find(newCollection.GetUUID()) == closure->end() && delta->find(newCollection.GetUUID()) == delta->end())
					{
						delta->insert(MK_PAIR(newCollection.GetUUID(), newCollection));
					}
				}
			}
		}
	} while (delta->size());

	canonnialCollections = std::move(closure);
	return std::move(closure);
}