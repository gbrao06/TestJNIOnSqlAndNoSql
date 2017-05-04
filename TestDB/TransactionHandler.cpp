#include "TransactionManagerImpl.h"

#include "TransactionHandler.h"

TransactionHandler::TransactionHandler()
	:m_pimpl(0)
{
}

TransactionHandler::~TransactionHandler() {
	delete m_pimpl; 
}

int TransactionHandler::createRecords(char* json) {//return
	if (m_pimpl)
		return m_pimpl->createRecords(json);

	return -1;
}


char* TransactionHandler::getRecord(char* tablename, long id) {
	if (m_pimpl)
		return m_pimpl->getRecord(tablename, id);
	return 0;
}

char* TransactionHandler::getAllRecords(char* tablename, char* queryparams) {
	if (m_pimpl)
		return m_pimpl->getAllRecords(tablename, queryparams);
	return 0;

}