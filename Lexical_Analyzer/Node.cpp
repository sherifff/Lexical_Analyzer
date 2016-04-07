#include "stdafx.h"
#include "Node.h"

int Node::StateCounter = 0;

Node::Node(void)
{
	nodeNum  = StateCounter++;
}


Node::~Node(void)
{

}
