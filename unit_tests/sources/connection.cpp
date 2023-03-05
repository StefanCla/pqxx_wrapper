#include <pqxx/pqxx>
#include <pqxx_wrapper.hpp>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

//Test database connection

namespace sc
{

	TEST(Connection, MUT_CONNECTION_CORRECT)
	{
		std::string url = "postgresql://postgres:admin@localhost:5432/unittest";
		pqxx::connection* con = sc::ConnectDatabase(url);

		EXPECT_FALSE(con == nullptr);

		delete con;
	}

	TEST(Connection, MUT_CONNECTION_NULL)
	{
		std::string url = "";
		pqxx::connection* con = sc::ConnectDatabase(url);

		EXPECT_TRUE(con == nullptr);
	}

	TEST(Connection, MUT_CONNECTION_INCORRECT)
	{
		std::string url = "postgreasql://postgreeees:admin@locqlhost:5432/unuttest";
		pqxx::connection* con = sc::ConnectDatabase(url);

		EXPECT_TRUE(con == nullptr);
	}

}