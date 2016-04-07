#include "stdafx.h"
#include "NFAGenerator.h"


NFAGenerator::NFAGenerator(string fileName)
{
	languageFile = fopen(fileName.c_str(),"r");
	if (languageFile==NULL)
	{
		languageFileValid = false;
		cout << "Can't open file" << endl;
	}
	Graph t;
	vector<Node*> v;
	vector<Node*>::iterator it = v.begin();
	it = v.insert(it,t.end);
	t.start->edges[0] = v;
	regDef["\\L"]=t;
}


NFAGenerator::~NFAGenerator(void)
{

}




void NFAGenerator::parseLine(string line){
	if( line.at(0)=='{' ){
		vector<string>::iterator it;
		vector<string> splitted;
		it = splitted.begin();

		//line = line.substr(1,line.length()-2);
		int index = 0;
		std::string delimiter = " ";
		size_t pos = 0;
		std::string token;
		int numberOfSpaces=0;
		for(int i = 0 ; i < line.size();i++ ){
			if(line.at(i) == ' ' ||  line.at(i) == '{' ){
				numberOfSpaces++;
			}else{
				break;
			}
		}
		if(numberOfSpaces > 0){
			line = line.substr(numberOfSpaces,line.length());
		}
		numberOfSpaces = 0;
		for(int i =  line.size()-1; i >= 0 ;i-- ){
			if(line.at(i) == ' ' || line.at(i) == '}' ){
				numberOfSpaces++;
			}else{
				break;
			}
		}
		if(numberOfSpaces > 0){
			line = line.substr(0,line.length()-numberOfSpaces);
		}
		
		for(int i = 0 ; i < line.size() ; i++){
			if(line.at(i) == ' '){
			it = splitted.insert(it,token);
			token="";
			}else{
				token+=line.at(i);
			}
		}

		it = splitted.insert(it,token);

		createKeywordGraph(splitted);
	}else if( line.at(0)=='[' ){
		vector<string>::iterator it;
		vector<string> splitted;
		it = splitted.begin();
		int index = 0;
		std::string delimiter = " ";
		size_t pos = 0;
		std::string token;
		int numberOfSpaces=0;
		for(int i = 0 ; i < line.size();i++ ){
			if(line.at(i) == ' ' ||  line.at(i) == '[' ){
				numberOfSpaces++;
			}else{
				break;
			}
		}
		if(numberOfSpaces > 0){
			line = line.substr(numberOfSpaces,line.length());
		}
		numberOfSpaces = 0;
		for(int i =  line.size()-1; i >= 0 ;i-- ){
			if(line.at(i) == ' ' || line.at(i) == ']' ){
				numberOfSpaces++;
			}else{
				break;
			}
		}
		if(numberOfSpaces > 0){
			line = line.substr(0,line.length()-numberOfSpaces);
		}
		
		for(int i = 0 ; i < line.size() ; i++){
			if(line.at(i) == ' '){
			it = splitted.insert(it,token);
			token="";
			}else{
				token+=line.at(i);
			}
		}

		it = splitted.insert(it,token);

		createPunctuationGraph(splitted);
	}else{
		string token;
		int index = 0;
		while( line.at(index) != '=' && line.at(index) != ':' ){
			if(line.at(index)==' '){
				index++;
				continue;
			}
			token += line.at(index);
			index++;
		}
		string line2 = line.substr(index+1,line.size());
		string temp = itp.convert(line2);
		Graph regular = evaluate(temp);
		if(line.at(index) == '=' ){
			regDef[token]=regular;
		}else{
			vector<Graph>::iterator it = regEx.begin();
			regular.end->state.accepted = true;
			regular.end->state.token = token;
			regular.end->state.priority = State::incPriority++;
			regEx.insert(it,regular);
		}
	}
}

void NFAGenerator::combine(){

	vector<Node*> destNodes;
	vector<Node*> endNode;
	vector<Node*>::iterator it = endNode.begin();
	endNode.insert(it,NFA.end);
	map<char,vector<Node*>>::iterator mit = NFA.start->edges.begin();
	map<char,vector<Node*>>::iterator endIt;
	it = destNodes.begin();
	vector<Graph> graphit;
	vector<Node*>::iterator endNodeit = endNode.begin();
	endNodeit = endNode.insert(endNodeit,NFA.end);
	vector<Node*>::iterator vecit;
	
	for(int i = 0 ; i < regEx.size() ; i++){
		it = destNodes.insert(it,regEx.at(i).start);
		
		if(regEx.at(i).end->edges.find(0) != regEx.at(i).end->edges.end()){
			vecit = regEx.at(i).end->edges.at(0).begin();
			vecit = regEx.at(i).end->edges.at(0).insert(vecit,NFA.end);			
		}else{
			endIt = regEx.at(i).end->edges.begin();
			endIt = regEx.at(i).end->edges.insert(endIt,pair<char,vector<Node*>>(0,endNode));
		}
	
	}
		NFA.start->edges[0]=destNodes;
	
}

void NFAGenerator::createKeywordGraph(vector<string> strvec){
	for (int i = 0; i < strvec.size() ; i++){
		Graph keyword;
		Node* current=new Node();
		Node* next=new Node();
		vector<Node*> destNodes;
		vector<Node*>::iterator it= destNodes.begin();
		it = destNodes.insert(it,current);
		//keyword.start->edges.insert(pair<char,vector<Node*>>(strvec.at(i).at(0),destNodes));
		keyword.start->edges[strvec.at(i).at(0)]=destNodes;
		givenInputs.push_back(strvec.at(i).at(0));
		keyword.end->state.accepted = true;
		keyword.end->state.token = strvec.at(i);
		keyword.end->state.priority = 0;	
		for (int k = 1 ; k < strvec.at(i).size()-1 ; k++ ){
			destNodes = vector<Node*>();
			it = destNodes.begin();
			it = destNodes.insert(it,next);
			//current->edges.insert(pair<char,vector<Node*>>(strvec.at(i).at(k),destNodes));
			current->edges[strvec.at(i).at(k)]=destNodes;
			givenInputs.push_back(strvec.at(i).at(k));
			current = next;
			next = new Node();
		}

		if(strvec.at(i).size() > 1){
			destNodes = vector<Node*>();
			it = destNodes.begin();
			it = destNodes.insert(it,keyword.end);
			//current->edges.insert(pair<char,vector<Node*>>(strvec.at(i).at(strvec.at(i).size()-1),destNodes));
			current->edges[strvec.at(i).at(strvec.at(i).size()-1)]=destNodes;
			givenInputs.push_back(strvec.at(i).at(strvec.at(i).size()-1));
		}
		//insert keyword into regEx vector
		vector<Graph>::iterator vit;
		vit = regEx.begin();
		regEx.insert(vit,keyword);

	}
}

void NFAGenerator::createPunctuationGraph(vector<string> strvec){
	for (int i = 0; i < strvec.size() ; i++){

		if(strvec.at(i).size() == 2 && strvec.at(i).at(0) == '\\'){
			strvec.at(i) = strvec.at(i).substr(1,strvec.at(i).size());
		}else if(strvec.at(i).size() > 2 ){
			cout << "Error!" << endl;
			continue;
		}

		Graph punct;
		vector<Node*> destNodes;
		vector<Node*>::iterator it = destNodes.begin();
		it = destNodes.insert(it,punct.end);
		punct.start->edges[strvec.at(i).at(0)]=destNodes;
		givenInputs.push_back(strvec.at(i).at(0));
		punct.end->state.accepted = true;
		punct.end->state.token = strvec.at(i);
		punct.end->state.priority = 0;
		//insert punct into regEx vector
		vector<Graph>::iterator vit;
		vit = regEx.begin();
		regEx.insert(vit,punct);

	}
}


Graph NFAGenerator::evaluate(string postFix) {
	string splited[100];
	int index = 0;
	std::string delimiter = " ";
	size_t pos = 0;
	std::string token;

	while ((pos = postFix.find(delimiter)) != std::string::npos) {
		token = postFix.substr(0, pos);
		splited[index] = token;
		//std::cout << token << std::endl;
		postFix.erase(0, pos + delimiter.length());
		index++;
	}
	splited[index] = postFix;

	std::stack<Graph> evaStack;

	for ( int i = 0 ; i < index  ; i++){
		if(splited[i].at(0)=='+'){
			Graph firstOperand = evaStack.top();
			evaStack.pop();
			vector<Node*> destNodes;
			vector<Node*>::iterator it= destNodes.begin();
			map<char,vector<Node*>>::iterator mapit = firstOperand.end->edges.begin();
			it = destNodes.insert(it,firstOperand.start);

			if(firstOperand.start->edges.find(0) != firstOperand.start->edges.end()){
				it = firstOperand.end->edges.at(0).begin();
				firstOperand.end->edges.at(0).insert(it,firstOperand.start);
			}else{
				//firstOperand.end->edges[0]=destNodes;
				//mapit = firstOperand.start->edges.insert(mapit,pair<char,vector<Node*>>(0,destNodes));
				mapit =  firstOperand.end->edges.insert(mapit,pair<char,vector<Node*>>(0,destNodes));

			}
			evaStack.push(firstOperand);
		}else if( splited[i].at(0)=='|') {

			Graph firstOperand = evaStack.top();
			evaStack.pop();
			Graph secondOperand = evaStack.top();
			evaStack.pop();
			//Graph newGraph;
			Node* newStart = new Node();
			Node* newEnd = new Node();
			vector<Node*> destNodes;
			vector<Node*>::iterator it= destNodes.begin();
			it = destNodes.insert(it,firstOperand.start);
			it = destNodes.insert(it,secondOperand.start);
			newStart->edges[0]=destNodes;

			vector<Node*> destNodes1;
			vector<Node*>::iterator it1= destNodes1.begin();
			if(firstOperand.end->edges.find(0) != firstOperand.end->edges.end()){
				it1 = firstOperand.end->edges.at(0).begin();
				firstOperand.end->edges.at(0).insert(it1,newEnd);
			}else{
				it1 = destNodes1.insert(it1,newEnd);
				firstOperand.end->edges[0]=destNodes1;
			}

			vector<Node*> destNodes2;
			vector<Node*>::iterator it2= destNodes2.begin();

			if(secondOperand.end->edges.find(0) != secondOperand.end->edges.end()){
				it2 = secondOperand.end->edges.at(0).begin();
				secondOperand.end->edges.at(0).insert(it2,newEnd);
			}else{
				it2 = destNodes2.insert(it2,newEnd);
				secondOperand.end->edges[0]=destNodes2;
			}
			firstOperand.start = newStart;
			firstOperand.end = newEnd;
			evaStack.push(firstOperand);
		}else if (splited[i].at(0)=='*'){ 
			Graph firstOperand = evaStack.top();
			evaStack.pop();
			Node* newStart=new Node();
			Node* newEnd=new Node();
			vector<Node*> destNodes;
			vector<Node*>::iterator it;

			if(firstOperand.end->edges.find(0) != firstOperand.end->edges.end()){

				it = firstOperand.end->edges.at(0).begin();
				it = firstOperand.end->edges.at(0).insert(it,firstOperand.start);
				it = firstOperand.end->edges.at(0).insert(it,newEnd);
			}else{
				it = destNodes.begin();
				it = destNodes.insert(it,firstOperand.start);
				it = destNodes.insert(it,newEnd);
				firstOperand.end->edges[0]=destNodes;
			}
			vector<Node*> newStartVec;
			it = newStartVec.begin();
			it = newStartVec.insert(it,firstOperand.start);
			it = newStartVec.insert(it,newEnd);
			newStart->edges[0]=newStartVec;
			firstOperand.end = newEnd;
			firstOperand.start = newStart;
			evaStack.push(firstOperand);
		}else if(splited[i].at(0)=='&'){

			Graph secondOperand = evaStack.top();
			evaStack.pop();
			Graph firstOperand = evaStack.top();
			evaStack.pop();

			vector<Node*> destNodes;
			vector<Node*>::iterator it= destNodes.begin();

			if(firstOperand.end->edges.find(0) != firstOperand.end->edges.end()){
				it = firstOperand.end->edges.at(0).begin();
				firstOperand.end->edges.at(0).insert(it,secondOperand.start);
			}else{
				it = destNodes.insert(it,secondOperand.start);
				firstOperand.end->edges[0]=destNodes;
			}
			firstOperand.end = secondOperand.end;
			evaStack.push(firstOperand);
		}else{
			if( regDef.find(splited[i]) != regDef.end()){
				
				map<char,vector<Node*>>::iterator m1 = regDef.at(splited[i]).start->edges.begin();
				map<char,vector<Node*>>::iterator m2 = regDef.at(splited[i]).end->edges.begin();

				Node* start = copyGraph(regDef.at(splited[i]).start,regDef.at(splited[i]).end->nodeNum,regDef.at(splited[i]).start->nodeNum);
				Graph g;//(start,&end);
				*(g.start) = *start;
				g.end = end;
				end= new Node();
				//g.end->edges = endedges;
				//g.end->nodeNum = end.nodeNum;
				//g.end->state = end.state;
				evaStack.push(g);


			}else{
				size_t found = splited[i].find('-');
				if( found != string::npos && splited[i].size() > 2 ){
					// range
					if( splited[i].size() == 3 ){
						evaStack.push(createRangeGraph(splited[i].at(0),splited[i].at(2)));
					}

				}else{
					//m[yy]=l'

		    		bool lastSlash = false;
					string temp="";
					for(int m = 0 ; m < splited[i].size() ; m++){
						if(lastSlash && splited[i].at(m) == '\\'){
							temp+='\\';
							}
						else if(splited[i].at(m) == '\\'){
							lastSlash = true;
							continue;
							}
						else{
							temp+=splited[i].at(m);
							}
						lastSlash = false;
						}
					splited[i] = temp;
		


					Graph keyword;
					Node* current=new Node();
					Node* next=new Node();
					vector<Node*> destNodes;
					vector<Node*>::iterator it= destNodes.begin();
					vector<Node*> destNodes2;
					vector<Node*>::iterator it2= destNodes2.begin();
					it = destNodes.insert(it,current);
					//keyword.start->edges.insert(pair<char,vector<Node*>>(splited[i].at(0),destNodes));
					keyword.start->edges[splited[i].at(0)]=destNodes;
					givenInputs.push_back(splited[i].at(0));
					
					for (int k = 1 ; k < splited[i].size()-1 ; k++ ){
						destNodes = vector<Node*>();
						it = destNodes.begin();
						it = destNodes.insert(it,next);
						//current->edges.insert(pair<char,vector<Node*>>(splited[i].at(k),destNodes));
						current->edges[splited[i].at(k)]=destNodes;
						givenInputs.push_back(splited[i].at(k));
						current = next;
						next = new Node();
					}

					if(splited[i].size() == 1){
						it2 = destNodes2.begin();
						it2 = destNodes2.insert(it2,keyword.end);
						keyword.start->edges[splited[i].at(0)]=destNodes2;
					
					}
					else if(splited[i].size() > 1){
						destNodes = vector<Node*>();
						it = destNodes.begin();
						it = destNodes.insert(it,keyword.end);
						//current->edges.insert(pair<char,vector<Node*>>(splited[i].at(splited[i].size()-1),destNodes));
						current->edges[splited[i].at(splited[i].size()-1)]=destNodes;
						givenInputs.push_back(splited[i].at(splited[i].size()-1));
					}
					//insert keyword into regEx vector
					evaStack.push(keyword);
				}
			}
		}
	}
	if( evaStack.size() != 1 )
		throw exception();
	return evaStack.top();
}

Graph NFAGenerator::createRangeGraph(char from,char to){
	vector<Node*> destNodes;
	vector<Node*>::iterator it = destNodes.begin();
	Graph temp;
	it = destNodes.insert(it,temp.end);
	//map<char,vector<Node*>>::iterator mit = temp.start->edges.begin();
	for ( int i = (int)from ; i <= (int)to ; i++ ){
		//mit = temp.start->edges.insert(mit,pair<char,vector<Node*>>((char)i,destNodes));
		temp.start->edges[(char)i]=destNodes;
		givenInputs.push_back((char)i);
	}
	return temp;
}

void NFAGenerator::getNFA(){

	if (languageFile==NULL)
	{
		cout << "File Not Opened" << endl;
		return;
	}

	string lineStr;
	char line[1000];
	while( fgets(line,1000,languageFile) != NULL ){
		line[1000];
		string lineStr(line);
		unsigned  pos = lineStr.find("\n");
		lineStr = lineStr.substr (0,pos);
		parseLine(lineStr);
		//cout<< lineStr;
	}

	combine();
	
}


float contorPairing(int a, int b){

	return (0.5)*(a+b)*(a+b+1) + b;
}


Node* NFAGenerator::copyGraph(Node* n,const int endNumber, int from){
	visited.clear();
	return traverseGraph(n,endNumber,from);
}

Node* NFAGenerator::traverseGraph(Node* n,const int endNumber, int from){


	if( visited.find(n->nodeNum) != visited.end() ){
		return visited.at(n->nodeNum);//already found
	}else{
		// not found, new node, iterate on it's edges
		//visited[n->nodeNum]= insert(contorPairing(n->nodeNum,from));
		map<char,vector<Node*>> edges;
		map<char,vector<Node*>>::iterator mapit = edges.begin();
		vector<Node*>::iterator vecit;
		vector<Node*>::iterator newVecit;
		Node* newNode = new Node();
		visited[n->nodeNum]=newNode;


		for ( mapit = n->edges.begin(); mapit!=n->edges.end(); ++mapit){// iterate on map
			vector<Node*> nodes;
			vector<Node*> newNodes;
			nodes = mapit->second;
			vecit = nodes.begin();
			newVecit = newNodes.begin();
			for(int i = 0 ; i < nodes.size() ; i++){// iterate on vector
				Node* temp = traverseGraph(nodes.at(i),endNumber, n->nodeNum);
				if(temp != NULL){
					newVecit = newNodes.insert(newVecit,temp);
				}
			}
			edges[mapit->first]=newNodes;
		}
		newNode->state.accepted = n->state.accepted;
		newNode->state.token = n->state.token;
		newNode->edges = edges;
		if(n->nodeNum == endNumber){
			end = newNode;
		}
		//visited[n->nodeNum]=newNode;
		return newNode;
	}
}
