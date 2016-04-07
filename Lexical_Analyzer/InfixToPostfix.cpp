#include "stdafx.h"
#include "InfixToPostfix.h"

using namespace std;

InfixToPostfix::InfixToPostfix(void)
{
}


InfixToPostfix::~InfixToPostfix(void)
{
}

int InfixToPostfix::peek(){
	if(stack.empty()){
		return 0;
}else{
	return (int)stack.top();
}

}

string InfixToPostfix::convert(string Infix) {
	string postFix ("");
	bool lastOperandIsString = false;
	for (int i = 0; i < Infix.size(); i++) {

 
		if (Infix.at(i) != '+' && Infix.at(i) != '*' && Infix.at(i) != '(' && Infix.at(i) != ')' && Infix.at(i) != '|' && Infix.at(i)!=' ') {

			if(lastOperandIsString){
				while (peek() == (int)'*' || peek() == (int)'+' || peek() == (int)'&') {
					postFix +=  stack.top();
					postFix +=" ";
					stack.pop();
				}
				stack.push('&');
				lastOperandIsString = false;
			}

			while ( i < Infix.size() && Infix.at(i) != '+' && Infix.at(i) != '*' && Infix.at(i) != '(' && Infix.at(i) != ')' && Infix.at(i) != '|' && ( Infix.at(i)!=' '||(Infix.at(i)==' ' && Infix.at(i+1)=='-') || (Infix.at(i)==' ' && Infix.at(i-1)=='-') ) ) {
				if((i+1) < Infix.size() && Infix.at(i) == '\\' && ((Infix.at(i+1) == '*')||(Infix.at(i+1) == '|')||(Infix.at(i+1) == '-')||(Infix.at(i+1) == '+')||(Infix.at(i+1) == '(')||(Infix.at(i+1) == ')'))){
					postFix += Infix.at(i);
					postFix += Infix.at(i+1);
					i++;
				}
				else if( Infix.at(i)!=' '){
					postFix += Infix.at(i);
				}
				i++;
			}
			postFix+=" ";
			i--;

			lastOperandIsString = true;
		} else if ( Infix.at(i) == '*' || Infix.at(i) == '+' ) {
			

				while(peek() == (int)'*' || peek() == (int)'+'){
					postFix += stack.top();
					postFix +=" ";
					stack.pop();
				}
				stack.push(Infix.at(i));
				//lastOperandIsString = false;
			}

		
		//else if (Infix.at(i) == '+') {
//} 
		else if (Infix.at(i) == '(') {
			if(lastOperandIsString){
				while (peek() == (int)'*' || peek() == (int)'+' || peek() == (int)'&') {
					postFix +=  stack.top();
					postFix +=" ";
					stack.pop();
				}
				stack.push('&');
				lastOperandIsString = false;
			}
			stack.push(Infix.at(i));
		} else if (Infix.at(i)== ')') {
			while( peek() != (int)'(' ){
				postFix += stack.top();
				stack.pop();
				postFix +=" ";
			}
			stack.pop();
		} else if (Infix.at(i) == ' ') {
			continue;
		}else if(Infix.at(i) == '|'){
			
			while( peek() == (int) '*' || peek() == (int) '+' || peek() == (int) '|' || peek() == (int) '&'){
				postFix += stack.top();
				stack.pop();
				postFix +=" ";
			}
			stack.push(Infix.at(i));
			lastOperandIsString = false;
		}
	}
	while ( !stack.empty() ){
		postFix+=stack.top();
		postFix+=" ";
		stack.pop();
	}
	return postFix;
}


