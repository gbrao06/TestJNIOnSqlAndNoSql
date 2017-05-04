#include "TransactionManagerImpl.h"
#include "DBDriver.h"

TransactionManagerImpl::TransactionManagerImpl(char* dburl,int dbport,char* username, char* passwd):
	TransactionHandler(),
	m_dburl(dburl),m_dbport(dbport), m_pdbclient(0)
{
	if (dbport == 3306) {
		SQLDBDriver* sqldriver=new SQLDBDriver(dburl, dbport, "root", "root");
		m_pdbclient = new DBDriver();//shared connection
		m_pdbclient->attachDriverImpl(sqldriver);
	}
	else if (dbport == 27017) {
		NOSQLDBDriver* sqldriver = new NOSQLDBDriver(dburl, dbport, "root", "root");

		m_pdbclient = new DBDriver();//shared connection
		m_pdbclient->attachDriverImpl(sqldriver);
	}
}


TransactionManagerImpl::~TransactionManagerImpl()
{
	//while service shutdown delete db instance
	delete m_pdbclient;
}

int TransactionManagerImpl::createRecords(char* json) {

	return false;
}


char* TransactionManagerImpl::getRecord(char* tablename, long id) {
	if (m_pdbclient)
		return m_pdbclient->getRecord(tablename, id);
	return 0;
}

char* TransactionManagerImpl::getAllRecords(char* tablename, char* queryparams) {
	if (m_pdbclient)
		return m_pdbclient->getAllRecords(tablename, queryparams);
	return 0;

}