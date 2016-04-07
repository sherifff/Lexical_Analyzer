#pragma once
#include "Symbol.h"
#include <map>
#include <vector>

class LL1GrammarConversionTool
{
public:
	LL1GrammarConversionTool(void);
	void removeLeftRecursion(map<string,vector<vector<Symbol>>> * CFG, vector<string>* nonTerminal); // clear, it edits the map whose pointer is in the parameters
	void LeftFactoring(map<string,vector<vector<Symbol>>>*cfg,vector<string> *nonTerminal); // clear, it edits the map whose pointer is in the parameters
	int getNumOfsame(vector<vector<Symbol>>r);
	bool vectorContains(vector<string> v, string search);
	void putEpsilon(vector<vector<Symbol>>* v);
    vector<vector<Symbol>>  newNonTerminalVector(vector<vector<Symbol>>r,int num);
	void LL1GrammarConversionTool::printMap(map<string,vector<vector<Symbol>>>*CFG);
	~LL1GrammarConversionTool(void);
};