#pragma once
#include <map>
#include <string>
#include "Graph.h"
#include <iostream>
#include <vector>
#include "InfixToPostfix.h"
#include <unordered_set>
#include <list>
using namespace std;

class NFAGenerator
{
public:
	NFAGenerator(string fileName);
	vector<Graph> regEx;
	map<string,Graph> regDef;
	void getNFA();
	FILE* languageFile;
	bool languageFileValid;
	list<char> givenInputs;
	Graph NFA;
	map<int,Node*> visited;
	InfixToPostfix itp;
	void combine();
	Graph evaluate(string postFix);
	void parseLine(string line);
	string getLine(FILE * file);
	Node* traverseGraph(Node* n,const int endNumber, int from);
	Node* NFAGenerator::copyGraph(Node* n,const int endNumber, int from);
	void createKeywordGraph(vector<string> str);
	void createPunctuationGraph(vector<string> str);
	Node* copy(Node* node);
	Graph NFAGenerator::createRangeGraph(char from,char to);
	Node* end;
	~NFAGenerator(void);
};

