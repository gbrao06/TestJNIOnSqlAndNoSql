#pragma once


//#include <stdio.h>

//#include <bson.h>
//#include <mongoc.h>
//#include <stdlib.h>

//#include <map>
//#include <string>
//#include <iostream>
//#include <boost/any.hpp>

//#include <mysql_connection.h>

class TransactionHandler;

class TransactionManager
{
	char* m_dburl;
	int m_dbport;

	TransactionHandler* m_psqlhandler;
	TransactionHandler* m_pnosqlhandler;

public:
	TransactionManager(char* dburl,int dbport);
	~TransactionManager();
	
	int createRecords(char* json);
	char* getRecord(char* tablename,long id);
	char* getAllRecords(char* tablename,char* queryparams);
	TransactionHandler* getHandlerImpl(char* dburl,int dbport);
};

