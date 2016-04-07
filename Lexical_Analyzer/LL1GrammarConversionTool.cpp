#include "stdafx.h"
#include "LL1GrammarConversionTool.h"
#include <iostream>
#include <stack>

LL1GrammarConversionTool::LL1GrammarConversionTool(void)
{
}


LL1GrammarConversionTool::~LL1GrammarConversionTool(void)
{
}


void LL1GrammarConversionTool::putEpsilon(vector<vector<Symbol>>* v){
	for(int i = 0 ; i < v->size() ; i++){
		if(v->at(i).at(0).sym == "\\L"){
			return;
		}
	}
	vector<Symbol> vs;
	Symbol s("\\L",true);
	vs.push_back(s);
	v->push_back(vs);
}

int LL1GrammarConversionTool:: getNumOfsame(vector<vector<Symbol>>r){
	int sum =0;
	for (int i = 0; i < r.at(0).size(); i++)
	{
		string e=r.at(0).at(i).sym;
	  for (int j = 1; j < r.size(); j++)
	  {
		  if(r.at(j).size()<=i){
			  return sum;
		  }
		  if(r.at(j).at(i).sym!=e){
			  return sum;
		  }
	  }
	  sum++;
	}
	return sum;
}


bool LL1GrammarConversionTool::vectorContains(vector<string> v, string search){
	for(int i = 0 ; i < v.size() ; i++){
		if(search == v.at(i)){
			return true;
		}
	}
	return false;
}

void LL1GrammarConversionTool::removeLeftRecursion(map<string,vector<vector<Symbol>>>* CFG, vector<string>* nonTerminal){

	/* 1.convert any recursion into left recursion
	loop through all of the map
	another loop through the first list
	if terminal, continue
	if non terminal, call a recursive method(first loop non terminal(i), non terminal of the second loop (j))
	if it returns an empty string, then continue
	if not replace, take this string split on space and then push front from back to end (could be the opposite)

	*/

	map<string,vector<vector<Symbol>>> newMap;
	vector<string> visited;
	map<string,vector<vector<Symbol>>>::iterator mapit;
	vector<vector<Symbol>> newVector;
	int i = 0;
	for(mapit = CFG->begin() ; mapit != CFG->end() ; mapit++){// looping through all of the map
		newVector.clear();
		for(int j = 0; j < mapit->second.size() ; j++){

			if(mapit->second.at(j)[0].isTerminal){//if terminal continue 
				newVector.push_back(mapit->second.at(j));
				continue;
			}
			else if(mapit->first == mapit->second.at(j)[0].sym){ // if left hand side == first element in right hand side, continue
				newVector.push_back(mapit->second.at(j));
				continue;
			}
			else if (vectorContains(visited,mapit->second.at(j)[0].sym)) { // if non terminal and visited before, replace this with old production rule
				// i think the safest strategy is to replace the whole old vector of vectors

				vector<vector<Symbol>> replacee = CFG->at(mapit->second.at(j)[0].sym); // here returns a vector of vectors
				for(int k = 0 ; k < replacee.size() ; k++){
					for(int m = 1 ; m < mapit->second.at(j).size() ; m++){
						replacee.at(k).push_back(mapit->second.at(j).at(m));
						// experiment with push back
					}
				}
				//CFG->at(mapit->first) = newVector;
				//loop in replacee size and push back in newVector
				for(int n = 0 ; n < replacee.size() ; n++){
					newVector.push_back(replacee.at(n));
				}
			}else{
				newVector.push_back(mapit->second.at(j));
			}

		}
		visited.push_back(mapit->first);
		CFG->at(mapit->first) = newVector;
		// put lfh non terminal in visited list 
		// modify new row in map
		// create new map to insert in it the new elements
		// 2.remove immediate recursion


		//put newElementVector in map, with name + single quote
		//CFG->insert( pair<string,vector<vector<Symbol>>>(mapit->first,newElementVector) ); // insert version 1



	}
	vector<bool> leftRecFound;
	bool leftRecFoundi;
	for(mapit = CFG->begin() ; mapit != CFG->end() ; mapit++){
		leftRecFoundi = false;
		for(int j = 0; j < mapit->second.size() ; j++){
			if(mapit->first == mapit->second.at(j)[0].sym){
				leftRecFoundi = true;
			}
		}
		leftRecFound.push_back(leftRecFoundi);
	}

	printMap(CFG);
	int l=0;
	for(mapit = CFG->begin() ; mapit != CFG->end() ; mapit++){

		if(leftRecFound.at(l)){
			vector<vector<Symbol>> newElementVector;

			for(int j = mapit->second.size() - 1 ; j >= 0 ; j--){

				if(mapit->first == mapit->second.at(j)[0].sym){
					vector<Symbol> newVector;
					Symbol s(mapit->first+'\'',false);
					for(int k = 1 ; k < mapit->second.at(j).size() ; k++){
						newVector.push_back(mapit->second.at(j).at(k));
					}
					newVector.push_back(s);
					newElementVector.push_back(newVector);
					mapit->second.erase(mapit->second.begin()+j);
				}else{
					Symbol s(mapit->first+'\'',false);
					mapit->second.at(j).push_back(s);
				}


				putEpsilon(&newElementVector);
				newMap[mapit->first+'\''] = newElementVector; // insert version 2
			}
		}
		l++;
	}
	for(mapit = newMap.begin() ; mapit != newMap.end() ; mapit++){
		CFG->insert(pair<string,vector<vector<Symbol>>>(mapit->first,mapit->second));
		nonTerminal->push_back(mapit->first);
	}


	// loop through the new map and insert it's elements in the CFG map


}

 vector<vector<Symbol>> LL1GrammarConversionTool:: newNonTerminalVector(vector<vector<Symbol>>r,int num){
	vector<vector<Symbol>>result2;
	for (int i = 0; i < r.size(); i++)
	{
		vector<Symbol>res;
		////////////////////////// handle epsooon
		for (int j =num ; j < r.at(i).size(); j++)
		{
			res.push_back(r.at(i).at(j));
		}
		if(num==r.at(i).size()){
			res.push_back(Symbol("\\L",true));
		}
		result2.push_back(res);
	}
	
	return result2;
}

void LL1GrammarConversionTool::LeftFactoring(map<string,vector<vector<Symbol>>>*cfg,vector<string> *nonTerminal)
{
	stack<string>nonTerminalFactoring;
	for (int i = nonTerminal->size()-1; i >= 0; i--)
	{
		nonTerminalFactoring.push(nonTerminal->at(i));
	}
	nonTerminal->clear();
	while(!nonTerminalFactoring.empty()){
		string nonter=nonTerminalFactoring.top();
		nonTerminal->push_back(nonter);
		string curname=nonter;
		nonTerminalFactoring.pop();
		vector<vector<Symbol>>line;
		line=cfg->at(nonter);
		bool * check;
		check=new bool[line.size()];
		for (int i = 0; i <line.size() ; i++)
		{
          check[i]=false;
		}
		vector<vector<vector<Symbol>>>result;
		for (int i = 0; i < line.size(); i++)
		{
			vector<vector<Symbol>>match;
			if(check[i]==false){
				check[i]=true;
				match.push_back(line.at(i));
				for (int j = i+1; j < line.size(); j++)
				{
					if(line.at(i).at(0).sym == line.at(j).at(0).sym){
						match.push_back(line.at(j));
						check[j]=true;
				 }
				}
				result.push_back(match);
			}

		}
		vector<vector<Symbol>>finalResult;
		for (int i = 0; i < result.size(); i++)
		{
			if(result.at(i).size()==1){
				finalResult.push_back(result.at(i).at(0));
			}
			else if(result.at(i).size()!=0){
				int num=getNumOfsame(result.at(i));
				vector<Symbol>form;
				for (int j = 0; j < num; j++)
				{
					form.push_back(result.at(i).at(0).at(j));
				}
				string name=curname+"'";
				curname=curname+"'";
				Symbol w= Symbol(name,false);
				form.push_back(w);
				finalResult.push_back(form);
				vector<vector<Symbol>>remain;
				remain=newNonTerminalVector(result.at(i),num);
				(*cfg)[name]=remain;
				//cfg->insert(pair<string,vector<vector<Symbol>>>(name,remain));
				nonTerminalFactoring.push(name);
			}

		}
		(*cfg)[nonter]=finalResult;

		//cfg->insert(pair<string,vector<vector<Symbol>>>(nonter,finalResult));
	}

}

void LL1GrammarConversionTool::printMap(map<string,vector<vector<Symbol>>>*CFG){
	map<string,vector<vector<Symbol>>>::iterator mapit;
	string print = "";
	for(mapit = CFG->begin() ; mapit != CFG->end() ; mapit++){
		print += mapit->first + "  ->  " ;
		//cout << mapit->second.size() << endl;
		for(int i = 0 ; i < mapit->second.size()-1 ; i++){
			//cout << mapit->second.at(i).size() << endl;
			for(int j = 0 ; j < mapit->second.at(i).size() ; j++){
				print+= mapit->second.at(i).at(j).sym + " " ;
			}
			print += " | " ;
		}
		for(int j = 0 ; j < mapit->second.at(mapit->second.size()-1).size() ; j++){
				print+= mapit->second.at(mapit->second.size()-1).at(j).sym + " " ;
			}
		cout << endl << endl << print << endl << endl;
		print = "";
	}

	cout << "____________________________________________" << endl;
}