#include "TestMySql.h"

#include "Test.h"
#include <stdlib.h>
#include <cstdint>
#include <iostream>
#include <vector>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>

using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;

#include <bson.h>
#include <mongoc.h>
#include <stdio.h>

#include <mysql_connection.h>

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

mongoc_client_t *client;

using namespace std;

#include "DBDriver.h"

int main(int argc, char *argv[])
{
	//SQLDBDriver sql("localhost",3306,"root","root");
	//char* query = "insert into testcollection (doc) values ('testdoc1')";
	//printf("query=%s", query);

	//int ret=sql.createRecords(query);
	//char * str=sql.getAllRecords("testcollection","empty");
	//printf("return=%s", str);

	NOSQLDBDriver nosql("localhost", 27017, "root", "root");
	char* str=nosql.getRecord("testcollection",1000);
	printf("mongo getRecord reault =%s", str);

}

/*(int main(int argc, char *argv[])
{
	mongoc_client_t *client;
	mongoc_collection_t *collection;
	mongoc_cursor_t *cursor;
	const bson_t *doc;
	bson_t *query;
	char *str;
	printf("hijjjjjjjjjjjjjjjj \n");
	mongoc_init();

	client =
		mongoc_client_new("mongodb://localhost:27017");
	collection = mongoc_client_get_collection(client, "ganjidb", "testcollection");
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
	mongoc_client_destroy(client);
	mongoc_cleanup();
	
	//call mysql data
	TestMySql psql;
	psql.getmysqldata();
	
		return 0;
}
*/

Test::Test()
{
}


Test::~Test()
{
}



TestMySql::TestMySql()
{

}

TestMySql::~TestMySql()
{

}

int TestMySql::getmysqldata()
{
	//const string url(argc >= 2 ? argv[1] : EXAMPLE_HOST);
	//const string user(argc >= 3 ? argv[2] : EXAMPLE_USER);
	//const string pass(argc >= 4 ? argv[3] : EXAMPLE_PASS);
	//const string database(argc >= 5 ? argv[4] : EXAMPLE_DB);

	const string url = "localhost";
	const string database = "testdb";
	try {
		sql::Driver *driver;
		sql::Connection *con;
		sql::Statement *stmt;
		sql::ResultSet *res = NULL;
		string user = "root";
		string passwd = "root";
		driver = get_driver_instance();         /* Create a connection */
		con = driver->connect("tcp://localhost:3306", "root", "root");
		con->setSchema("sakila"); /* Set the database */

		stmt = con->createStatement();
		res = stmt->executeQuery("SELECT LAST_NAME FROM ACTOR");

		while (res->next())
		{
			cout << "\t... MySQL replies: ";
			cout << res->getString("last_name").c_str() << endl;
			cout << "\t... MySQL says it again: ";
			sql::SQLString col = res->getString(1);
			cout << col.c_str() << endl;

			Sleep(3000);
		}
		delete res;
		delete stmt;
		delete con;


	}
	catch (sql::SQLException &e) {
		/*
		The MySQL Connector/C++ throws three different exceptions:

		- sql::MethodNotImplementedException (derived from sql::SQLException)
		- sql::InvalidArgumentException (derived from sql::SQLException)
		- sql::SQLException (derived f
		rom std::runtime_error)
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