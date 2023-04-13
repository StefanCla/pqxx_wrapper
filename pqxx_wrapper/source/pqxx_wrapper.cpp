#include <pqxx_wrapper.hpp>

namespace sc
{
	pqxx::connection* ConnectDatabase(std::string url)
	{
		pqxx::connection* con = nullptr;

		if (!url.empty())
		{
			try
			{
				con = new pqxx::connection(url);
			}
			catch (std::exception const& e)
			{
				std::cerr << "ERROR: " << e.what() << '\n';
				DEBUG_ASSERT(false && "Failed to establish a connection with the database");
			}
		}
		else
		{
			printf("ERROR: url is empty\n");
			DEBUG_ASSERT(false && "url is empty");
		}

		return con;
	}

	pqxx::work* GetTransaction(pqxx::connection* con)
	{
		pqxx::work* transaction = nullptr;

		if (con != nullptr)
		{
			if (con->dbname() != nullptr)
			{
				try
				{
					transaction = new pqxx::work(*con);
				}
				catch (std::exception const& e)
				{
					std::cerr << "ERROR: " << e.what() << '\n';
					DEBUG_ASSERT(false && "An error has occured when getting a worker");
				}
			}
			else
			{
				printf("ERROR: con has no valid connection to a database\n");
				DEBUG_ASSERT(false && "con has no valid connection to a database");
			}
		}
		else
		{
			printf("ERROR: con is a null pointer\n");
			DEBUG_ASSERT(false && "con is a null pointer");
		}

		return transaction;
	}

	pqxx::nontransaction* GetNonTransaction(pqxx::connection* con)
	{
		pqxx::nontransaction* nonTransaction = nullptr;

		if (con != nullptr)
		{
			if (con->dbname() != nullptr)
			{
				try
				{
					nonTransaction = new pqxx::nontransaction(*con);
				}
				catch (std::exception const& e)
				{
					std::cerr << "ERROR: " << e.what() << '\n';
					DEBUG_ASSERT(false && "An error has occured when getting a nontransaction");
				}
			}
			else
			{
				printf("ERROR: con has no valid connection to a database\n");
				DEBUG_ASSERT(false && "con has no valid connection to a database");
			}
		}
		else
		{
			printf("ERROR: con is a null pointer\n");
			DEBUG_ASSERT(false && "con is a null pointer");
		}

		return nonTransaction;
	}

	pqxx::result Query(pqxx::transaction_base* transaction, std::string sqlQuery)
	{
		pqxx::result result{};

		if (transaction != nullptr)
		{
			if (!sqlQuery.empty())
			{
				try
				{
					result = transaction->exec(sqlQuery);
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