#pragma once
#include <stdio.h>
#include <stdlib.h>


class SqlFacadeApi
{
public:
	SqlFacadeApi(char* host, int port, char* queryparams);
	~SqlFacadeApi();
};


class SqlFlowDecider
{
public:
	SqlFlowDecider(char* host, int port, char* queryparams);
	~SqlFlowDecider();
};


