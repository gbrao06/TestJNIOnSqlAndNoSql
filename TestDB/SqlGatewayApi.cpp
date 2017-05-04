#include "SqlGatewayApi.h"

#include <stdlib.h>
#include <cstdint>
#include <iostream>
#include <vector>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>


#include <bson.h>
#include <mongoc.h>
#include <stdio.h>

#include <mysql_connection.h>

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

using namespace std;
using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;

#define  MAX_URL_SIZE 1024;

std::string getRecordsInJson(char *url);

SqlFacadeApi::SqlFacadeApi(char* host,int port, char* queryparams)
{
	//check the port mapping configuration
}


SqlFacadeApi::~SqlFacadeApi()
{

}

SqlFlowDecider::SqlFlowDecider(char* host, int port, char* queryparams)
{
	//check the port mapping configuration
	//ideally should be a seperate class which can read the configuration and decide
	//temporarily decide based on port
	if (port == 27017)
	{
		//MongoDB mongodb;

	}
}

SqlFlowDecider::~SqlFlowDecider()
{
	//check the port mapping configuration
}

//int main(int args, char argv[]) {
	//std::string json = getRecordsInJson("localhost:3036?querydata=dummy");
//}

std::string getRecordsInJson(char *url) {

	if (url == NULL) return std::string("emprt url");
	char urlcp[1024];
	strcpy(urlcp, url); //query part not present

	//take url part i.e., left of ?
	char leftpart[1024];
	const char* delimiter = "?";
	char querypart[1024];
	//url part
	char * token = std::strtok(urlcp, delimiter);

	if (token != NULL) {
		strcpy(leftpart, token);
		token = std::strtok(NULL, delimiter);
		if (token != NULL)
			strcpy(querypart, token);
	}
	else
		strcpy(leftpart, urlcp); //query part not present

	//get host port nos
	const char* colon = ":";
	char host[1024];
	char aport[10];
	//host:port?
	token = std::strtok(leftpart, colon);
	if (token != NULL) {
		strcpy(host, token);
		
		token = std::strtok(NULL, colon);

		if (token != NULL)
			strcpy(aport, token);
		else
			return std::string("host empty");//port must be present
	}
	else
		return  std::string("port empty");; //host must present
	
	int port=std::stoi(aport);

	//const string url(argc >= 2 ? argv[1] : EXAMPLE_HOST);
	//const string user(argc >= 3 ? argv[2] : EXAMPLE_USER);
	//const string pass(argc >= 4 ? argv[3] : EXAMPLE_PASS);
	//const string database(argc >= 5 ? argv[4] : EXAMPLE_DB);

	return std::string("");
}