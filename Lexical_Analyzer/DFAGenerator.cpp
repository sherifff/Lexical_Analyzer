#include "stdafx.h"
#include "DFAGenerator.h"
#include "Graph.h"
#include "Node.h"
#include "Partition.h"
#include <stack>
#include <list>
#include <iterator>
#include <iostream>
#include <vector>
using namespace std;

DFAGenerator::DFAGenerator(Graph givenNFA, std::list<char> givenInput) {
	// TODO Auto-generated constructor stub
	NFA = givenNFA;
	inputs = givenInput;
}
DFAGenerator::DFAGenerator(){
}
bool DFAGenerator::fncompppp (Node lhs, Node rhs) {return lhs.nodeNum< rhs.nodeNum;}

bool DFAGenerator::uniqueee(Node lhs, Node rhs) {return lhs.nodeNum== rhs.nodeNum;}

bool DFAGenerator::fncompppp2 (Node *lhs, Node* rhs) {return lhs->nodeNum< rhs->nodeNum;}

bool DFAGenerator::uniqueee2(Node *lhs, Node *rhs) {return lhs->nodeNum== rhs->nodeNum;}

std::list<Node*> DFAGenerator::ebsilonClosure(Node *node) {

	list<Node*> resultNodes  ;
	std::stack<Node*> nodesStack;
	nodesStack.push(node);

	while (!nodesStack.empty()) {

		Node * poppedNode = nodesStack.top();
		nodesStack.pop();
		bool flag = false;

      	std::list<Node*>::const_iterator iterator;
      for (iterator = resultNodes.begin(); iterator != resultNodes.end(); ++iterator) {
			Node* resultNode = *iterator;
		  if (poppedNode->nodeNum == resultNode->nodeNum) {
			flag = true;
				break;
			}
		}
		

		if (!flag) {		
			resultNodes.push_back(poppedNode);
			std::vector<Node*>reach;
			if(poppedNode->edges.find(0) != poppedNode->edges.end()){
			reach=poppedNode->edges.at(0);
			for (std::vector<Node*>::iterator it = reach.begin() ; it != reach.end(); ++it){
				Node*l=*it;
				nodesStack.push(l);
			}
		   }
		}
		
	}
	resultNodes.sort(fncompppp2);
    resultNodes.unique(uniqueee2);
	return resultNodes;

	}
/*
list<Node> DFAGenerator::getPath(char a, Node node){
     list<Node>result;
	 std::vector<Node>reach;
			if(node.edges.find(a) != node.edges.end()){
			reach=node.edges.at(a);
			for (std::vector<Node>::iterator it = reach.begin() ; it != reach.end(); ++it){
				Node l=*it;
				list<Node>result2;
				result2=ebsilonClosure(l);
				for (std::list<Node>::iterator it2 = result2.begin() ; it2 != result2.end(); ++it2){
					Node w=*it2;
					result.push_back(w);
				}
			}
			result.unique(uniqueee);
			result.sort(fncompppp);
			}
			return result;
}
*/
list<Node*> DFAGenerator::getPath(char a, Node* node){
     list<Node*>result;
	 std::vector<Node*>reach;
			if(node->edges.find(a) != node->edges.end()){
			reach=node->edges.at(a);
			for (std::vector<Node*>::iterator it = reach.begin() ; it != reach.end(); ++it){
				Node*l=*it;
				//list<Node>result2;
				list<Node*>result3;
				result3=ebsilonClosure(l);
				//result3=pointer(result2);
				for (std::list<Node*>::iterator it2 = result3.begin() ; it2 != result3.end(); ++it2){
					Node* w=new Node();
						w=*it2;
					result.push_back(w);
				}
			}
			result.sort(fncompppp2);
			result.unique(uniqueee2);
			}
			return result;
}
list<Node*>DFAGenerator::pointer(list<Node>generate){
		list<Node*> generate2;
	for (std::list<Node>::iterator it2 = generate.begin() ; it2 != generate.end(); ++it2){
		Node y= *it2;
		Node *r=new Node();
		r->edges=y.edges;
		r->nodeNum=y.nodeNum;
		generate2.push_back(r);
	}
	return generate2;
}

void DFAGenerator::subsetConstruction(){
    map <list<Node*>,int> statesmap;
	map <int,list<Node*> > get;
	int input_index=0;
	numOfStates=0;
	int rows=0;
	int countt=Node::StateCounter;
	dTransTable=new int*[countt];
	int i=0;
	int ww=inputs.size();
	for(i=0;i<countt;i++){
		dTransTable[i]=new int[inputs.size()];
	}
	Node *start=NFA.start;
	//list<Node> generate;
    list<Node*> generate2;
	generate2=ebsilonClosure(start);
	//generate2=pointer(generate);
	statesmap[generate2]=numOfStates;
	get[rows]=generate2;
	numOfStates++;
	State f=State(false,"");
	 int priority2=-1;
	for (std::list<Node*>::iterator itq = generate2.begin() ; itq != generate2.end(); ++itq){
		Node * w=*itq;
		if(w->state.accepted==true){
			if(priority2==-1||w->state.priority<priority2){
			f.accepted=true;
			f.token=w->state.token;
			priority2=w->state.priority;
			}
		}
	}
	states.push_back(f);
	while(rows<numOfStates){
	      list<Node*> generate3;
		  input_index=0;
		  generate3=get.at(rows);
		for (std::list<char>::iterator it = inputs.begin() ; it != inputs.end(); ++it){
           char inp=*it;
		   list<Node*>result;
		   // INITALIZE the state of group
		   State s(false,"");
		   int priority=-1;
		   for (std::list<Node*>::iterator it2 = generate3.begin() ; it2 != generate3.end(); ++it2){
			   Node * r=new Node();
				   r =*it2;
			   //list<Node>one;
			   list<Node*>one2;
			   one2 = getPath(inp,r);
			   if(one2.size()!=0){
			  // one2 = pointer(one);
			     for (std::list<Node*>::iterator it3 = one2.begin() ; it3 != one2.end(); ++it3){
					 Node *re=*it3;
					 result.push_back(re);
					 //sompare the prority of the nodes and set the higher one.
					 if((re->state).accepted==true){
						 //compare
						 if(priority==-1||re->state.priority<priority){
						 s.accepted=true;
						 s.token=re->state.token;
						 priority=re->state.priority;
						 }
					 }
				 }
			   }
		   }
		   if(result.size()!=0){
		   result.sort(fncompppp2);
		   result.unique(uniqueee2);
		   if(statesmap.find(result) == statesmap.end()){
			   statesmap[result]=numOfStates;
			   get[numOfStates]=result;
			   //add in the list states 
			   states.push_back(s);
			   numOfStates++;
			   dTransTable[rows][input_index]=numOfStates-1;
		   }
		   else{
			   int t=0;
			   t=statesmap.at(result);
			   dTransTable[rows][input_index]=t;
		   }
		   }else{
              dTransTable[rows][input_index]=-1;
		   }

		   input_index++;
		}
		rows++;
	}
}
DFAGenerator::~DFAGenerator(void)
{}


/////////////////////sikooo///////////////////////////////////////////////
int** DFAGenerator::generateMinDFA() {

   Partition generatedInitialPartition;
   std::set<int> finalStates;
   std::set<int> notFinalStates;
             /* 
   			std::list<bool>::const_iterator iteratorOnBool;

			int nodeCount = 0;
			for (iteratorOnBool = containsAccState.begin(); iteratorOnBool != containsAccState.end();
				++iteratorOnBool) {
					if(*iteratorOnBool){ // if acceptance state 
						finalStates.insert(nodeCount);
						nodeCount++;
					}else{ // if not acceptance state
					notFinalStates.insert(nodeCount);
						nodeCount++;
					}
			}
			*/

             std::list<State>::const_iterator iteratorOnStates;

			int nodeCount = 0;
			for (iteratorOnStates = states.begin(); iteratorOnStates != states.end();
				++iteratorOnStates) {
					if((*iteratorOnStates).accepted){ // if acceptance state 
						finalStates.insert(nodeCount);
						nodeCount++;
					}else{ // if not acceptance state
					notFinalStates.insert(nodeCount);
						nodeCount++;
					}
			}

			generatedInitialPartition.groups.insert(notFinalStates);
			generatedInitialPartition.groups.insert(finalStates);

			Partition newPartition, generatedPartition;
			 
			
				bool started = false;
				generatedPartition = generatedInitialPartition;


///////////////////////PRINTING ///////////////////////////////

				/*
				std::set<set<int> >::const_iterator iteratorOnGroupsPrinter;
				for (iteratorOnGroupsPrinter = generatedPartition.groups.begin(); iteratorOnGroupsPrinter != generatedPartition.groups.end();
				++iteratorOnGroupsPrinter) {
					
					set<int> groupOfNodes = *iteratorOnGroupsPrinter;
					
					// Iteration on nodes in group.
					std::set<int>::const_iterator iteratorOnNodes;	  
				for (iteratorOnNodes = groupOfNodes.begin(); iteratorOnNodes != groupOfNodes.end();
				++iteratorOnNodes) {
				    
				cout << *iteratorOnNodes;
				}cout << " " << endl; 
			} */
////////////////////////////////////////////////////
			
				while(!equalPartitions(newPartition,generatedPartition)){
				
				if(!started)
				  started = true;
			      else {
					generatedPartition=newPartition;
					newPartition.groups.clear();
				 }
				
  
			// Iteration on groups of the partition.
			std::set<set<int> >::const_iterator iteratorOnGroups;
			for (iteratorOnGroups = generatedPartition.groups.begin(); iteratorOnGroups != generatedPartition.groups.end();
				++iteratorOnGroups) {
					
					set<int> groupOfNodes = *iteratorOnGroups;
					
					// Iteration on nodes in group.
					std::set<int>::const_iterator iteratorOnNodes;	  
				for (iteratorOnNodes = groupOfNodes.begin(); iteratorOnNodes != groupOfNodes.end();
				++iteratorOnNodes) {
				    
					set<int> newGroup;
					int nodeToBeComparedToOthers = *iteratorOnNodes;
						newGroup.insert(nodeToBeComparedToOthers);

				 ///////////////////////////////////////////////////////////////////////////////////////////////
					// iteration on nodes again.
			       		
					   	std::set<int>::const_iterator iteratorOnNodesAgain;	  
				        for (iteratorOnNodesAgain = groupOfNodes.begin(); iteratorOnNodesAgain != groupOfNodes.end();
				        ++iteratorOnNodesAgain) {
						
				            	int node = *iteratorOnNodesAgain;
				                bool sameGroup = true;
								int i;
								for (i = 0; i < inputs.size(); i++)
					            {	
									if(!inSameGroup(dTransTable[nodeToBeComparedToOthers][i],dTransTable[node][i], generatedPartition))
									{
								      sameGroup = false;
									  break;
									}
				                }
								if(sameGroup)
									newGroup.insert(node);
						 }
						
                 ///////////////////////////////////////////////////////////////////////////////////////////////

						newPartition.groups.insert(newGroup);
			      }
			 
  			  }
			
			}
////////////////////////////////newPartition is the final partition/////////////////////////////////////////////////

			/*/ Iteration on groups of the partition.
			std::set<set<int> >::const_iterator iteratorOnGroupsPrinter;
			for (iteratorOnGroupsPrinter = newPartition.groups.begin(); iteratorOnGroupsPrinter != newPartition.groups.end();
				++iteratorOnGroupsPrinter) {
					
					set<int> groupOfNodes = *iteratorOnGroupsPrinter;
					
					// Iteration on nodes in group.
					std::set<int>::const_iterator iteratorOnNodes;	  
				for (iteratorOnNodes = groupOfNodes.begin(); iteratorOnNodes != groupOfNodes.end();
				++iteratorOnNodes) {
				    
				cout << *iteratorOnNodes;
				}cout << " " << endl; 
			}  		*/
////////////////////////////////generating the minimum transitionTable/////////////////////////////////////
				
				int ** resultMinDFATransTable = new int*[newPartition.groups.size()];
			int i;
			for (i = 0; i < newPartition.groups.size(); i++)
				resultMinDFATransTable[i]= new int[inputs.size()];

			int newNodeIndex = 0;
			std::set<set<int> >::const_iterator iteratorOnGroups;
			for (iteratorOnGroups = newPartition.groups.begin(); iteratorOnGroups != newPartition.groups.end();
				++iteratorOnGroups) {
					
					set<int> groupOfNodes = *iteratorOnGroups;

					bool finalStateFound = false; // for the new list of states.
					string tokenOfFinalState = "";
					// Iteration on nodes in group.
					std::set<int>::const_iterator iteratorOnNodes;	  
				for (iteratorOnNodes = groupOfNodes.begin(); iteratorOnNodes != groupOfNodes.end();
				++iteratorOnNodes) {
				    
					int node = *iteratorOnNodes;

         			/////////////Generating the new state of group//////////////

					State stateOfNode = getStateOfIndex(node);	
					if(!finalStateFound&&stateOfNode.accepted == true){
						finalStateFound=true;
						tokenOfFinalState = stateOfNode.token;
					}
					///////////////////////////////////////////////////////////

					int i;
					for (i = 0; i < inputs.size(); i++)	
                       resultMinDFATransTable[newNodeIndex][i]=numberOfTargetGroup(dTransTable[node][i],newPartition);	    
			
					break;
			      	
				}
				minimizedStates.push_back(State(finalStateFound,tokenOfFinalState));
				newNodeIndex++;
			 }
	/*				cout << "" << endl;
	for (int i = 0; i < 4; i++){
	  for (int j = 0; j < 2; j++)
		{
			cout << resultMinDFATransTable[i][j];
		} cout << "" << endl;} 
		*/
/////////////////////////////////////Generating the new list of states////////////////////////////////////
			


//////////////////////////////////////////////////////////////////////////////////////////////////////////
			return resultMinDFATransTable;
}

State DFAGenerator::getStateOfIndex(int nodeIndex){
  	int indexOfState = 0;
	std::list<State>::const_iterator iteratorOnStates;	  
			for (iteratorOnStates = states.begin(); iteratorOnStates != states.end();
					++iteratorOnStates) {
		        if(indexOfState == nodeIndex)
					return *iteratorOnStates;
				indexOfState++;
				}
}


bool DFAGenerator::inSameGroup(int node1, int node2, Partition partition)
{
	int testIndex = 0;
	std::set<set<int> >::const_iterator iteratorOnGroups;
	for (iteratorOnGroups = partition.groups.begin(); iteratorOnGroups != partition.groups.end();
				++iteratorOnGroups) {

					bool node1Found=false, node2Found = false;
					set<int> groupOfNodes = *iteratorOnGroups;	
					// Iteration on nodes in group.
						std::set<int>::const_iterator iteratorOnNodes;	  
				for (iteratorOnNodes = groupOfNodes.begin(); iteratorOnNodes != groupOfNodes.end();
				++iteratorOnNodes) {
					
				  if(*iteratorOnNodes==node1&&*iteratorOnNodes==node2){
				  node1Found = true;
				  node2Found = true;
				  } else if(*iteratorOnNodes==node1)
					  node1Found = true;
				  else if(*iteratorOnNodes==node2)
					  node2Found = true;
				  
				}
				if(node1Found&&node2Found)
					return true;		
			testIndex++;}

	return false;
}

bool DFAGenerator::equalPartitions(Partition p1, Partition p2){
	if((p2.groups.size()==0||p1.groups.size()==0)||(p1.groups.size()!=p2.groups.size()))
		return false;
	if(p1.groups!=p2.groups)return false;

	/*
	int group1Index = 0;
	std::set<set<int> >::const_iterator iteratorOnGroups;
	for (iteratorOnGroups = p1.groups.begin(); iteratorOnGroups != p1.groups.end();
				++iteratorOnGroups) {
				
					int group2Index = 0;
              	std::set<set<int> >::const_iterator iteratorOnGroups2;
	for (iteratorOnGroups2 = p2.groups.begin(); iteratorOnGroups2 != p2.groups.end();
				++iteratorOnGroups2) {
					
					if(group1Index==group2Index&&(*iteratorOnGroups)!=(*iteratorOnGroups2))
						return false;
					if(group2Index>group1Index)
						break;
					group2Index++;
		cout << "hey";
			}
	group2Index++;
	} */

return true;
}

int DFAGenerator::numberOfTargetGroup(int targetNodeInPartition, Partition partition)
{
	             int indexOfGroup = 0;
			std::set<set<int> >::const_iterator iteratorOnGroups;
			for (iteratorOnGroups = partition.groups.begin(); iteratorOnGroups != partition.groups.end();
				++iteratorOnGroups) {
					
					set<int> groupOfNodes = *iteratorOnGroups;

					// Iteration on nodes in group.
					std::set<int>::const_iterator iteratorOnNodes;	  
				for (iteratorOnNodes = groupOfNodes.begin(); iteratorOnNodes != groupOfNodes.end();
				++iteratorOnNodes) {
				    
					int node = *iteratorOnNodes;
					if(node==targetNodeInPartition)
						return indexOfGroup;
			      	}
				indexOfGroup++;
			 }
			return 0;
}



