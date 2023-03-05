#include <pqxx/pqxx>
#include <pqxx_wrapper.hpp>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

//Test database connection

namespace sc
{

	TEST(Connection, success)
	{
		std::string url = "postgresql://postgres:admin@localhost:5432/unittest";
		pqxx::connection* con = sc::ConnectDatabase(url);

		EXPECT_TRUE(con);

		delete con;
	}

	TEST(Connection, failed)
	{
		std::string url = "postgreasql://postgreeees:admin@locqlhost:5432/unuttest";
		pqxx::connection* con = sc::ConnectDatabase(url);

		EXPECT_FALSE(con);

		if (con != nullptr)
			delete con;
	}

}