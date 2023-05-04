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

class Production
{

    friend class Grammar;
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

class ItemSet
{
    friend class Grammar;

private:
    std::set<Item> generator;
    std::set<Item> items;

public:
    bool Contains(const Item& Item) const;

    std::set<Item> GetAllItem() const;

public:
    bool operator==(const ItemSet& b) const;
};

class Grammar
{
public:

    Grammar(std::string start, std::set<std::string> terminal, std::set<std::string> nonterminal, std::map<std::string, std::vector<std::vector<std::string>>> productionsTable);

public:

    ItemSet GetClosure();

    void GenerateGOandI(ItemSet closure);

    void GenerateFirstAndFollow();

public:

    void PrintAllProduction() const;

    void PrintItemSet(ItemSet itemSet) const;

    void PrintGO() const;

    void PrintI() const;

    void PrintFirst() const;

    void PrintFollow() const;

private:

    Production GetProduction(std::string key) const;

    std::string GetDotSymbol(Item item) const;

    Item GetNxt(Item item) const;

    std::string GetUUID(Item item) const;

    std::string GetUUID(ItemSet itemSet) const;

    void Extend(ItemSet &itemSet) const;

public:
    std::map<std::string, std::set<std::string>> GetFollow() const;

    std::map<std::pair<int, std::string>, int> GetGO() const;

    bool IsTerminal(std::string symbol) const;

    bool IsNonTerminal(std::string symbol) const;

    std::map<int, ItemSet> GetI() const;

    std::string GetStart() const;

private:
    
    std::string start;
    
    std::set<std::string> terminal;
    
    std::set<std::string> nonterminal;

    std::map<std::string, std::set<std::string>> follow;

    std::map<std::string, std::set<std::string>> first;
    
    std::map<std::string, std::vector<std::string>> productionsTable;

    std::map<std::string, Production> productions;

    std::map<std::pair<int, std::string>,int> GO;

    std::map<int, ItemSet> I;
};