#include "pch.h"
#include "Grammar.h"
#include "Parser.h"

void PrintProduction(void* args)
{
    std::cout <<"Reduce:" << (char*)args << std::endl;
}

S_PTR(Grammar) GenerateGrammar()
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

    S_PTR(Grammar, grammar) = MK_SPTR(Grammar, "S", terminal, nonterminal);

    for (int i = 0; i < items.size(); i++)
    {
        Production p(items[i]);
        p.BindSubFunc(PrintProduction);
        grammar->AddProduction(p);
    }

    ItemSet closure = grammar->GetClosure();
    grammar->PrintItemSet(closure);
    grammar->GenerateGOandI(closure);
    std::cout << "GO:" << std::endl;
    grammar->PrintGO();
    std::cout << "I:" << std::endl;
    grammar->PrintI();
    grammar->GenerateFirstAndFollow();
    std::cout << "First:" << std::endl;
    grammar->PrintFirst();
    std::cout << "Follow:" << std::endl;
    grammar->PrintFollow();

    return grammar;
}

S_PTR(Parser) GenerateAnalyseTable(S_PTR(Grammar) grammar)
{
    S_PTR(Parser, parser) = MK_SPTR(Parser,grammar);
    parser->GenerateAnalyseTable();
    parser->PrintAnalyseTable(18);
    return parser;
}

void Parsing(S_PTR(Parser,parser))
{
    std::vector<std::string> words{ "a","a","b","b","c","c","d","d" };
    S_PTR(std::vector<std::string>, pWords) = MK_SPTR(std::vector<std::string>,words);
    parser->Parsing(pWords);
}

int main(int, char**) 
{
    S_PTR(Grammar,grammar) = GenerateGrammar();
    std::cout << "\nGenerate Grammar Successfully!\n";
    S_PTR(Parser,parser) = GenerateAnalyseTable(grammar);
    Parsing(parser);
}
