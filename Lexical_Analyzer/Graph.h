#pragma once
#include "Node.h"
class Graph
{
public:
	Graph(void);
	Graph(Node* a, Node* b);
	Node* start;
	Node* end;
	~Graph(void);
};

