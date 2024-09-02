#include "cpr/api.h"
#include "cpr/cprtypes.h"
#include "cpr/response.h"
#include "loader.hpp"
#include "logger.hpp"
#include "lutils.hpp"
#include "parselpkg.hpp"
#include <algorithm>
#include <cstdint>
#include <cxxopts.hpp>
#include <exception>
#include <fstream>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <vector>
namespace install {
void install(cxxopts::ParseResult args, Lpkg pkg) {
	int installed = 1;
	int pkgCount = 1 + splitstr(pkg.getValue("depends"), ",").size();
	log(LogLevel::INFO, "building {}",
	    bold() + color(24, 24, 24) +
		gradient(pkg.getValue("name"), new uint8_t[]{57, 158, 41},
			 new uint8_t[]{90, 181, 76}, true) +
		colorTerminate());
	if (system(pkg.getFilename().c_str()) != 0) {
		log(LogLevel::FATAL, "failed to install {}",
		    pkg.getValue("name"));
	}
	log(LogLevel::INFO, "installing {}", pkg.getValue("name"));
}
void installPackage(cxxopts::ParseResult args) {
	privEsc();
	Lpkg lpkg = ParseLpkg(args["install"].as<std::string>()).parse();
	install(args, lpkg);
}
} // namespace install
