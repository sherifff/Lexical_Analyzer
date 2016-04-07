#pragma once
#include <set>
#include"Graph.h"
#include<list>
#include<set>
#include"Node.h"
#include"Partition.h"
#include <vector>
using namespace std;
class DFAGenerator
{
public:
	DFAGenerator(Graph NFA, std::list<char> givenInput);
	DFAGenerator();
	~DFAGenerator(void);
	Graph NFA;
	int numOfStates;
	std::list<State>states;
	//////////////////////////////////////////////////
     int** generateMinDFA();
     std::list<State> minimizedStates;
	 State getStateOfIndex(int indexOfState);
	 std::list<char> inputs;
	 int** dTransTable;
	 int** resultMinDFATransTable;
	std::list<list<int> > minDTranTable;
	 int numberOfTargetGroup(int targetNodeInPartition,Partition partition);
     bool equalPartitions(Partition p1, Partition p2);
	 bool inSameGroup(int node1, int node2, Partition partition);
	/////////////////////////////////////////////////
	void subsetConstruction();
	//list<Node>getPath(char a, Node node);
	list<Node*> getPath(char a, Node*node);
	list<Node*>pointer(list<Node>generate);
   static bool fncompppp (Node lhs, Node  rhs);
     static bool fncompppp2 (Node* lhs, Node*  rhs);
	std::set<Node> move(Node T,Node A);
	static bool uniqueee(Node lhs , Node rhs );
		static bool uniqueee2(Node *lhs , Node *rhs );
	std::list< Node*> ebsilonClosure(Node *S);
 
};

