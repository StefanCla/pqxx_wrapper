#include <pqxx/pqxx>
#include <pqxx_wrapper.hpp>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

//Test querying SQL scripts that return an array of values

namespace sc
{
	class PrepareTestF : public ::testing::Test
	{
	public:
		PrepareTestF() :
			connection(nullptr),
			nullConnection(nullptr),
			nonTransaction(nullptr),
			nullptrNonTransaction(nullptr),
			correctScriptInsert(""),
			correctScriptUpdate(""),
			selectScript(""),
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
			std::string setupTableScript = "CREATE TABLE unitcombo (id SERIAL PRIMARY KEY, text TEXT NOT NULL, textarray TEXT[] NOT NULL, int INTEGER NOT NULL, intarray INTEGER[] NOT NULL)";
			nonTransaction->exec(setupTableScript);

			std::string insertTableScript = "INSERT INTO unitcombo (text, textarray, int, intarray) VALUES ($1, $2, $3, $4)";
			connection->prepare("init", insertTableScript);

			std::string valueOne = "stringOne";
			std::vector<std::string> valueTwo;
			valueTwo.push_back("stringTwo");
			valueTwo.push_back("stringThree");

			int valueThree = 0;
			std::vector<int> valueFour;
			valueFour.push_back(1);
			valueFour.push_back(2);

			nonTransaction->exec_prepared("init", valueOne, valueTwo, valueThree, valueFour);

			//Initialize test values
			correctScriptInsert = "INSERT INTO unitcombo (text, textarray, int, intarray) VALUES ($1, $2, $3, $4)";
			correctScriptUpdate = "UPDATE unitcombo SET text = $1, textarray = $2, int = $3, intarray = $4 WHERE id = 1";

			invalidScript = "UPDATE unitcombo SET text = $1, textarray = $2, int = $3, intarray = $4 WHERE id = 3";

			selectScript = "SELECT text FROM unitcombo WHERE id = 1";
		}

		~PrepareTestF()
		{
			delete nonTransaction;
			delete connection;
		}

		pqxx::connection* connection;
		pqxx::connection* nullConnection;
		pqxx::nontransaction* nonTransaction;
		pqxx::nontransaction* nullptrNonTransaction;
		std::string correctScriptInsert;
		std::string correctScriptUpdate;
		std::string selectScript;
		std::string emptyScript;
		std::string invalidScript;
	};


	TEST_F(PrepareTestF, MUT_PREPARE_CORRECT_INSERT)
	{
		//Setup prepare variables
		std::string valueOne = "Text1";
		std::vector<std::string> valueTwo;
		valueTwo.push_back("Text2");
		valueTwo.push_back("Text3");

		int valueThree = 5;
		std::vector<int> valueFour;
		valueFour.push_back(10);
		valueFour.push_back(15);

		pqxx::result result = sc::Prepare(connection, nonTransaction, correctScriptInsert, valueOne, valueTwo, valueThree, valueFour);
		EXPECT_EQ(result.affected_rows(), 1);
	}

	TEST_F(PrepareTestF, MUT_PREPARE_CORRECT_UPDATE)
	{
		//Setup prepare variables
		std::string valueOne = "Text4";
		std::vector<std::string> valueTwo;
		valueTwo.push_back("Text5");
		valueTwo.push_back("Text6");

		int valueThree = 20;
		std::vector<int> valueFour;
		valueFour.push_back(25);
		valueFour.push_back(30);

		pqxx::result result = sc::Prepare(connection, nonTransaction, correctScriptUpdate, valueOne, valueTwo, valueThree, valueFour);
		EXPECT_EQ(result.affected_rows(), 1);
	}


	TEST_F(PrepareTestF, MUT_PREPARE_NULL)
	{
		//Setup prepare variables
		std::string valueOne = "Text7";
		std::vector<std::string> valueTwo;
		valueTwo.push_back("Text8");
		valueTwo.push_back("Text9");

		int valueThree = 35;
		std::vector<int> valueFour;
		valueFour.push_back(40);
		valueFour.push_back(45);

		//Null connection, correct transaction, correct script
		pqxx::result resultOne = sc::Prepare(nullConnection, nonTransaction, correctScriptUpdate, valueOne, valueTwo, valueThree, valueFour);
		EXPECT_EQ(resultOne.affected_rows(), 0);
		EXPECT_TRUE(resultOne.empty());

		//Correct connection, null transaction, correct script
		pqxx::result resultTwo = sc::Prepare(connection, nullptrNonTransaction, correctScriptUpdate, valueOne, valueTwo, valueThree, valueFour);
		EXPECT_EQ(resultTwo.affected_rows(), 0);
		EXPECT_TRUE(resultTwo.empty());

		//Correct connection, correct transaction, null script
		pqxx::result resultThree = sc::Prepare(connection, nonTransaction, emptyScript, valueOne, valueTwo, valueThree, valueFour);
		EXPECT_EQ(resultThree.affected_rows(), 0);
		EXPECT_TRUE(resultThree.empty());

		//Null connection, null transaction, null script
		pqxx::result resultFour = sc::Prepare(nullConnection, nullptrNonTransaction, emptyScript, valueOne, valueTwo, valueThree, valueFour);
		EXPECT_EQ(resultFour.affected_rows(), 0);
		EXPECT_TRUE(resultFour.empty());
	}


	TEST_F(PrepareTestF, MUT_PREPARE_INVALID)
	{
		//Setup prepare variables
		std::string valueOne = "Text7";
		std::vector<std::string> valueTwo;
		valueTwo.push_back("Text8");
		valueTwo.push_back("Text9");

		int valueThree = 35;
		std::vector<int> valueFour;
		valueFour.push_back(40);
		valueFour.push_back(45);

		pqxx::result result = sc::Prepare(connection, nonTransaction, invalidScript, valueOne, valueTwo, valueThree, valueFour);
		EXPECT_EQ(result.affected_rows(), 0);
		EXPECT_TRUE(result.empty());
	}


	TEST_F(PrepareTestF, MUT_PREPARE_WRONG_SCRIPT_TYPE)
	{
		//Setup prepare variables
		std::string valueOne = "Text10";
		std::vector<std::string> valueTwo;
		valueTwo.push_back("Text11");
		valueTwo.push_back("Text12");

		int valueThree = 50;
		std::vector<int> valueFour;
		valueFour.push_back(55);
		valueFour.push_back(60);

		pqxx::result result = sc::Prepare(connection, nonTransaction, selectScript, valueOne, valueTwo, valueThree, valueFour);
		EXPECT_EQ(result.affected_rows(), 0);
		EXPECT_TRUE(result.empty());
	}
}