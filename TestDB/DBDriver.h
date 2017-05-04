#pragma once



class DBDriverImpl {
private:
	//for now by username and passwd
	char* m_username;
	char* m_passwd;
protected:
	char* m_name;
	char * m_dbhost;
	int m_dbport;

protected:
	virtual int init() = 0;

public:
	DBDriverImpl(char * dbhost, int port, char* username, char* passwd) :m_name(0),
		m_dbhost(dbhost),
		m_dbport(port),
		m_username(username),
		m_passwd(passwd) {

	};


public:
	DBDriverImpl() {}
	virtual int createRecords(char* json) { return false; };
	virtual char* getRecord(char* tablename, long id) { return 0; };
	virtual char* getAllRecords(char* tablename, char* queryparams) { return 0; };
};

class  DBDriver
{
	DBDriverImpl* m_dbdriver;

public:
	DBDriver() :
		m_dbdriver(0) {}
	void attachDriverImpl(DBDriverImpl * impl) { m_dbdriver = impl; }
	virtual ~DBDriver() {};

	virtual int createRecords(char* json) { return m_dbdriver->createRecords(json); };
	virtual char* getRecord(char* tablename, long id) { return m_dbdriver->getRecord(tablename,id); };
	virtual char* getAllRecords(char* tablename, char* queryparams) { return m_dbdriver->getAllRecords(tablename, queryparams); };

};


class SQLDBDriver :public DBDriverImpl {
	friend DBDriver;
	//sql::Driver *m_psqldriver;
public:
	SQLDBDriver(char * dbhost, int port, char* username, char* passwd);


	int init();
public:
	
	int createRecords(char* json);
	char* getRecord(char* tablename, long id);
	char* getAllRecords(char* tablename, char* queryparams);

};

class NOSQLDBDriver :public DBDriverImpl {
	DBDriverImpl* m_singleton;
	//mongoc_client_t *m_pclient;
public:
	NOSQLDBDriver(char * dbhost, int port, char* username, char* passwd);

	int init();
public:
	virtual int createRecords(char* json) ;
	virtual char* getRecord(char* tablename, long id);
	virtual char* getAllRecords(char* tablename, char* queryparams);

	 
};

/*

1. Have a template
//DBDriver function template
Handle<FunctionTemplate> dbdriver_template = FunctionTemplate::New();
//create instance
Handle<ObjectTemplate> driver_instance_template = dbdriver_template->InstanceTemplate();
//set its internal field count to one (we'll put references to the C++ DBDriver here later)
driver_instance_template->SetInternalFieldCount(1);

//add properties
//add some properties (x and y)
driver_instance_template->SetAccessor(String::New("x"), GetPointX, SetPointX);
driver_instance_template->SetAccessor(String::New("y"), GetPointY, SetPointY);

2. Wrtap that template

Handle<Object> wrapDBDriver(DbDriver *driverToWrap) //DBDriver is c++ object 
{
//enter a handle scope
HandleScope handle_scope;

//create a new point instance
Local<Object> driver_instance = driver_templ->NewInstance();

//set that internal field
driver_instance->SetInternalField(0, External::New(DriverToWrap));

//I'm skipping some Persistent MakeWeak goodness right here

//to prevent the point_instance from being destroyed when its
//scope handle_scope is, use the Close() function
return handle_scope.Close(driver_instance);

}


*/