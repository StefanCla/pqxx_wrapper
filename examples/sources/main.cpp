#include <pqxx/pqxx>
#include <pqxx_wrapper.hpp>

//Enabled the define to allow for asserts to be triggered.
//Useful for debugging instead of testing.

int main()
{
	//Connect to the database
	//Get details
	printf("Establish a connection with the database...\n");

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

	//Create new table
	std::string sqlCreateTable = "CREATE TABLE exampletable (id SERIAL PRIMARY KEY, text TEXT NOT NULL, int INTEGER NOT NULL)";
	sc::Query(nonTransaction, sqlCreateTable);

	printf("A table has been created!\n");

	//Insert data in new table
	printf("Player name...\n");
	std::string playername;
	std::cin >> playername;

	printf("Age...\n");
	std::string age;
	std::cin >> age;

	std::string sqlInsertTable = "INSERT INTO exampletable (text, int) VALUES ('" + playername + "', '" + age + "')";
	sc::Query(nonTransaction, sqlInsertTable);

	printf("Data has been inserted into the database!\n");

	//Obtain data from new table
	std::string sqlSelectName = "SELECT text FROM exampletable WHERE id = 1";
	std::string insertedName = sc::QueryValue<std::string>(nonTransaction, sqlSelectName);

	std::string sqlSelectAge = "SELECT int FROM exampletable WHERE id = 1";
	int insertedAge = sc::QueryValue<int>(nonTransaction, sqlSelectAge);

	printf("User #1 has: name - %s and age - %d\n", insertedName.c_str(), insertedAge);

	//Create new array table
	std::string sqlCreateArrayTable = "CREATE TABLE examplearraytable (id SERIAL PRIMARY KEY, text TEXT[] NOT NULL, int INTEGER[] NOT NULL)";
	sc::Query(nonTransaction, sqlCreateArrayTable);

	printf("An array table has been created!\n");

	//Insert data into the array table
	printf("Name...\n");
	std::string name;
	std::cin >> name;

	printf("Surname...\n");
	std::string surname;
	std::cin >> surname;

	printf("Favourite number...\n");
	int favNum;
	std::cin >> favNum;

	printf("Least favourite number...\n");
	int lFavNum;
	std::cin >> lFavNum;

	std::vector<std::string> user;
	user.push_back(name);
	user.push_back(surname);

	std::vector<int> numbers;
	numbers.push_back(favNum);
	numbers.push_back(lFavNum);

	std::string sqlInsertArrays = "INSERT INTO examplearraytable (text, int) VALUES ($1, $2)";

	sc::Prepare(con, nonTransaction, sqlInsertArrays, user, numbers);

	printf("Data has been inserted into the database!\n");

	//Retreive array data
	std::string sqlSelectArrayUser = "SELECT text from examplearraytable WHERE id = 1";
	std::vector<std::string> userData = sc::QueryArrayValue<std::string>(nonTransaction, sqlSelectArrayUser);

	std::string sqlSelectArrayNumber = "SELECT int from examplearraytable WHERE id = 1";
	std::vector<int> numberData = sc::QueryArrayValue<int>(nonTransaction, sqlSelectArrayNumber);

	printf("Printing array data from the database...\n");
	for (int i = 0; i < userData.size(); i++)
		printf("User: %s\n", userData[i].c_str());

	for (int i = 0; i < numberData.size(); i++)
		printf("Numbers: %d\n", numberData[i]);

	//Close connection
	con->close();
	delete con;
	printf("Connection has been closed!\n");
}