#pragma once
#include "Graph.h"
#include <string>
#include <stack>
#include <map>
using namespace std;
class InfixToPostfix
{
public:
	InfixToPostfix(void);
	string convert(string Infix);
	int peek();
	std::stack<char> stack;
	~InfixToPostfix(void);
};

