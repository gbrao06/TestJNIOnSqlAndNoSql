#include "TestMySql.h"

#include <mysql_connection.h>

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

#define EXAMPLE_HOST "localhost"
#define EXAMPLE_USER "root"
#define EXAMPLE_PASS "get your own password here"
#define EXAMPLE_DB "information_schema"

#include <windows.h>
using namespace std;


TestMySql::TestMySql()
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
		sql::ResultSet *res=NULL;
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
			//cout << res->getString("last_name") << endl;
			cout << "\t... MySQL says it again: ";
			sql::SQLString col = res->getString(1);
			cout <<  col << endl;
			
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

/*
#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

using namespace std;

int main(void)
{
	cout << endl;
	cout << "Running 'SELECT 'Hello World!' »
		AS _message'..." << endl;

		try {
		sql::Driver *driver;
		sql::Connection *con;
		sql::Statement *stmt;
		sql::ResultSet *res;

		// Create a connection 
		driver = get_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3306", "root", "root");
		//Connect to the MySQL test database 
		con->setSchema("test");

		stmt = con->createStatement();
		res = stmt->executeQuery("SELECT 'Hello World!' AS _message");
		while (res->next()) {
			cout << "\t... MySQL replies: ";
			Access column data by alias or column name
			cout << res->getString("_message") << endl;
			cout << "\t... MySQL says it again: ";
			Access column data by numeric offset, 1 is the first column
			cout << res->getString(1) << endl;
		}
		delete res;
		delete stmt;
		delete con;

	}
	catch (sql::SQLException &e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " »
			<< __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}

	cout << endl;

	return EXIT_SUCCESS;
}
*/
TestMySql::~TestMySql()
{
}
