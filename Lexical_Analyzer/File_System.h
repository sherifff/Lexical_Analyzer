#pragma once
//#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <string>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>

using namespace std;

class File_System
{
	 FILE *file ;
public:
	File_System();
	void init();
	string getLine();
	void begin();
	bool valid;
	~File_System(void);
};


