#pragma once

class Item
{
public:
    Item(int dotPlace, std::string production,bool fullyExtended);
public:
    int dotPlace;
    std::string production;
    bool fullyExtended;

public:
    bool operator==(const Item& b) const;

    bool operator<(const Item& b) const;
};

class ItemSet
{
    friend class Grammar;

private:
    std::set<Item> generator;
    std::set<Item> items;

public:
    bool operator==(const ItemSet& b) const;
};

class Production
{
private:
    std::string head;
    std::vector<std::string> body;
    int length;

public:
    Production(std::string head, std::vector<std::string> body);

    Production(std::vector<std::string> items);

    std::string ToString() const;
    
    Item GetFirstCanonnialCollection() const;

    std::string GetSymbol(int index) const;

    std::string GetHead() const;

    int GetLength() const;
};

class Grammar
{
public:

    Grammar(std::string start, std::set<std::string> terminal, std::set<std::string> nonterminal, std::map<std::string, std::vector<std::vector<std::string>>> productionsTable);

public:

    ItemSet GetClosure();

    void GenerateGOandI(ItemSet closure);

public:

    void PrintAllProduction() const;

    void PrintItemSet(ItemSet itemSet) const;

    void PrintGO() const;

    void PrintI() const;

private:
    bool IsTerminal(std::string symbol) const;

    bool IsNonTerminal(std::string symbol) const;

    Production GetProduction(std::string key) const;

    std::string GetDotSymbol(Item item) const;

    Item GetNxt(Item item) const;

    std::string GetUUID(Item item) const;

    std::string GetUUID(ItemSet itemSet) const;

    void Extend(ItemSet &itemSet) const;

private:
    
    std::string start;
    
    std::set<std::string> terminal;
    
    std::set<std::string> nonterminal;
    
    std::map<std::string, std::vector<std::string>> productionsTable;

    std::map<std::string, Production> productions;

    std::map<std::pair<int, std::string>,int> GO;

    std::map<int, ItemSet> I;
};