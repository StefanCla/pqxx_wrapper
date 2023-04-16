# pqxx_wrapper
The pqxx_wrapper is a safety-wrapper library that aims to stabilize the PQXX library by encapsulating commonly used functions to always return zero initialized variables. It also prints error messages when unexpected errors occur during runtime and allows for asserts to be triggered when enable during compilation of the library.

# Goal
The goal of this wrapper is to allow developers to have an easier time working with the PQXX library. As the PQXX library often results in crashes due to incorrect SQL syntax, typos, and unavailable data. I decided to write this small library that will prevent the project from crashing and allow for better debugging within projects.

### Disclaimer, this library is not endorsed or promoted by PQXX or their associates.

# How to use
The library functions are under the **sc** namespace. 
Some examples below show how to execute a query or pass data using the prepare statement in the library.
For examples on all the functions, check the ***examples*** folder or the ***unit_test*** folder.

```c++
//Example of executing a query
std::string sqlCreateTable = "CREATE TABLE table_example (id SERIAL PRIMARY KEY, name TEXT NOT NULL)";
sc::Query(transaction, sqlCreateTable);
```

```c++
//Example of prepare statement
std::string sqlPrepareName = "INSERT INTO table_example (name) VALUES ($1)";
std::string name = "Stefan";
sc::Prepare(connection, transaction, sqlPrepareName, name);
```

# Road Map
- [X] Database connection
- [X] Transactions
- [X] Execute queries
- [X] Query single values
- [X] Query array values
- [X] Prepare statements

# Improvements
If you feel like I have missed anything, or if you noticed any issues.
Please let me know either through GitHub or Discord: Qunur#1124.
