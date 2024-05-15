#include "logger.hpp"
#include <parseargs.hpp>
#include <string>
#include <vector>

namespace install
{
void installPackage(std::vector<std::string> args)
{
    log(LogLevel::DEBUG, "{}", args.at(0));
}
void install(std::string name)
{
}
} // namespace install
