#pragma once
#include <string.h>
using namespace std;
class State
{
public:
	State(void);
	State(bool acceptanceFlag, string tokenOfState);
	bool accepted;
	string token;
	int priority;
	static int incPriority;
	~State(void);
};

