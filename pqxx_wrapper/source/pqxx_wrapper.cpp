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
			printf("ERROR: the connection url is empty\n");
			DEBUG_ASSERT(false && "the connection url is empty");
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
				printf("ERROR: Con has no valid connection to a database\n");
				DEBUG_ASSERT(false && "Con has no valid connection to a database");
			}
		}
		else
		{
			printf("ERROR: Con is a null pointer\n");
			DEBUG_ASSERT(false && "Con is a null pointer");
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
				printf("ERROR: Con has no valid connection to a database\n");
				DEBUG_ASSERT(false && "Con has no valid connection to a database");
			}
		}
		else
		{
			printf("ERROR: Con is a null pointer\n");
			DEBUG_ASSERT(false && "Con is a null pointer");
		}

		return nonTransaction;
	}
}