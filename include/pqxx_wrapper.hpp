#include <pqxx/pqxx>

namespace sc
{
	//Test function
	pqxx::connection* ReturnConnection(std::string sqlConnect);
}
