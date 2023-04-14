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
	pqxx::connection* ConnectDatabase(pqxx::zview url);

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
	pqxx::result Query(pqxx::transaction_base* transaction, pqxx::zview sqlQuery);

	/// <summary>
	/// Executes a SQL query on the database.
	/// Should only be used to receive 1 datatype. For arrays, use QueryArrayValue.
	/// </summary>
	/// <typeparam name="T">: Datatype to be receiving</typeparam>
	/// <param name="transaction">: Transaction of the database to be used</param>
	/// <param name="sqlQuery">: SQL query to be executed</param>
	/// <returns>Returns type T from the database</returns>
	template <typename T>
	T QueryValue(pqxx::transaction_base* transaction, pqxx::zview sqlQuery)
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

	/// <summary>
	/// Executes a SQL query on the database to retreived an array of type T.
	/// Should only be used to receive an array of a specific datatype. For single values, use QueryValue.
	/// </summary>
	/// <typeparam name="T">: Datatype to be received</typeparam>
	/// <param name="transaction">: Transaction of the database to be used</param>
	/// <param name="sqlQuery">: SQL query to be executed</param>
	/// <returns>Returns a vector of type T from the database</returns>
	template <typename T>
	std::vector<T> QueryArrayValue(pqxx::transaction_base* transaction, pqxx::zview sqlQuery)
	{
		std::vector<T> result{};

		if (transaction != nullptr)
		{
			if (!sqlQuery.empty())
			{
				try
				{
					pqxx::result sqlResult = sc::Query(transaction, sqlQuery);
					if (!sqlResult.empty())
					{
						pqxx::array_parser parse = sqlResult[0][0].as_array();

						std::pair<pqxx::array_parser::juncture, std::string> temporaryState;
						do
						{
							temporaryState = parse.get_next();
							if (temporaryState.first == pqxx::array_parser::juncture::string_value)
								result.push_back(pqxx::from_string<T>(temporaryState.second));

						} while (temporaryState.first != pqxx::array_parser::juncture::done);
					}
					else
					{
						printf("ERROR: sqlQuery did not return any results\n");
						DEBUG_ASSERT(false && "sqlQuery did not return any results");
					}
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

	/// <summary>
	/// Prepares a statement and immediately executes it.
	/// Can be used for passing array data to the database.
	/// </summary>
	/// <typeparam name="...Args">: Arguments to be passed in the prepare statement</typeparam>
	/// <param name="connection">: Connection to the database to be used</param>
	/// <param name="transaction">: Transaction of the database to be used</param>
	/// <param name="sqlQuery">: SQL query to be executed</param>
	/// <param name="...args">: Arguments to be passed in the prepare statement</param>
	/// <returns>Returns pqxx::result</returns>
	template<typename... Args>
	pqxx::result Prepare(pqxx::connection* connection, pqxx::transaction_base* transaction, pqxx::zview sqlQuery, Args&&... args)
	{
		pqxx::result result{};

		if (connection != nullptr)
		{
			if (transaction != nullptr)
			{
				if (!sqlQuery.empty())
				{
					try
					{
						connection->prepare("preparedStatement", sqlQuery);
						result = transaction->exec_prepared("preparedStatement", args...);
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
		}
		else
		{
			printf("ERROR: connection is a null pointer\n");
			DEBUG_ASSERT(false && "connection is a null pointer");
		}

		return result;
	}
}