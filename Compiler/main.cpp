#include "pch.h"
#include "Grammar.h"

void TestClosure()
{
    std::set<std::string> terminal{ "a","b","c","d" };
    std::set<std::string> nonterminal{ "S","A","B" };
    std::vector<std::vector<std::string>> items =
        { { "A", "B" },
        { "c", "B", "d"},
        { "c", "d" },
        { "a", "A", "b"},
        { "a", "b" } 
    };
    std::vector<std::vector<std::string>> temp;
    std::map<std::string, std::vector<std::vector<std::string>>> productions;
    temp.assign(items.begin(), items.begin() + 1);
    productions.insert(MK_PAIR("S", temp));
    temp.assign(items.begin() + 1, items.begin() + 3);
    productions.insert(MK_PAIR("B", temp));
    temp.assign(items.begin() + 3, items.end());
    productions.insert(MK_PAIR("A", temp));

    S_PTR(Grammar, grammar) = MK_SPTR(Grammar, "S", terminal, nonterminal, productions);
    ItemSet closure = grammar->GetClosure();
    grammar->PrintItemSet(closure);
    grammar->GenerateGOandI(closure);
    std::cout << "GO:" << std::endl;
    grammar->PrintGO();
    std::cout << "I:" << std::endl;
    grammar->PrintI();
}

int main(int, char**) 
{
    TestClosure();
    std::cout << "Hello, world!\n";
}
