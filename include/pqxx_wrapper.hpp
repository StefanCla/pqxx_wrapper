#include <pqxx/pqxx>
#include <iostream>

//Enabled the define to allow for asserts to be triggered.
//Useful for debugging instead of testing.

// #define DEBUG_ASSERT_ENABLED

#ifdef DEBUG_ASSERT_ENABLED
#define DEBUG_ASSERT(x) assert(x)
#else
#define DEBUG_ASSERT(x)
#endif

namespace sc
{
	/// <summary>
	/// Connects the user to the database using the given url
	/// </summary>
	/// <param name="url">: Postres connection url</param>
	/// <returns>Returns pqxx::connection on success | nullptr on fail</returns>
	pqxx::connection* ConnectDatabase(std::string url);

	/// <summary>
	/// Get a one-time transaction of the given database connection
	/// </summary>
	/// <param name="con">: Connection to a database</param>
	/// <returns>Returns pqxx::work on success | nullptr on fail</returns>
	pqxx::work* GetTransaction(pqxx::connection* con);

	/// <summary>
	/// Get a transaction of the given database connection that can be used multipeltimes
	/// </summary>
	/// <param name="con">: Connection to a database</param>
	/// <returns>Returns pqxx::nontransaction on success | nullptr on fail</returns>
	pqxx::nontransaction* GetNonTransaction(pqxx::connection* con);
}
