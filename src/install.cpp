#include "cpr/api.h"
#include "cpr/cprtypes.h"
#include "cpr/response.h"
#include "loader.hpp"
#include "logger.hpp"
#include "lutils.hpp"
#include "parselpkg.hpp"
#include <algorithm>
#include <cxxopts.hpp>
#include <exception>
#include <fstream>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <vector>
namespace install {
void install(cxxopts::ParseResult args, Lpkg pkg) {
	log(LogLevel::DEBUG, pkg.getValue("name"));
}
void installPackage(cxxopts::ParseResult args) {
	privEsc();
	Lpkg lpkg = ParseLpkg(args["install"].as<std::string>()).parse();
	install(args, lpkg);
}
} // namespace install
