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

	/// <summary>
	/// Executes a SQL query on the database.
	/// Should only be used for updates and inserts.
	/// Makes uses of the exec() function in pqxx.
	/// </summary>
	/// <param name="transaction">: Transaction of the database to be used</param>
	/// <param name="sqlQuery">: SQL query to be executed</param>
	/// <returns>Returns pqxx::result</returns>
	pqxx::result Query(pqxx::transaction_base* transaction, std::string sqlQuery);

	/// <summary>
	/// Executes a SQL query on the database.
	/// Should only be used to receive 1 datatype. For arrays use QueryArrayValue.
	/// </summary>
	/// <typeparam name="T">: Datatype to be receiving</typeparam>
	/// <param name="transaction">: Transaction of the database to be used</param>
	/// <param name="sqlQuery">: SQL query to be executed</param>
	/// <returns>Returns type T from the database</returns>
	template <typename T>
	T QueryValue(pqxx::transaction_base* transaction, std::string sqlQuery)
	{
		T result{};

		if (transaction != nullptr)
		{
			if (!sqlQuery.empty())
			{
				try
				{
					result = transaction->query_value<T>(sqlQuery);
				}
				catch (std::exception const& e)
				{
					std::cerr << "ERROR: " << e.what() << '\n';
					DEBUG_ASSERT(false && "An error has occured when executing the SQL script");
				}
			}
			else
			{
				printf("ERROR: sqlQuery is empty\n");
				DEBUG_ASSERT(false && "sqlQuery is empty");
			}
		}
		else
		{
			printf("ERROR: transaction is a null pointer\n");
			DEBUG_ASSERT(false && "transaction is a null pointer");
		}

		return result;
	}
}
