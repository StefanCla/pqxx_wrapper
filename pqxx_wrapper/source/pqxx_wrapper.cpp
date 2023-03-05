#include <pqxx_wrapper.hpp>

namespace sc
{
	pqxx::connection* ConnectDatabase(std::string url)
	{
		pqxx::connection* con = nullptr;

		try
		{
			con = new pqxx::connection(url);
		}
		catch (std::exception const& e)
		{
			std::cerr << "ERROR: " << e.what() << '\n';
			DEBUG_ASSERT(false && "Failed to establish a connection with the database");
		}

		return con;
	}
}