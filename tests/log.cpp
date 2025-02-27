#include "log.hpp"

int main()
{
	std::map < std::string, std::map < std::string, std::vector < std::string > > > map = 
	{	{ "A", { { "Key1", { "Value1.1",  "Value1.2" } }, { "Key2", { "Value2.1",  "Value2.2" } } } },
		{ "B", { { "Key1", { "Value1.1",  "Value1.2" } }, { "Key2", { "Value2.1",  "Value2.2" } } } },
	};
	debug::log(map, "\n");
	debug::log(debug::GetStackTrace());
	return 0;
}
