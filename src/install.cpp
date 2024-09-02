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
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <vector>
namespace install {
void install(cxxopts::ParseResult args, Lpkg pkg) {
	int installed = 1;
	int pkgCount = 1 + splitstr(pkg.getValue("depends"), ",").size();
	std::string prettyname =
	    bold() + color(24, 24, 24) +
	    gradient(pkg.getValue("name"), new uint8_t[]{57, 158, 41},
		     new uint8_t[]{90, 181, 76}, true) +
	    colorTerminate();
	log(LogLevel::INFO, "building {}", prettyname);
	if (system(pkg.getFilename().c_str()) != 0) {
		log(LogLevel::FATAL, "failed to install {}",
		    pkg.getValue("name"));
	}
	if (args.count("make-binpkg")) {
		throw new std::logic_error("not implemented");
	} else {
		log(LogLevel::INFO, "installing {}", prettyname);
		std::string targetDir =
		    format("/usr/pkg/{}/{}", pkg.getValue("name"),
			   pkg.getValue("version"));
		std::filesystem::create_directories(targetDir);
		std::filesystem::copy(
		    "/tmp/luna/staging/" + pkg.getValue("name"), targetDir,
		    std::filesystem::copy_options::recursive |
			std::filesystem::copy_options::overwrite_existing);
		for (const auto &dirEntry :
		     std::filesystem::recursive_directory_iterator(targetDir)) {
			std::filesystem::directory_entry dest(
			    replace(dirEntry.path(), targetDir, ""));
			if (dirEntry.is_regular_file()) {
				if (args.count("pretend")) {
					log(LogLevel::INFO, "{} -> {}",
					    dirEntry.path(), dest.path());
				} else {
					std::filesystem::remove(dest.path());
					std::filesystem::create_symlink(
					    dirEntry.path(), dest.path());
				}
			} else if (dirEntry.is_directory() && !dest.exists() &&
				   !args.count("pretend")) {
				std::filesystem::create_directories(
				    dest.path());
			}
		}
	}
}
void installPackage(cxxopts::ParseResult args) {
	privEsc();
	Lpkg lpkg = ParseLpkg(args["install"].as<std::string>()).parse();
	install(args, lpkg);
}
} // namespace install
