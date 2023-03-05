#include <pqxx/pqxx>
#include <pqxx_wrapper.hpp>

int main()
{
	printf("Start\n");
	std::string sql = "postgresql://postgres:admin@localhost:5432/Login";
	//pqxx::connection* con = new pqxx::connection{ sql };
	pqxx::connection* con = sc::ReturnConnection(sql);


	printf("End\n");
}