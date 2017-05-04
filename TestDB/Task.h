#pragma once

#define MAX_FILTER_LENGTH 1024

class Task
{
	char m_filters[MAX_FILTER_LENGTH];
	int m_action;
public:
	Task(char* filters, int action=-1);
	virtual ~Task();
	virtual void preprocessFilters() {};
};

