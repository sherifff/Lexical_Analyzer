#include "stdafx.h"
#include "SyntaxAnalyzer.h"
#include <algorithm>
#include <iostream>
#include "Node.h"
#include "InfixToPostfix.h"
#include "Graph.h"
#include "File_System.h"
#include "NFAGenerator.h"
#include "DFAGenerator.h"
#include <vector>
using namespace std;
	int indexOfLastCharOfLastAccState;
	int j=0;
	int sourceNode = 0, targetNode=-1;
	string lineStr, inputJavaFile = "";
    char line[1000];
	State lastAcceptedState(false,"");
	map<char,int> inputToItsIndex;
	int** minDFATransTable;
	DFAGenerator dfaGenerator ;
State getMinStateOfIndex(int nodeIndex, std::list<State> listOfMinStates){
	
	  	int indexOfState = 0;
	std::list<State>::const_iterator iteratorOnStates;	  
	for (iteratorOnStates = listOfMinStates.begin(); iteratorOnStates != listOfMinStates.end();
					++iteratorOnStates) {
		        if(indexOfState == nodeIndex)
					return *iteratorOnStates;
				indexOfState++;
				}
}

void writeIntoFile(FILE* file, string toBeWritten){
	
char tokenClassToBeWritten[1000];
int i;
for (i = 0; i < toBeWritten.size(); i++)
	tokenClassToBeWritten[i]=toBeWritten.at(i);
tokenClassToBeWritten[i]='\n';
tokenClassToBeWritten[i+1]='\0';
fputs(tokenClassToBeWritten,file);
		   
}

string getToken(){
	string tokeen="";
	while( j< inputJavaFile.size())
	{
			 //cout<<"hhhh :  "<<j<<inputJavaFile.at(j)<<endl;
			 if(inputJavaFile.at(j)==' '||inputJavaFile.at(j)=='\n')
			  {/*write down the lastAcceptedState token*/
				  if(lastAcceptedState.accepted){
					  //////////writeIntoFile(outFile,lastAcceptedState.token);
					  sourceNode = 0; 
					  targetNode=-1;
					  tokeen=lastAcceptedState.token;
					  lastAcceptedState=State(false,"");
					  return tokeen;
					  }
			   while(j<inputJavaFile.size()&&inputJavaFile.at(j)==' ')j++;
			   while(j<inputJavaFile.size()&&inputJavaFile.at(j)=='\n'){
				   j++;
				   sourceNode = 0;
				   targetNode=-1;
			   }
			  }
			 else{
           if(inputToItsIndex.find(inputJavaFile.at(j)) != inputToItsIndex.end()){

			 targetNode = minDFATransTable[sourceNode][inputToItsIndex.at(inputJavaFile.at(j))];
			 State currentState = getMinStateOfIndex(targetNode,dfaGenerator.minimizedStates);
			  
			 if(targetNode==-1)
			 {  
				 if(sourceNode==0) // first symbol is not valid.
				 {/*ERROR!*/
				/////// writeIntoFile(outFile,"ERROR!");
				 cout<<"ERROR!"<<endl;
				}
				 else if(lastAcceptedState.accepted){
                       // write the token of lastAcceptedState into file.
					 /////writeIntoFile(outFile,lastAcceptedState.token);
					  sourceNode = 0;
				     targetNode=-1;
					  tokeen=lastAcceptedState.token;
					  lastAcceptedState=State(false,"");
					  return tokeen;
				 }
			 }else{
			 if(currentState.accepted){
			 sourceNode=targetNode;
			  j++;
			   lastAcceptedState = currentState;
			 }else{
                       // write the token of lastAcceptedState into file.
			  if(lastAcceptedState.accepted){
				//////writeIntoFile(outFile,lastAcceptedState.token);
				  sourceNode = 0;
				   targetNode=-1;
					  tokeen=lastAcceptedState.token;
					  lastAcceptedState=State(false,"");
					  return tokeen;
			     }
			  else{
				  sourceNode=targetNode;
				  j++;
			  }
			  }
			 }
			   }
		   else{
			   cout<<"Error"<<endl;
			   j++;
			   }


		}
		if(j==inputJavaFile.size()){
			//cout << lastAcceptedState.accepted << endl;
			if(lastAcceptedState.accepted){
				//////writeIntoFile(outFile,lastAcceptedState.token);
			  		 sourceNode = 0;
				     targetNode=-1;
					 tokeen=lastAcceptedState.token;
					  lastAcceptedState=State(false,"");
					  return tokeen;
			}
			else{
				if(lastAcceptedState.token.size()==0){
					return"$";
				}
				else{
					cout<<"ERROR!"<<endl;
					return"$";
					}

			}
		}
		}
}
SyntaxAnalyzer::SyntaxAnalyzer(void)
	{
	}


SyntaxAnalyzer::~SyntaxAnalyzer(void)
	{
	}

SyntaxAnalyzer::SyntaxAnalyzer(string CFGfilePath)
	{

	FILE* GrammerFile = fopen(CFGfilePath.c_str(),"r");
	if (GrammerFile==NULL)
		{
		cout << "Can't open file" << endl;
		}


	//read the file line by line and sent this line to parse line method

	vector<string> grammerLines;
	string lineStr;
	char line[1000];
	while( fgets(line,1000,GrammerFile) != NULL ){
		line[1000];
		string lineStr(line);
		unsigned  pos = lineStr.find("\n");
		lineStr = lineStr.substr (0,pos);
		grammerLines.push_back(lineStr);
		//cout<< lineStr;
		}


	for (int i =0 ; i < grammerLines.size(); i++){
		string lineExact;
		if( !grammerLines.at(i).empty() && grammerLines.at(i).at(0) == '#' ){
			lineExact = grammerLines.at(i).substr(1,grammerLines.at(i).size());
			for( int k = i+1 ; k < grammerLines.size() ; k++){
				if( !grammerLines.at(k).empty() && grammerLines.at(k).at(0) != '#' ){
					lineExact += grammerLines.at(k);
					}else{
						break;
					}
				}
			parseLine(lineExact);
			}
		}

	//@ this point the CFG map and the nonTerminal list are built

	//the bonus part gets called here, using the convTools object
	convTool.removeLeftRecursion(&CFG,&nonTerminal);
	convTool.LeftFactoring(&CFG,&nonTerminal);
	// call computeFirst(), then call computeFollow(), then call computeParsingTable()
	convTool.printMap(&CFG);
	computeFirst();
	computeFollow();
	computeParsingTable();

	// now we can call the simulation part
	simulate();// i haven't thought about the design of the simulation part, or where to put it

	}

void SyntaxAnalyzer::parseLine(string line)
	{
	int equalIndex = line.find('=');
	string nonTerminalString = spaceRemove(line.substr(0,equalIndex));
	nonTerminal.push_back(nonTerminalString);
	vector<string> OrSplit = split(line.substr(equalIndex+1,line.size()),'|');
	vector<vector<Symbol>> CFGRow;

	for ( int i = 0 ; i <  OrSplit.size(); i++){
		vector<Symbol> andSplit = splitSymbol(OrSplit.at(i),' ');
		CFGRow.push_back(andSplit);
		}

	if( OrSplit.empty() ){
		vector<Symbol> andSplit = splitSymbol(line.substr(equalIndex+1,line.size()),' ');
		CFGRow.push_back(andSplit);
		}

	CFG[nonTerminalString] = CFGRow;
	}


void SyntaxAnalyzer::computeFirst()
	{

	//for (std::map<string,vector<vector<Symbol>>>::iterator it=CFG.begin(); it!=CFG.end(); ++it){
	for( int index = 0 ; index < nonTerminal.size() ; index++){
		string nonTerminalName = nonTerminal.at(index);
		vector<vector<Symbol>> CFGRow = CFG[nonTerminalName];
		vector<string> firstForRow;
		vector<string> pindVector;
		for(int i = 0 ; i < CFGRow.size() ; i++ ){

			if( CFGRow.at(i).at(0).isTerminal ){
				firstForRow.push_back(CFGRow.at(i).at(0).sym);
				//first[nonTerminalName]=firstForRow;
				}else{
					///pindVector = computeFirst(CFGRow.at(i).at(0));



					map<string,vector<string>>::iterator it2 = first.find(CFGRow.at(i).at(0).sym);
					vector<string> symbolFirst;
					if(it2 != first.end())
						{
						// Found in First map
						symbolFirst = it2->second;
						for(int i = 0; i < symbolFirst.size() ; i++){
							int add = true;
							for( int k = 0 ; k < firstForRow.size() ; k++){
								if( firstForRow.at(k).compare(symbolFirst.at(i)) == 0 ){
									add= false;
									break;
									}
								}
							if( add )
								firstForRow.push_back(symbolFirst.at(i));
							}

						//first[nonTerminalName]=firstForRow;

						}else{
							pindVector = computeFirst(CFGRow.at(i).at(0));

							for(int i = 0 ; i < pindVector.size() ; i++){
								bool add = true;
								for(int k = 0 ; k < firstForRow.size() ; k++){
									if( firstForRow.at(k).compare(pindVector.at(i)) == 0 ){
										add = false;
										break;
										}
									}
								if( add ){
									firstForRow.push_back(pindVector.at(i));
									}
								}
							//first[nonTerminalName]=firstForRow;		
						}



				}
			first[nonTerminalName]=firstForRow;
			}
		}
	}

vector<string> SyntaxAnalyzer::computeFirst(Symbol symbol){

	vector<string> symbolFirst;
	vector<string> firstForRow;

	vector<vector<Symbol>> row = CFG[symbol.sym];
	vector<vector<string>> rowFirst;
	for ( int i = 0 ; i < row.size(); i++){
		if( row.at(i).at(0).isTerminal){
			vector<string> temp;
			temp.push_back(row.at(i).at(0).sym);
			rowFirst.push_back(temp);
			}else{
				map<string,vector<string>>::iterator it = first.find(row.at(i).at(0).sym);
				if(it != first.end()){
					rowFirst.push_back(first[row.at(i).at(0).sym]);
					}else{
						rowFirst.push_back(computeFirst(row.at(i).at(0)));
					}
			}
		}
	for( int i = 0 ; i< rowFirst.size(); i ++){
		for ( int k = 0 ; k < rowFirst.at(i).size() ; k++){
			firstForRow.push_back(rowFirst.at(i).at(k));
			}
		}
	first[symbol.sym]=firstForRow;

	return firstForRow;
	}

void SyntaxAnalyzer::computeFollow()
	{
	//iterate on the non terminals.
	std::vector<string>::const_iterator iteratorOnNonTerminals;
	for (iteratorOnNonTerminals = nonTerminal.begin();
		iteratorOnNonTerminals != nonTerminal.end();
		++iteratorOnNonTerminals) {

			string nonTerminalElementSym = *iteratorOnNonTerminals;
			follow.insert(pair<string,vector<string>>(nonTerminalElementSym, getFollowOf(nonTerminalElementSym)));

		} // end of non terminals iteration.

	// add the $ character to the terminals vector.
	terminal.push_back("$");
	}

vector<string> SyntaxAnalyzer::getFollowOf(string nonTerminalElementSym)
	{

	map<string,vector<string>>::iterator followIterator = follow.find(nonTerminalElementSym);
	if(followIterator != follow.end())
		return followIterator->second;

	vector<string> followResult;

	// iterate on production rules.
	std::map<string,vector<vector<Symbol>>>::const_iterator iteratorOnProductionRules;
	for (iteratorOnProductionRules = CFG.begin(); iteratorOnProductionRules != CFG.end();
		++iteratorOnProductionRules) {

			vector<vector<Symbol>> rightHandSide = (*iteratorOnProductionRules).second;   

			// iterate on right hands side.
			std::vector<vector<Symbol>>::const_iterator iteratorOnRightHandSide;
			for (iteratorOnRightHandSide = rightHandSide.begin(); iteratorOnRightHandSide != rightHandSide.end();
				++iteratorOnRightHandSide) {

					vector<Symbol> resultSymbols = (*iteratorOnRightHandSide);
					int indexOfResultSymbolsList = 1;
					bool lastOfResultSymbols = true;

					// iterate on result symbols. 
					std::vector<Symbol>::const_iterator iteratorOnResultSymbols;
					for (iteratorOnResultSymbols = resultSymbols.begin(); iteratorOnResultSymbols != resultSymbols.end();
						++iteratorOnResultSymbols) {

							Symbol currentSymbol = (*iteratorOnResultSymbols); 
							if(!lastOfResultSymbols) // if not last of result symbol then add first of next.
								{
								if(currentSymbol.isTerminal){ //if terminal add it to the follow result.
									if(currentSymbol.sym.compare("\\L")!=0)
										followResult.push_back(currentSymbol.sym);
									}else{ // it's non terminal
										vector<string> firstOfNext = first.find(currentSymbol.sym)->second;
										// add first of next symbol.
										followResult.insert(followResult.end(), firstOfNext.begin(), firstOfNext.end());

										//check if firstOfNext contains ebsilon then add follow of left hand side.
										if(firstHasEbsilon(firstOfNext))
											{  
											vector<string> followOfLeftSide = getFollowOf((*iteratorOnProductionRules).first);
											// add follow of left side.
											followResult.insert(followResult.end(), followOfLeftSide.begin(), followOfLeftSide.end());

											}
									} // end of if else terminal.

								lastOfResultSymbols=true; // just in case.
								break; // DONE...break from current iteration.
								}

							if(currentSymbol.sym==nonTerminalElementSym&&lastOfResultSymbols)
								{
								if(indexOfResultSymbolsList==resultSymbols.size()) // it's last of symbols
									{
									if((*iteratorOnProductionRules).first!=currentSymbol.sym){
										vector<string> followOfLeftSide = getFollowOf((*iteratorOnProductionRules).first);
										// add follow of left side.
										followResult.insert(followResult.end(), followOfLeftSide.begin(), followOfLeftSide.end());
										}
									}else // it's followed by another symbol
										lastOfResultSymbols=false;
									// end of if else.
								} // end if
							indexOfResultSymbolsList++;
						}// end of result symbols iteration.
				} // end of right hand side iteration.
		} // end of production rules iteration.

	// add $, remove ebsilon, remove duplicates.
	followResult.push_back("$");
	followResult.erase(std::remove(followResult.begin(), followResult.end(), "\\L"), followResult.end());
	std::sort(followResult.begin(), followResult.end());
	followResult.erase(unique(followResult.begin(), followResult.end()), followResult.end() );

	return followResult;
	}

void SyntaxAnalyzer::computeParsingTable(){

	generateIndexMaps();
	parsingTable = new vector<Symbol>*[nonTerminal.size()];
	int i;
	for (i = 0; i < nonTerminal.size(); i++)
		parsingTable[i]= new vector<Symbol>[terminal.size()];

	// iterate on production rules.
	std::map<string,vector<vector<Symbol>>>::const_iterator iteratorOnProductionRules;
	for (iteratorOnProductionRules = CFG.begin(); iteratorOnProductionRules != CFG.end();
		++iteratorOnProductionRules) {

			vector<vector<Symbol>> rightHandSide = (*iteratorOnProductionRules).second;   

			// iterate on right hands side.
			std::vector<vector<Symbol>>::const_iterator iteratorOnRightHandSide;
			for (iteratorOnRightHandSide = rightHandSide.begin(); iteratorOnRightHandSide != rightHandSide.end();
				++iteratorOnRightHandSide) {

					vector<Symbol> resultSymbols = (*iteratorOnRightHandSide);
					Symbol firstSymbol = *resultSymbols.begin();

					if(firstSymbol.isTerminal) // then first of it is itself.

						if(firstSymbol.sym.compare("\\L")!=0)
							parsingTable[(*(indexOfNonTerminal.find((*iteratorOnProductionRules).first))).second][(*(indexOfTerminal.find(firstSymbol.sym))).second]=resultSymbols;
							else // add follow of left side.
							{
							vector<string> followOfleftSide = follow.find((*iteratorOnProductionRules).first)->second;
							std::vector<string>::const_iterator iteratorOnFollowOfLeftSide;
							for (iteratorOnFollowOfLeftSide = followOfleftSide.begin(); iteratorOnFollowOfLeftSide != followOfleftSide.end();
								++iteratorOnFollowOfLeftSide)   
								parsingTable[(*(indexOfNonTerminal.find((*iteratorOnProductionRules).first))).second][(*(indexOfTerminal.find(*iteratorOnFollowOfLeftSide))).second]=resultSymbols;
							}

					else { 

						// add to to M[A,a] for each terminal a in first of symbol.
						vector<string> firstOffirstSymbol = first.find(firstSymbol.sym)->second;
						std::vector<string>::const_iterator iteratorOnFirstOfFirstSymbol;
						for (iteratorOnFirstOfFirstSymbol = firstOffirstSymbol.begin(); iteratorOnFirstOfFirstSymbol != firstOffirstSymbol.end();
							++iteratorOnFirstOfFirstSymbol)   
							if((*iteratorOnFirstOfFirstSymbol).compare("\\L")!=0)
								parsingTable[(*(indexOfNonTerminal.find((*iteratorOnProductionRules).first))).second][(*(indexOfTerminal.find(*iteratorOnFirstOfFirstSymbol))).second]=resultSymbols;

						// if first has ebsilon then add to the terminal of follow of A too.
						if(firstHasEbsilon(firstOffirstSymbol)){

							vector<string> followOffirstSymbol = follow.find(firstSymbol.sym)->second;
							std::vector<string>::const_iterator iteratorOnFollowOfFirstSymbol;
							for (iteratorOnFollowOfFirstSymbol = followOffirstSymbol.begin(); iteratorOnFollowOfFirstSymbol != followOffirstSymbol.end();
								++iteratorOnFollowOfFirstSymbol){   
									if((*iteratorOnFollowOfFirstSymbol).compare("$"))
										parsingTable[(*(indexOfNonTerminal.find((*iteratorOnProductionRules).first))).second][(*(indexOfTerminal.find(*iteratorOnFollowOfFirstSymbol))).second]=resultSymbols;
									else{ 
										// add -> L to "$"
										vector<Symbol> ebsilonVector;
										ebsilonVector.push_back(Symbol("\\L",true));
										parsingTable[(*(indexOfNonTerminal.find((*iteratorOnProductionRules).first))).second][(*(indexOfTerminal.find(*iteratorOnFollowOfFirstSymbol))).second]=ebsilonVector;
										} // else else. 

								} // end of iteration on follow.

							} // end of if has ebsilon.

						} // end of else.

				} // end of iteration 
		} // end of iteration on production rules.

	}

void SyntaxAnalyzer::generateIndexMaps(){

	// generate terminals map.
	std::vector<string>::const_iterator iterator1;
	int index = 0;
	for (iterator1 = terminal.begin(); iterator1 != terminal.end();
		++iterator1){
			indexOfTerminal.insert(pair<string,int>(*iterator1,index));
			index++;
		}
	// generate non terminals map.
	index = 0;  
	std::vector<string>::const_iterator iterator2;
	for (iterator2 = nonTerminal.begin(); iterator2 != nonTerminal.end();
		++iterator2){
			indexOfNonTerminal.insert(pair<string,int>(*iterator2,index));
			index++;
		}
	}

bool SyntaxAnalyzer::firstHasEbsilon(vector<string> firstOfNext){
	std::vector<string>::const_iterator iterator;
	for (iterator = firstOfNext.begin(); iterator != firstOfNext.end();
		++iterator) {
			if(*iterator=="\L")
				return true;
		}
	return false;
	}


void SyntaxAnalyzer::simulate()
{
}

vector<string> SyntaxAnalyzer::split(const string& s, char c) {
	vector<string> v;
	string::size_type i = 0;
	string::size_type j = s.find(c);

	while (j != string::npos) {
		if (!(s.substr(i, j-i).empty())){
			v.push_back(s.substr(i, j-i));
			}
		i = ++j;
		j = s.find(c, j);

		if (j == string::npos  && !(s.substr(i, s.length()).empty()) )
			v.push_back(s.substr(i, s.length()));
		}
	return v;
	}

vector<Symbol> SyntaxAnalyzer::splitSymbol(const string& s, char c) {
	vector<Symbol> v;
	string::size_type i = 0;
	string::size_type j = s.find(c);
	Symbol sym;
	while (j != string::npos) {
		if (!(s.substr(i, j-i).empty())){
			string tempString = ((s.substr(i, j-i).at(0) =='\'') && (s.substr(i, j-i).at(s.substr(i, j-i).size()-1) =='\'' ) ) ? s.substr(i+1, (j-i)-2):s.substr(i, j-i);
			if(tempString.at(tempString.size()-1) == '\'' ){
				tempString = tempString.substr(0,tempString.size()-1);
				}
			bool tempBool = ( (s.substr(i, j-i).at(0) =='\'') && (s.substr(i, j-i).at(s.substr(i, j-i).size()-1) =='\'' ) ) ? true:false;
			Symbol temp( tempString, tempBool);
			// v.push_back( Symbol( ((s.substr(i, j-i).at(0) =='\'') && (s.substr(i, j-i).at(s.substr(i, j-i).size()-1) =='\'' ) ) ? s.substr(i+1, (j-i)-2):s.substr(i, j-i),  
			// ( (s.substr(i, j-i).at(0) =='\'') && (s.substr(i, j-i).at(s.substr(i, j-i).size()-1) =='\'' ) ) ? true:false) );
			v.push_back(temp);
			if(temp.isTerminal){
				if(temp.sym.at(0)!='\\')
					addTerminal(temp.sym);
				}
			}
		i = ++j;
		j = s.find(c, j);

		if (j == string::npos  && !(s.substr(i, s.length()).empty()) ){
			string temp2String = ((s.substr(i, s.length()).at(0) =='\'') && (s.substr(i, s.length()).at(s.substr(i, s.length()).size()-1) =='\'')) ?  s.substr(i+1, s.length()-2) :s.substr(i, s.length());
			if(temp2String.at(temp2String.size()-1) == '\'' ){
				temp2String = temp2String.substr(0,temp2String.size()-1);
				}
			Symbol temp2(  temp2String, 
				((s.substr(i, s.length()).at(0) =='\'') && (s.substr(i, s.length()).at(s.substr(i, s.length()).size()-1) =='\'')) ? true:false);
			v.push_back(temp2);
			if(temp2.isTerminal){
				addTerminal(temp2.sym);
				}
			//v.push_back( Symbol(  ((s.substr(i, s.length()).at(0) =='\'') && (s.substr(i, s.length()).at(s.substr(i, s.length()).size()-1) =='\'')) ?  s.substr(i+1, s.length()-2) :s.substr(i, s.length()), 
			//((s.substr(i, s.length()).at(0) =='\'') && (s.substr(i, s.length()).at(s.substr(i, s.length()).size()-1) =='\'')) ? true:false) );
			}
		}
	return v;
	}

string SyntaxAnalyzer::spaceRemove(string symbol){
	string temp = "";
	for ( int i = 0 ; i < symbol.size() ; i++){
		if( symbol.at(i) != ' '){
			temp += symbol.at(i);
			}
		}
	return temp;
	}

void SyntaxAnalyzer::addTerminal(string sym){
	for ( int i = 0 ; i < terminal.size() ; i++){
		if(terminal.at(i).compare(sym)==0){
			return;
			}
	}
	terminal.push_back(sym);
}


int main (int arg,char** args){

	NFAGenerator nfaGenerator("rules.txt");
	nfaGenerator.getNFA();
	nfaGenerator.givenInputs.sort();
	nfaGenerator.givenInputs.unique();
    dfaGenerator = DFAGenerator(nfaGenerator.NFA, nfaGenerator.givenInputs);
	dfaGenerator.subsetConstruction();
	 minDFATransTable = dfaGenerator.generateMinDFA();



	std::list<char>::const_iterator iteratorOnChar;
	 std::list<State>::const_iterator iteratorOnChar2;

	     int node1 = 0;
		   for (iteratorOnChar2 = dfaGenerator.minimizedStates.begin(); iteratorOnChar2 != dfaGenerator.minimizedStates.end();
				++iteratorOnChar2) {	
					cout << node1<<"  " << (*iteratorOnChar2).accepted << " "<< (*iteratorOnChar2).token <<"| ";
					node1++;
			}

			int nodeCount = 0;
			
			int node = 0;
			cout << endl;
	for (int i = 0; i < 15; i++){
	    cout << node++ << " |";      
		   int symcount = 0;
		   

		for (iteratorOnChar = dfaGenerator.inputs.begin(); iteratorOnChar != dfaGenerator.inputs.end();
				++iteratorOnChar) {	
					cout << "("<<*iteratorOnChar << "," << minDFATransTable[i][symcount]<<") ";
					symcount++;	
			}

		 cout << "" << endl<< endl;
		
		} 
///////////////////////////reading from file/////////////////////
	string javaFileName = "test.txt";
	FILE* file = fopen(javaFileName.c_str(),"r");
	
    if ( file ==NULL)
		cout << "ERROR!: File Not Opened" << endl;
	

while( fgets(line,1000,file) != NULL ){
	line[1000];
	string lineStr(line);
	//unsigned  pos = lineStr.find("\n");
	//lineStr = lineStr.substr (0,pos);
	inputJavaFile+=lineStr;
} 
///////////////////////////////////////////////////////////////////

	// mapping input symbols to index.
	int indexOfChar = 0;
			std::list<char>::const_iterator iteratorOnInputs;
			for (iteratorOnInputs = nfaGenerator.givenInputs.begin(); iteratorOnInputs != nfaGenerator.givenInputs.end();
				++iteratorOnInputs){

					inputToItsIndex[*iteratorOnInputs]=indexOfChar;
					indexOfChar++;
			}


///////////////////////WRITE IN FILE////////////////////////	
//string outputFileName = "outputfile.txt";
//FILE* outFile = fopen(outputFileName.c_str(),"w");		
 //   if ( outFile==NULL)
//		cout << "ERROR!: File Not Opened" << endl;
	
//////////////////////GENERATING THE OUTPUT FILE////////////////////////
	//fclose(outFile);
	SyntaxAnalyzer syntaxAnalyzer("input.txt");
	cout << "Working Main" << endl;
	Symbol startSymbol(syntaxAnalyzer.nonTerminal.at(0),false);// start state of grammar
	stack<Symbol> simulationStack;
	simulationStack.push(Symbol("$",true));
	simulationStack.push(startSymbol);
	//map<string,int> indexOfTerminal;
	//map<string,int> indexOfNonTerminal;
	//vector<Symbol> **parsingTable;

	// the stack now has a dollar sign then the starting state
	string currentToken;
	currentToken = getToken();
	while(true){
		//currentToken = getToken();

		if(simulationStack.top().sym == "$" && currentToken != "$"){
			simulationStack.push(startSymbol);
		}

		if(simulationStack.top().sym == currentToken && currentToken == "$"){
			//parsing complete
			cout << "Accept, Succesful Completion!" << endl;
			break;
		}else if( simulationStack.top().isTerminal && simulationStack.top().sym == currentToken && currentToken != "$"){
			simulationStack.pop();
			currentToken = getToken();
			continue;
		}else if(!simulationStack.top().isTerminal){
			string nonTerminal = simulationStack.top().sym;
			int ter,nonTer;
			ter = syntaxAnalyzer.indexOfTerminal.at(currentToken);
			nonTer = syntaxAnalyzer.indexOfNonTerminal.at(nonTerminal);
			if(syntaxAnalyzer.parsingTable[nonTer][ter].size() == 0){ // empty field in table
				cout << "Error:(illegal "<< nonTerminal <<" ) – discard "<< currentToken << endl;
				currentToken = getToken();
				continue;
			}else if(syntaxAnalyzer.parsingTable[nonTer][ter].size() == 1 && syntaxAnalyzer.parsingTable[nonTer][ter].at(0).sym == "$$$sync$$$"){ // sync
				cout << "Sync is Encountered!" << endl;
				simulationStack.pop();
				continue;
			}else{
				simulationStack.pop();
				cout << nonTerminal << "\t->\t" ;
				for(int i = 0 ; i < syntaxAnalyzer.parsingTable[nonTer][ter].size()-1 ; i++){ // printing
					cout << syntaxAnalyzer.parsingTable[nonTer][ter].at(i).sym << " ";
				}
				cout << endl;

				for(int i = syntaxAnalyzer.parsingTable[nonTer][ter].size()-1 ; i >= 0 ; i-- ){// pushing in stack in reverse
					simulationStack.push(syntaxAnalyzer.parsingTable[nonTer][ter].at(i));
				}
			}

		}else if(simulationStack.top().isTerminal && simulationStack.top().sym != currentToken){
			cout <<"Error: missing " << simulationStack.top().sym <<", inserted" << endl;
			simulationStack.pop();
		}
		else{
			cout << "Error" << endl;
		}
	}

	system("pause");
	//return 0;
		

}

/*
int main (int arg,char** args){
	cout << "Working Main" << endl;
	SyntaxAnalyzer f("input.txt");
	//string test = "IF = 'if' '(' EXPRESSION ')' '{' STATEMENT '}' 'else' '{' STATEMENT '}'";
	//f.parseLine(test);
	system("pause");
}*/