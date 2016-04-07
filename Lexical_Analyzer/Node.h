#pragma once
#include <list>
#include "Edge.h"
#include "State.h"
#include <map>
#include <vector>

class Node
{
public:
	Node(void);
	std::map<char,std::vector<Node*>> edges;
	State state;
	int nodeNum;
	static int StateCounter;
	~Node(void);
};