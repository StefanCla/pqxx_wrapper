#include <pqxx/pqxx>
#include <pqxx_wrapper.hpp>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

//Test obtaining transactions

namespace scTest
{

	class ConnectionTestF : public ::testing::Test
	{
	public:
		ConnectionTestF() :
			correctCon(nullptr),
			nullptrCon(nullptr),
			invalidCon(nullptr)
		{
			std::string url = "postgresql://postgres:admin@localhost:5432";
			correctCon = sc::ConnectDatabase(url);

			//Results in errors (which is good) but will cause the body of the unit tests to fail as well
			//invalidCon = new pqxx::connection();
		}

		~ConnectionTestF()
		{
			delete correctCon;
			delete invalidCon;
		}

		pqxx::connection* correctCon;
		pqxx::connection* nullptrCon;
		pqxx::connection* invalidCon;
	};

	//GetTransaction
	TEST_F(ConnectionTestF, MUT_TRANSACTION_CORRECT)
	{
		pqxx::work* transaction = sc::GetTransaction(correctCon);

		EXPECT_FALSE(transaction == nullptr);

		delete transaction;
	}

	TEST_F(ConnectionTestF, MUT_TRANSACTION_DB_NULL)
	{
		pqxx::work* transaction = sc::GetTransaction(nullptrCon);

		EXPECT_TRUE(transaction == nullptr);
	}

	//Can not be tested due to body or fixture class errors by pqxx
	//TEST_F(ConnectionTestF, MUT_TRANSACTION_DB_INVALID)
	//{
	//	pqxx::work* transaction = sc::GetTransaction(invalidCon);

	//	EXPECT_TRUE(transaction == nullptr);
	//}

	//GetNonTransaction
	TEST_F(ConnectionTestF, MUT_NONTRANSACTION_CORRECT)
	{
		pqxx::nontransaction* nonTransaction = sc::GetNonTransaction(correctCon);

		EXPECT_FALSE(nonTransaction == nullptr);

		delete nonTransaction;
	}

	TEST_F(ConnectionTestF, MUT_NONTRANSACTION_DB_NULL)
	{
		pqxx::nontransaction* nonTransaction = sc::GetNonTransaction(nullptrCon);

		EXPECT_TRUE(nonTransaction == nullptr);
	}

	//Can not be tested due to body or fixture class errors by pqxx
	//TEST_F(ConnectionTestF, MUT_NONTRANSACTION_DB_INVALID)
	//{
	//	pqxx::nontransaction* nonTransaction = sc::GetNonTransaction(invalidCon);

	//	EXPECT_TRUE(nonTransaction == nullptr);
	//}

}