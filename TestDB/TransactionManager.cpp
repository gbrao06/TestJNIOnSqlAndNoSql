#include "TransactionHandler.h"

#include "TransactionManager.h"

#include "TransactionManagerImpl.h"

#include "DBDriver.h"

#include "jni.h"

TransactionManager::TransactionManager(char* url,int dbport)
	:m_dburl(url),
	m_dbport(dbport)
{
	//create two handlers
	//TransactionManagerImpl* psqlimpl=new TransactionManagerImpl(url, 3306, "root", "root");
	TransactionManagerImpl* psqlimpl = new TransactionManagerImpl(url, dbport, "root", "root");
	m_psqlhandler = new TransactionHandler();
	m_psqlhandler->attachHandlerImpl(psqlimpl);

	//TransactionManagerImpl* pnosqlimpl = new TransactionManagerImpl(url, 27017, "root", "root");
	TransactionManagerImpl* pnosqlimpl = new TransactionManagerImpl(url, dbport, "root", "root");

	m_pnosqlhandler = new TransactionHandler();

	m_pnosqlhandler->attachHandlerImpl(pnosqlimpl);
}


TransactionManager::~TransactionManager()
{
	delete 	m_psqlhandler;
	delete m_pnosqlhandler;
}

int TransactionManager::createRecords(char* json) {

	return false;
}

char* TransactionManager::getRecord(char* tablename, long id) {
	return getHandlerImpl(m_dburl,m_dbport)->getRecord(tablename,id);
}
char* TransactionManager::getAllRecords(char* tablename, char* queryparams) {
	return  getHandlerImpl(m_dburl, m_dbport)->getAllRecords(tablename, queryparams);
}

TransactionHandler* TransactionManager::getHandlerImpl(char* dburl, int dbport) {
	if (dbport == 3306)
		return m_psqlhandler;
	else if (dbport == 27017)
		return m_pnosqlhandler;

	return 0;

}

