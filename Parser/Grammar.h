#pragma once

class Item
{
public:
    int dotPlace;
    std::string head;
    std::vector<std::string> body;

public:

    bool operator<(const Item& b) const;

public:
    std::string GetItem() const
    {
        return body[dotPlace];
    }

    bool Fully_Extended() const
    {
        return body.size() <= dotPlace;
    }

    Item GetNxt() const
    {
        return Item{ dotPlace + 1,head,body };
    }

    std::string GetUUID() const
    {
        S_PTR(std::stringstream, sstream) = MK_SPTR(std::stringstream, );
        *sstream << head << "->";
        for (int i = 0; i < body.size(); i++)
        {
            if (dotPlace == i)
            {
                *sstream << ".";
            }
            *sstream << body[i];
        }

        return sstream->str();
    }
};

class ItemSet
{
};

class Production
{
private:
    std::string head;
    std::vector<std::string> body;

public:
    Production(std::string head, std::vector<std::string> body);

    Production(std::vector<std::string> items);

    std::string ToString() const 
    { 
        S_PTR(std::stringstream, sstream) = MK_SPTR(std::stringstream, );
        *sstream << head << "->";
        for (auto item : body)
        {
            *sstream << item;
        }
        return sstream->str();
    }
    
    Item GetFirstCanonnialCollection() const
    {
        return Item{ 0,head,body };
    }
};

class Grammar
{
public:

    Grammar(std::string start, std::set<std::string> terminal, std::set<std::string> nonterminal, std::map<std::string, std::vector<Production>> productions);

public:

    std::shared_ptr<std::map<std::string,Item>> GetClosure();

    void PrintAllProduction() const
    {
        for (auto p : productions)
        {
            for (auto pp : p.second)
            {
                std::cout << pp.ToString() << std::endl;
            }
        }
    }

    void PrintAllCanonnialCollections() const
    {
        for (auto p : *canonnialCollections)
        {
            std::cout << p.second.GetUUID() << std::endl;
        }
    }

private:
    bool IsTerminal(std::string symbol) const
    {
        return terminal.find(symbol) != terminal.end();
    }

    bool IsNonTerminal(std::string symbol) const
    {
        return nonterminal.find(symbol) != nonterminal.end();
    }

public:
    
    std::string start;
    
    std::set<std::string> terminal;
    
    std::set<std::string> nonterminal;
    
    std::map<std::string, std::vector<Production>> productions;

private:

    std::shared_ptr<std::map<std::string, Item>> canonnialCollections;
};