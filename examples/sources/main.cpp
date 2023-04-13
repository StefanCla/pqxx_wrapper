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

	std::string urlBase = "postgresql://" + username + ":" + password + "@" + ip + ":" + port;// +"/" + databaseName;

	//Attempt connecting
	pqxx::connection* con = sc::ConnectDatabase(urlBase);

	if (con == nullptr)
		return -1;

	printf("Connection has successfully been established!\n");

	//Get a non transaction that can commit without calling commit()
	pqxx::nontransaction* nonTransactionPrior = sc::GetNonTransaction(con);
	if (nonTransactionPrior == nullptr)
		return -1;

	printf("A non transaction has been created!\n");

	//Drop & create example database
	std::string dropDatabase = "DROP DATABASE IF EXISTS " + databaseName;
	sc::Query(nonTransactionPrior, dropDatabase);

	std::string createDatabase = "CREATE DATABASE " + databaseName;
	sc::Query(nonTransactionPrior, createDatabase);

	//Reconnect to the new database
	con->close();
	delete nonTransactionPrior;
	delete con;

	std::string urlExample = "postgresql://" + username + ":" + password + "@" + ip + ":" + port + "/" + databaseName;
	con = sc::ConnectDatabase(urlExample);

	//Create new non transcaction
	pqxx::nontransaction* nonTransaction = sc::GetNonTransaction(con);
	if (nonTransaction == nullptr)
		return -1;

	printf("A non transaction has been created!\n");

	std::string sqlCreateTable = "CREATE TABLE exampletable (id SERIAL PRIMARY KEY, text TEXT NOT NULL, int INTEGER NOT NULL)";
	sc::Query(nonTransaction, sqlCreateTable);

	printf("A table has been created!\n");

	printf("Name...\n");
	std::string name;
	std::cin >> name;

	printf("Age...\n");
	std::string age;
	std::cin >> age;

	std::string sqlInsertTable = "INSERT INTO exampletable (text, int) VALUES ('" + name + "', '" + age + "')";
	sc::Query(nonTransaction, sqlInsertTable);

	printf("Data has been inserted into the database!\n");

	//Close connection
	con->close();
	delete con;
	printf("Connection has been closed!\n");
}