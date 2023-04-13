#include <pqxx/pqxx>
#include <pqxx_wrapper.hpp>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

//Test querying SQL scripts that return a value

namespace sc
{
	class QueryValueTestF : public ::testing::Test
	{
	public:
		QueryValueTestF() :
			connection(nullptr),
			nonTransaction(nullptr),
			nullptrNonTransaction(nullptr),
			correctScriptInt(""),
			correctScriptString(""),
			correctScriptFloat(""),
			correctScriptChar(""),
			correctScriptBool(""),
			insertScript(""),
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
			std::string insertTableScriptOne = "INSERT INTO unitsingle (text, int, char, float, bool) VALUES ('text', '0', '0', '2.3', 'FALSE')";
			std::string insertTableScriptTwo = "INSERT INTO unitsingle (text, int, char, float, bool) VALUES ('content', '45', '&', '11.293', 'TRUE')";

			nonTransaction->exec(setupTableScript);
			nonTransaction->exec(insertTableScriptOne);
			nonTransaction->exec(insertTableScriptTwo);

			//Initialize test values
			correctScriptInt = "SELECT id FROM unitsingle WHERE id = 1";
			correctScriptString = "SELECT text FROM unitsingle WHERE id = 2";
			correctScriptFloat = "SELECT float FROM unitsingle WHERE id = 1";
			correctScriptChar = "SELECT char FROM unitsingle WHERE id = 2";
			correctScriptBool = "SELECT bool FROM unitsingle WHERE id = 1";

			invalidScript = "SELECT float FROM unitsingle WHERE id = 3";

			insertScript = "INSERT INTO unitsingle (text, int, char, float, bool) VALUES ('Third', '3', '%', '88.1', 'FALSE')";
		}

		~QueryValueTestF()
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
		std::string insertScript;
		std::string emptyScript;
		std::string invalidScript;
	};


	TEST_F(QueryValueTestF, MUT_QUERYVALUE_CORRECT_INT)
	{
		int value = sc::QueryValue<int>(nonTransaction, correctScriptInt);

		EXPECT_EQ(value, 1);
	}

	TEST_F(QueryValueTestF, MUT_QUERYVALUE_CORRECT_STRING)
	{
		std::string value = sc::QueryValue<std::string>(nonTransaction, correctScriptString);

		EXPECT_EQ(value, "content");
	}

	TEST_F(QueryValueTestF, MUT_QUERYVALUE_CORRECT_FLOAT)
	{
		float value = sc::QueryValue<float>(nonTransaction, correctScriptFloat);

		EXPECT_EQ(value, 2.3f);
	}

	TEST_F(QueryValueTestF, MUT_QUERYVALUE_CORRECT_BOOL)
	{
		bool value = sc::QueryValue<bool>(nonTransaction, correctScriptBool);

		EXPECT_FALSE(value);
	}

	//PQXX does not allow for character conversion, thus this part does not compile
	//TEST_F(QueryValueTestF, MUT_QUERYVALUE_CORRECT_CHAR)
	//{
	//	char value = sc::QueryValue<char>(nonTransaction, correctScriptChar);

	//	EXPECT_EQ(value, '&');
	//}


	TEST_F(QueryValueTestF, MUT_QUERYVALUE_NULL)
	{
		//Null transaction, Correct script
		int valueOne = sc::QueryValue<int>(nullptrNonTransaction, correctScriptInt);
		EXPECT_EQ(valueOne, 0);

		//Correct transaction, Null script
		float valueTwo = sc::QueryValue<float>(nonTransaction, emptyScript);
		EXPECT_EQ(valueTwo, 0.0f);

		//Null transaction, Null script
		std::string valueThree = sc::QueryValue<std::string>(nullptrNonTransaction, emptyScript);
		EXPECT_TRUE(valueThree.empty());
	}


	TEST_F(QueryValueTestF, MUT_QUERYVALYE_INVALID)
	{
		float value = sc::QueryValue<float>(nonTransaction, invalidScript);

		EXPECT_EQ(value, 0.0f);
	}


	TEST_F(QueryValueTestF, MUT_QUERYVALUE_WRONG_RETURN_TYPE)
	{
		//Returns a script, to an int
		int value = sc::QueryValue<int>(nonTransaction, correctScriptString);
		EXPECT_EQ(value, 0);
	}


	TEST_F(QueryValueTestF, MUT_QUERYVALUE_WRONG_SCRIPT_TYPE)
	{
		//Insert query
		std::string value = sc::QueryValue<std::string>(nonTransaction, insertScript);
		EXPECT_TRUE(value.empty());
	}
}