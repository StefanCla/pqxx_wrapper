#include <gtest/gtest.h>

// IMPORTANT:
// To run the unit tests, a database must be created before hand
// The database must have the following details:
// Username = postgres
// Password = admin
// Ip = localhost
// Port = 5432
// Database name = unittest

// The database must have the following tables & columns:
// TBA

//Unit tests are meant to be ran locally, not on public databases.

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}