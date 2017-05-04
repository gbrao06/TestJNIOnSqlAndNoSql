#pragma once

#include "TransactionHandler.h"

#include "DBDriver.h"

class TransactionManagerImpl: public TransactionHandler
{
	char* m_dburl;
	int m_dbport;
	DBDriver* m_pdbclient;
	
public:
	TransactionManagerImpl(char* dburl,int dbport,char* username, char* passwd);

	virtual ~TransactionManagerImpl();

	int createRecords(char* json);
	char* getRecord(char* tablename, long id);
	char* getAllRecords(char* tablename, char* queryparams);

};

