#include "pch.h"
#include "Grammar.h"

void TestClosure()
{
    std::set<std::string> terminal{ "a","b","c","d" };
    std::set<std::string> nonterminal{ "S","A","B" };
    std::vector<std::vector<std::string>> items =
        { { "S", "A", "B" },
        { "B", "c", "B", "d"},
        { "B", "c", "d" },
        { "A", "a", "A", "b"},
        { "A", "a", "b" } 
    };
    std::map<std::string, std::vector<Production>> productions;
    std::vector<Production> p0 = std::vector<Production>{ Production(items[0]) };
    std::vector<Production> p1 = std::vector<Production>{ Production(items[1]),Production(items[2]) };
    std::vector<Production> p2 = std::vector<Production>{ Production(items[3]),Production(items[4]) };
    productions.insert(MK_PAIR("S", p0));
    productions.insert(MK_PAIR("B", p1));
    productions.insert(MK_PAIR("A", p2));
    S_PTR(Grammar, grammar) = MK_SPTR(Grammar, "S", terminal, nonterminal, productions);
    grammar->PrintAllProduction();
    grammar->GetClosure();
    grammar->PrintAllCanonnialCollections();
}

int main(int, char**) 
{
    TestClosure();
    std::cout << "Hello, world!\n";
}
