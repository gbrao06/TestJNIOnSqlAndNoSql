#pragma once
class TransactionManagerImpl;
class TransactionHandler
{
	//Handler is the adapterobject and TransactionManagerImpl is one such thing that takes care of everythng..input/output mapper,configuration etc..
	TransactionManagerImpl * m_pimpl;

public:
	
	TransactionHandler();
	virtual ~TransactionHandler();
	virtual int TransactionHandler::createRecords(char* json);
	virtual char* TransactionHandler::getRecord(char* tablename, long id);
	virtual char* TransactionHandler::getAllRecords(char* tablename, char* queryparams);
	void attachHandlerImpl(TransactionManagerImpl *impl) { m_pimpl = impl; }
};