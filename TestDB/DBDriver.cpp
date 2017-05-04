#include "DBDriver.h"

#include <mysql_connection.h>

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

#include <mongoc.h>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace sql;


sql::Driver *g_psqldriver;
mongoc_client_t *g_pmongo_client;


sql::SQLString convertResultSetIntoJsonString(sql::ResultSet* rs) {
	ResultSetMetaData* rsmd = rs->getMetaData();
	sql::SQLString	jsonstr = "";
	
	while (rs->next()) {
		int numColumns = rsmd->getColumnCount();
		jsonstr.append("{");

		for (int i = 1; i < numColumns + 1; i++) {
			
			if(i!=1)
				jsonstr.append(",");

			sql::SQLString column_name = rsmd->getColumnName(i);

			//if (rsmd->getColumnType(i) == sql::DataType::) {
			//obj.put(column_name, rs.getArray(column_name));
			//}
			if (rsmd->getColumnType(i) == sql::DataType::BIGINT) {
				//obj.put(column_name, rs.getInt(column_name));
				jsonstr.append("\"");
				jsonstr.append(column_name);
				jsonstr.append("\"");
				jsonstr.append(":");
				std::string col = boost::lexical_cast<std::string>(rs->getInt64(column_name));
				jsonstr.append("\"");
				jsonstr.append(col.c_str());
				jsonstr.append("\"");

				
			}
			else if (rsmd->getColumnType(i) == sql::DataType::BIT) {
				//obj.put(column_name, rs.getBoolean(column_name));
				jsonstr.append("\"");
				jsonstr.append(column_name);
				jsonstr.append("\"");
				jsonstr.append(":");
				std::string col = boost::lexical_cast<std::string>(rs->getBoolean(column_name));

				jsonstr.append("\"");
				jsonstr.append(col.c_str());
				jsonstr.append("\"");


				//jsonstr.append(",");

			}
			//else if (rsmd->getColumnType(i) == sql::DataType::BLOB) {
			//obj.put(column_name, rs.getBlob(column_name));
			//}
			else if (rsmd->getColumnType(i) == sql::DataType::DOUBLE) {
				//obj.put(column_name, rs.getFloat(column_name));
				jsonstr.append("\"");
				jsonstr.append(column_name);
				jsonstr.append("\"");
				jsonstr.append(":");
				std::string col = boost::lexical_cast<std::string>(rs->getDouble(column_name));

				jsonstr.append("\"");
				jsonstr.append(col.c_str());
				jsonstr.append("\"");
				//jsonstr.append(",");

			}
			else if (rsmd->getColumnType(i) == sql::DataType::INTEGER) {
				//obj.put(column_name, rs.getInt(column_name));
				jsonstr.append("\"");
				jsonstr.append(column_name);
				jsonstr.append("\"");
				jsonstr.append(":");
				std::string col = boost::lexical_cast<std::string>(rs->getInt(column_name));
				jsonstr.append("\"");
				jsonstr.append(col.c_str());
				jsonstr.append("\"");
				//jsonstr.append(",");
			}
			else if (rsmd->getColumnType(i) == sql::DataType::VARCHAR) {
				jsonstr.append("\"");
				jsonstr.append(column_name);
				jsonstr.append("\"");
				jsonstr.append(":");
				jsonstr.append("\"");
				jsonstr.append(rs->getString(column_name).c_str());
				jsonstr.append("\"");

			}
			else if (rsmd->getColumnType(i) == sql::DataType::BINARY) {
				//obj.put(column_name, rs.getString(column_name));
				jsonstr.append("\"");
				jsonstr.append(column_name);
				jsonstr.append("\"");
				jsonstr.append(":");
				jsonstr.append("\"");
				jsonstr.append(rs->getString(column_name).c_str());
				jsonstr.append("\"");

				//jsonstr.append(",");
			}
			else if (rsmd->getColumnType(i) == sql::DataType::TINYINT) {
				//obj.put(column_name, rs.getInt(column_name));
				jsonstr.append("\"");
				jsonstr.append(column_name);
				jsonstr.append("\"");
				jsonstr.append(":");

				std::string col = boost::lexical_cast<std::string>(rs->getInt(column_name));
				jsonstr.append("\"");
				jsonstr.append(col.c_str());
				jsonstr.append("\"");

				//jsonstr.append(",");
			}
			else if (rsmd->getColumnType(i) == sql::DataType::SMALLINT) {
				//obj.put(column_name, rs.getInt(column_name));
				
				jsonstr.append("\"");
				jsonstr.append(column_name);
				jsonstr.append("\"");
				jsonstr.append(":");
				std::string col=boost::lexical_cast<std::string>(rs->getInt(column_name));
				SQLString query_str =col.c_str();
				jsonstr.append("\"");
				jsonstr.append(query_str);
				jsonstr.append("\"");

				//jsonstr.append(",");

			}
			else if (rsmd->getColumnType(i) == sql::DataType::DATE) {
				//obj.put(column_name, rs.getDate(column_name));
				jsonstr.append("\"");
				jsonstr.append(column_name);
				jsonstr.append("\"");
				jsonstr.append(":");
				jsonstr.append("\"");
				jsonstr.append(rs->getString(column_name).c_str());
				jsonstr.append("\"");



				//jsonstr.append(",");

			}
			//else if (rsmd->getColumnType(i) == sql::DataType::TIMESTAMP) {
			//obj.put(column_name, rs.getTimestamp(column_name));
			//jsonstr = jsonstr + "\"" + column_name + "\"" + ":";
			//jsonstr = jsonstr + rs->(column_name);
			//jsonstr = jsonstr + ",";

			//}
			//else {
			//obj.put(column_name, rs.getObject(column_name));
			//	jsonstr = jsonstr + "\"" + column_name + "\"" + ":";
			//jsonstr = jsonstr + rs->getObject(column_name);
			//jsonstr = jsonstr + ",";

			//}

			
		}//for
		jsonstr.append("}");
	}

	//json.put(obj);
	printf("json string is %s", jsonstr.c_str());
	return jsonstr;
}


SQLDBDriver::SQLDBDriver(char * dbhost, int port, char* username, char* passwd)
	:DBDriverImpl(dbhost, port, username, passwd) {
	init();
}

NOSQLDBDriver::NOSQLDBDriver(char * dbhost, int port, char* username, char* passwd)
	:DBDriverImpl(dbhost, port, username, passwd) {
	init();
};

int SQLDBDriver::init() {
	try {
		sql::Connection *pconnection;
		sql::Statement *pstatement;
		sql::ResultSet *presult = NULL;
		if (g_psqldriver == NULL)
			g_psqldriver = get_driver_instance();
	}
	catch (sql::SQLException &e) {
		/*
		The MySQL Connector/C++ throws three different exceptions:

		- sql::MethodNotImplementedException (derived from sql::SQLException)
		- sql::InvalidArgumentException (derived from sql::SQLException)
		- sql::SQLException (derived from std::runtime_error)
		*/
		return EXIT_FAILURE;

	}
	return EXIT_SUCCESS;
}

/*
DBDriverImpl* SQLDBDriver::getSingleInstance(char * dbhost, int port, char* username, char* passwd) {
	if (m_singleton == 0)
		m_singleton = new SQLDBDriver(dbhost, port, username, passwd);

	return m_singleton;
}

DBDriverImpl* NOSQLDBDriver::getSingleInstance(char * dbhost, int port, char* username, char* passwd) {
	if (m_singleton == 0)
		m_singleton = new NOSQLDBDriver(dbhost, port, username, passwd);

	return m_singleton;
}
*/

int SQLDBDriver::createRecords(char* json) {
	string jsonResult = "";
	try {
		sql::Connection *pconnection;
		//	sql::Driver *psqldriver;
		sql::Statement *pstatement;
		sql::ResultSet *presult = NULL;
		if (g_psqldriver == NULL)
			g_psqldriver = get_driver_instance();
		//psqldriver = get_driver_instance();         /* Create a connection */
		pconnection = g_psqldriver->connect("tcp://localhost:3306", "root", "root");
		pconnection->setSchema("sakila"); /* Set the database */

		pstatement = pconnection->createStatement();
		//sql::PreparedStatement* pPrepared =pconnection->prepareStatement(json);
		//std::string oid = boost::lexical_cast<std::string>(id);
		//mysql_query()
		sql::SQLString query(json);
		//sql::SQLString query);
		presult = pstatement->executeQuery(query);
		//presult = pstatement->executeQuery("INSERT INTO TESTCOLLECTION (oid,name,age,gender,profession,doc) VALUES(?)");
		//pPrepared->set;
		//pstatement->prepareStatement("INSERT INTO test(id, label) VALUES (?, ?)");
		
		/*while (presult->next())
		{
		cout << "\t... MySQL replies: ";
		//cout << res->getString("last_name") << endl;
		cout << "\t... MySQL says it again: ";
		sql::SQLString col = presult->getString(1);
		cout << col << endl;
		//Sleep(3000);
		}
		*/
		jsonResult = convertResultSetIntoJsonString(presult);
		delete presult;
		delete pstatement;
		delete pconnection;

	//	char *result = _strdup(jsonResult.c_str());
	
	}
	catch (sql::SQLException &e) {
		/*
		The MySQL Connector/C++ throws three different exceptions:

		- sql::MethodNotImplementedException (derived from sql::SQLException)
		- sql::InvalidArgumentException (derived from sql::SQLException)
		- sql::SQLException (derived from std::runtime_error)
		*/
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		/* Use what() (derived from std::runtime_error) to fetch the error message */
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;

		return EXIT_FAILURE;
		
	}
	return EXIT_SUCCESS;


}

char* SQLDBDriver::getRecord(char* tablename, long id) {

	sql::SQLString jsonResult = "";
	try {
		sql::Connection *pconnection;
		//	sql::Driver *psqldriver;
		sql::Statement *pstatement;
		sql::ResultSet *presult = NULL;
		if (g_psqldriver == NULL)
			g_psqldriver = get_driver_instance();
		//psqldriver = get_driver_instance();         /* Create a connection */
		pconnection = g_psqldriver->connect("tcp://localhost:3306", "root", "root");

		pconnection->setSchema("sakila"); /* Set the database */

		pstatement = pconnection->createStatement();
		sql::SQLString table(tablename);
		std::string oid = boost::lexical_cast<std::string>(id);		

		sql::SQLString query = "select * from ";
		query.append(table);
		query.append(" where oid = ");
		query.append(oid.c_str());
		printf("getRreecord statement=%s", query.c_str());

		//presult = pstatement->executeQuery("SELECT *_FROM testcollection");
		presult = pstatement->executeQuery(query);
		
		jsonResult=	convertResultSetIntoJsonString(presult);
		printf("getRecord Result=%s", jsonResult.c_str());

		delete presult;
		delete pstatement;
		delete pconnection;
		
		 char *result = _strdup(jsonResult.c_str());
		 return result;

	}
	catch (sql::SQLException &e) {
		/*
		The MySQL Connector/C++ throws three different exceptions:

		- sql::MethodNotImplementedException (derived from sql::SQLException)
		- sql::InvalidArgumentException (derived from sql::SQLException)
		- sql::SQLException (derived from std::runtime_error)
		*/
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		/* Use what() (derived from std::runtime_error) to fetch the error message */
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;

		//return EXIT_FAILURE;
		return "";
	}
	//return EXIT_SUCCESS;

	return "";

}


char* SQLDBDriver::getAllRecords(char* tablename, char* queryparams) {
	

	sql::SQLString jsonResult = "";
	try {
		sql::Connection *pconnection;
		//	sql::Driver *psqldriver;
		sql::Statement *pstatement;
		sql::ResultSet *presult = NULL;
		if (g_psqldriver == NULL)
			g_psqldriver = get_driver_instance();
		//psqldriver = get_driver_instance();         /* Create a connection */
		pconnection = g_psqldriver->connect("tcp://localhost:3306", "root", "root");
		pconnection->setSchema("sakila"); /* Set the database */

		pstatement = pconnection->createStatement();
		sql::SQLString table(tablename);
		//std::string oid = boost::lexical_cast<std::string>(id);

		sql::SQLString query("SELECT * FROM ");
		query.append(table);

		//presult = pstatement->executeQuery("SELECT *_FROM testcollection");
		presult = pstatement->executeQuery(query);
		
		jsonResult = convertResultSetIntoJsonString(presult);
		delete presult;
		delete pstatement;
		delete pconnection;
		char* result = "";
		result = _strdup(jsonResult.c_str());

		return result;

	}
	catch (sql::SQLException &e) {
		/*
		The MySQL Connector/C++ throws three different exceptions:

		- sql::MethodNotImplementedException (derived from sql::SQLException)
		- sql::InvalidArgumentException (derived from sql::SQLException)
		- sql::SQLException (derived from std::runtime_error)
		*/
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		/* Use what() (derived from std::runtime_error) to fetch the error message */
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;

		//return EXIT_FAILURE;
		return "";
	}
	//return EXIT_SUCCESS;

	return "";


	return "";
}


/////////////////////////////////////////////NOSQL
int NOSQLDBDriver::init() {
	//mongoc_client_t *client;
	mongoc_collection_t *collection;
	mongoc_cursor_t *cursor;
	const bson_t *doc;
	bson_t *query;
	char *str;
	printf("hijjjjjjjjjjjjjjjj \n");
	mongoc_init();
	if(g_pmongo_client ==NULL)
		g_pmongo_client =mongoc_client_new("mongodb://localhost:27017");
	/*
	collection = mongoc_client_get_collection(g_pmongo_client, "ganjidb", "testcollection");
	query = bson_new();
	cursor = mongoc_collection_find_with_opts(collection, query, NULL, NULL);

	while (mongoc_cursor_next(cursor, &doc)) {
		str = bson_as_json(doc, NULL);
		printf("%s\n", str);
		bson_free(str);
	}

	bson_destroy(query);
	mongoc_cursor_destroy(cursor);
	mongoc_collection_destroy(collection);
	//mongoc_client_destroy(m_pclient);
	mongoc_cleanup();
	*/
	return 0;
}

int  NOSQLDBDriver::createRecords(char* json) {
	return true;
}

char* NOSQLDBDriver::getRecord(char* tablename, long id) {

	//mongoc_client_t *client;
	mongoc_collection_t *collection;
	mongoc_cursor_t *cursor;
	const bson_t *doc;
	bson_t *query;
	char *str;
	printf("hijjjjjjjjjjjjjjjj \n");
	mongoc_init();

	//client =
		//mongoc_client_new("mongodb://localhost:27017");
	collection = mongoc_client_get_collection(g_pmongo_client, "ganjidb", tablename);
	query = bson_new();
	cursor = mongoc_collection_find_with_opts(collection, query, NULL, NULL);
	string jsonStr = "";
	while (mongoc_cursor_next(cursor, &doc)) {
		str = bson_as_json(doc, NULL);
		jsonStr = jsonStr + str;
		printf("%s\n", str);
		bson_free(str);
	}

	bson_destroy(query);
	mongoc_cursor_destroy(cursor);
	mongoc_collection_destroy(collection);
	//mongoc_client_destroy(g_pmongo_client);
	mongoc_cleanup();

	//call mysql data
	

	return _strdup(jsonStr.c_str());
}

char* NOSQLDBDriver::getAllRecords(char* tablename, char* queryparams) {

	mongoc_collection_t *collection;
	mongoc_cursor_t *cursor;
	const bson_t *doc;
	bson_t *query;
	char *str;
	printf("hijjjjjjjjjjjjjjjj \n");
	mongoc_init();

	//client =
	//mongoc_client_new("mongodb://localhost:27017");
	collection = mongoc_client_get_collection(g_pmongo_client, "ganjidb", tablename);
	query = bson_new();
	cursor = mongoc_collection_find_with_opts(collection, query, NULL, NULL);
	string jsonStr = "";
	while (mongoc_cursor_next(cursor, &doc)) {
		str = bson_as_json(doc, NULL);
		jsonStr = jsonStr + str;
		printf("%s\n", str);
		bson_free(str);
	}

	bson_destroy(query);
	mongoc_cursor_destroy(cursor);
	mongoc_collection_destroy(collection);
	//mongoc_client_destroy(g_pmongo_client);
	mongoc_cleanup();

	//call mysql data


	return _strdup(jsonStr.c_str());
}

