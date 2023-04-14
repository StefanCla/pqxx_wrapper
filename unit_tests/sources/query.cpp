#include <pqxx/pqxx>
#include <pqxx_wrapper.hpp>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

//Test querying SQL scripts

namespace sc
{
	class QueryTestF : public ::testing::Test
	{
	public:
		QueryTestF() :
			connection(nullptr),
			nonTransaction(nullptr),
			nullptrNonTransaction(nullptr),
			corrctScript(""),
			emptyScript(""),
			invalidScript("")
		{
			//Setup connection
			std::string urlBase = "postgresql://postgres:admin@localhost:5432";
			connection = sc::ConnectDatabase(urlBase);
			nonTransaction = sc::GetNonTransaction(connection);

			//Drop & create unittest database
			std::string dropDatabaseScript = "DROP DATABASE IF EXISTS unittest";
			nonTransaction->exec(dropDatabaseScript);

			std::string setupDatabaseScript = "CREATE DATABASE unittest";
			nonTransaction->exec(setupDatabaseScript);

			//Close connection to connect with the right database
			connection->close();
			delete nonTransaction;
			delete connection;

			//Re-connect to the database
			std::string urlUnittest = "postgresql://postgres:admin@localhost:5432/unittest";
			connection = sc::ConnectDatabase(urlUnittest);
			nonTransaction = sc::GetNonTransaction(connection);

			//Create & initialize table
			std::string setupTableScript = "CREATE TABLE unitsingle (id SERIAL PRIMARY KEY, text TEXT NOT NULL, int INTEGER NOT NULL, char VARCHAR NOT NULL, float FLOAT NOT NULL,  bool BOOL NOT NULL)";
			std::string insertTableScript = "INSERT INTO unitsingle (text, int, char, float, bool) VALUES ('text', '0', '0', '2.3', 'FALSE')";

			nonTransaction->exec(setupTableScript);
			nonTransaction->exec(insertTableScript);

			//Initialize test values
			corrctScript = "UPDATE unitsingle SET text = 'updatedname', int = 5, char = '#', float = 4.3, bool = true WHERE id = 1";
			invalidScript = "UPDATE unitsingle SET text = 'updatedname2', int = 10, charizard = '@', float = 6.8, bool = true WHERE id = 1";
		}

		~QueryTestF()
		{
			delete nonTransaction;
			delete connection;
		}

		pqxx::connection* connection;
		pqxx::nontransaction* nonTransaction;
		pqxx::nontransaction* nullptrNonTransaction;
		std::string corrctScript;
		std::string emptyScript;
		std::string invalidScript;
	};

	TEST_F(QueryTestF, MUT_QUERY_CORRECT)
	{
		pqxx::result result = sc::Query(nonTransaction, corrctScript);

		EXPECT_EQ(result.affected_rows(), 1);
	}

	TEST_F(QueryTestF, MUT_QUERY_NULL)
	{
		//Null transaction, Correct script
		pqxx::result resultOne = sc::Query(nullptrNonTransaction, corrctScript);
		EXPECT_EQ(resultOne.affected_rows(), 0);

		//Correct transaction, Null script
		pqxx::result resultTwo = sc::Query(nonTransaction, emptyScript);
		EXPECT_EQ(resultTwo.affected_rows(), 0);

		//Null transaction, Null script
		pqxx::result resultThree = sc::Query(nullptrNonTransaction, emptyScript);
		EXPECT_EQ(resultThree.affected_rows(), 0);
	}

	TEST_F(QueryTestF, MUT_QUERY_INVALID)
	{
		pqxx::result result = sc::Query(nonTransaction, invalidScript);

		EXPECT_EQ(result.affected_rows(), 0);
	}

}