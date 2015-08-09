
#include <Caffeine/Components/StorageComponent.hpp>
#include <Includes/SQLite/sqlite3.h>

#include <iostream>


namespace Caffeine {
namespace Components {


StorageComponent::StorageComponent()
: Entity::Component()
, m_callback()
{
	//openDatabase();
	excuteSQL("foo", m_callback);
    
    //std::map<std::string, std::string> foo;
    //m_callback(foo);
}



StorageComponent::~StorageComponent()
{
}



void StorageComponent::openDatabase()
{


}



void StorageComponent::closeDatabase()
{
}


void StorageComponent::callBack(void *caller, int size, char **argv, char **azColName)
{

}


void StorageComponent::excuteSQL(const std::string &sqlQuery, DBCallback& resultCallback)
{
	auto callback = [](void *caller, int sizeOfResults, char **argv, char **azColName) -> int
	{
		DBRowResult results;
		
		for(int i = 0; i < sizeOfResults; ++i)
		{
			results[azColName[i]] = argv[i] ? argv[i] : "NULL";
		}
	
        //*(static_cast<DBCallback*>(caller))(results);
			
		return 0;
	};

   sqlite3 *db;
   char *zErrMsg = 0;
   int rc;
   char *sql;
   const char* data = "Callback function called";
   
   /* Open database */
   rc = sqlite3_open("test.db", &db);
   if( rc ){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      exit(0);
   }else{
      fprintf(stderr, "Opened database successfully\n");
   }

   /* Create SQL statement */
   sql = "SELECT * from COMPANY";

   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, (void*)&resultCallback, &zErrMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      fprintf(stdout, "Operation done successfully\n");
   }
   sqlite3_close(db);
}



void StorageComponent::sqlResult(const std::vector<std::string> &result)
{
	for(auto &row : result)
	{
		//std::cout << row.at("NAME") << std::endl;
	}
}



void StorageComponent::sqlPrivateCallback(void *data, int i, char **fields, char **colNames)
{

}


} // namespace
} // namespace
