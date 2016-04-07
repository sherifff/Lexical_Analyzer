#include "stdafx.h"
#include "State.h"
using namespace std;

int State::incPriority = 1;

State::State(void){
	priority=-1;
	accepted = false;
}

State::State(bool acceptanceFlag, string tokenOfState)
{
	accepted=acceptanceFlag;
	token=tokenOfState;
}

State::~State(void)
{
}
