#include <pqxx/pqxx>
#include <pqxx_wrapper.hpp>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

//Test querying SQL scripts that return an array of values

namespace sc
{
	class QueryArrayValueTestF : public ::testing::Test
	{
	public:
		QueryArrayValueTestF() :
			connection(nullptr),
			nonTransaction(nullptr),
			nullptrNonTransaction(nullptr),
			correctScriptInt(""),
			correctScriptString(""),
			correctScriptFloat(""),
			correctScriptChar(""),
			correctScriptBool(""),
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
			std::string setupTableScript = "CREATE TABLE unitarray (id SERIAL PRIMARY KEY, textarray TEXT[] NOT NULL, intarray INTEGER[] NOT NULL, chararray VARCHAR[] NOT NULL, floatarray FLOAT[] NOT NULL,  boolarray BOOL[] NOT NULL)";


			std::string insertTableScriptOne = "INSERT INTO unitarray (textarray, intarray, chararray, floatarray, boolarray) VALUES (ARRAY ['text1', 'text2'], ARRAY [0, 5], ARRAY ['0', '@'], ARRAY [0.0, 1.43], ARRAY [false, true])";
			std::string insertTableScriptTwo = "INSERT INTO unitarray (textarray, intarray, chararray, floatarray, boolarray) VALUES (ARRAY ['text3', 'text4'], ARRAY [53, 32], ARRAY ['y', '('], ARRAY [3.553, 9004.445], ARRAY [true, false])";

			nonTransaction->exec(setupTableScript);
			nonTransaction->exec(insertTableScriptOne);
			nonTransaction->exec(insertTableScriptTwo);

			//Initialize test values
			correctScriptInt = "SELECT intarray FROM unitarray WHERE id = 1";
			correctScriptString = "SELECT textarray FROM unitarray WHERE id = 2";
			correctScriptFloat = "SELECT floatarray FROM unitarray WHERE id = 1";
			correctScriptChar = "SELECT chararray FROM unitarray WHERE id = 2";
			correctScriptBool = "SELECT boolarray FROM unitarray WHERE id = 1";

			invalidScript = "SELECT floatarray FROM unitarray WHERE id = 3";

			selectScript = "SELECT textarry FROM unitarray WHERE id = 1";
		}

		~QueryArrayValueTestF()
		{
			delete nonTransaction;
			delete connection;
		}

		pqxx::connection* connection;
		pqxx::nontransaction* nonTransaction;
		pqxx::nontransaction* nullptrNonTransaction;
		std::string correctScriptInt;
		std::string correctScriptString;
		std::string correctScriptFloat;
		std::string correctScriptChar;
		std::string correctScriptBool;
		std::string selectScript;
		std::string emptyScript;
		std::string invalidScript;
	};


	TEST_F(QueryArrayValueTestF, MUT_QUERYARRAYVALUE_CORRECT_INT)
	{
		std::vector<int> values = sc::QueryArrayValue<int>(nonTransaction, correctScriptInt);

		EXPECT_EQ(values[0], 0);
		EXPECT_EQ(values[1], 5);
	}

	TEST_F(QueryArrayValueTestF, MUT_QUERYARRAYVALUE_CORRECT_STRING)
	{
		std::vector<std::string> values = sc::QueryArrayValue<std::string>(nonTransaction, correctScriptString);

		EXPECT_EQ(values[0], "text3");
		EXPECT_EQ(values[1], "text4");
	}

	TEST_F(QueryArrayValueTestF, MUT_QUERYARRAYVALUE_CORRECT_FLOAT)
	{
		std::vector<float> values = sc::QueryArrayValue<float>(nonTransaction, correctScriptFloat);

		EXPECT_EQ(values[0], 0.0f);
		EXPECT_EQ(values[1], 1.43f);
	}

	TEST_F(QueryArrayValueTestF, MUT_QUERYARRAYVALUE_CORRECT_BOOL)
	{
		std::vector<bool> values = sc::QueryArrayValue<bool>(nonTransaction, correctScriptBool);

		EXPECT_FALSE(values[0]);
		EXPECT_TRUE(values[1]);
	}

	////PQXX does not allow for character conversion, thus this part does not compile
	////TEST_F(QueryArrayValueTestF, MUT_QUERYARRAYVALUE_CORRECT_CHAR)
	////{
	////	std::vector<char> values = sc::QueryArrayValue<char>(nonTransaction, correctScriptChar);

	////	EXPECT_EQ(values[0], 'y');
	////	EXPECT_EQ(values[1], '(');
	////}


	TEST_F(QueryArrayValueTestF, MUT_QUERYARRAYVALUE_NULL)
	{
		//Null transaction, Correct script
		std::vector<int> valuesOne = sc::QueryArrayValue<int>(nullptrNonTransaction, correctScriptInt);
		EXPECT_EQ(valuesOne.size(), 0);

		//Correct transaction, Null script
		std::vector<float> valuesTwo = sc::QueryArrayValue<float>(nonTransaction, emptyScript);
		EXPECT_EQ(valuesTwo.size(), 0);

		//Null transaction, Null script
		std::vector<std::string> valuesThree = sc::QueryArrayValue<std::string>(nullptrNonTransaction, emptyScript);
		EXPECT_EQ(valuesThree.size(), 0);
	}


	TEST_F(QueryArrayValueTestF, MUT_QUERYARRAYVALUE_INVALID)
	{
		std::vector<int> values = sc::QueryArrayValue<int>(nonTransaction, invalidScript);
		EXPECT_EQ(values.size(), 0);
	}


	TEST_F(QueryArrayValueTestF, MUT_QUERYARRAYVALUE_WRONG_RETURN_TYPE)
	{
		////Returns a string, to an int
		std::vector<int> values = sc::QueryArrayValue<int>(nonTransaction, correctScriptString);
		EXPECT_EQ(values.size(), 0);
	}


	TEST_F(QueryArrayValueTestF, MUT_QUERYARRAYVALUE_WRONG_SCRIPT_TYPE)
	{
		std::vector<std::string> values = sc::QueryArrayValue<std::string>(nonTransaction, selectScript);
		EXPECT_EQ(values.size(), 0);
	}
}