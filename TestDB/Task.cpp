#include "Task.h"

#include <bson.h>


Task::Task(char* filters, int action)
:m_action(action)
{
	strcpy(m_filters,filters);
}


Task::~Task()
{
}
