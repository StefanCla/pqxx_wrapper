#include <pqxx/pqxx>
#include <pqxx_wrapper.hpp>

//Enabled the define to allow for asserts to be triggered.
//Useful for debugging instead of testing.

int main()
{
	//Connect to the database
	//Get details
	printf("Username:..\n");
	std::string username;
	std::cin >> username;

	printf("Password:..\n");
	std::string password;
	std::cin >> password;

	printf("IP:..\n");
	std::string ip;
	std::cin >> ip;

	printf("Port:..\n");
	std::string port;
	std::cin >> port;

	printf("Database name:..\n");
	std::string databaseName;
	std::cin >> databaseName;

	std::string url = "postgresql://" + username + ":" + password + "@" + ip + ":" + port + "/" + databaseName;

	//Attempt connecting
	pqxx::connection* con = sc::ConnectDatabase(url);

	if (con == nullptr)
		return -1;

	printf("Connection has successfully been established!\n");

	//Get a non transaction that can commit without calling commit()
	pqxx::nontransaction* nonTransaction = sc::GetNonTransaction(con);
	if (nonTransaction == nullptr)
		return -1;

	printf("A non transaction has been created!\n");

	//Close connection
	delete con;
	printf("Connection has been closed!\n");
}