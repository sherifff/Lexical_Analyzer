#pragma once
#include "Node.h"
#include <set>
class Partition
{
public:
	Partition(void);
	std::set<std::set<int>> groups;
	~Partition(void);
};

