#include "liblpkg.hpp"
#include "logger.hpp"
#include <fstream>
#include <sstream>
#include <string.h>

namespace liblpkg
{
Lpkg parse_lpkg(std::string str)
{
	
}
Lpkg parse_lpkg_from_file(std::string path)
{
	std::ifstream file(path);
	std::stringstream stream;
	stream << file.rdbuf();
	parse_lpkg(stream.str());
}
} // namespace liblpkg