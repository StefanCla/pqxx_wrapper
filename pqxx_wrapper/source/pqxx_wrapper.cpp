#include <pqxx_wrapper.hpp>

namespace sc
{
	pqxx::connection* ReturnConnection(std::string sqlConnect)
	{
		pqxx::connection* con = nullptr;

		con = new pqxx::connection{ sqlConnect };

		return con;
	}
}