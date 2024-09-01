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
void install(cxxopts::ParseResult args, Lpkg::Lpkg pkg) {
	Loader L = Loader();
	std::function<void()> func = [&]() {
		L.setProgress("downloading tarball");
		cpr::Response r = cpr::Get(cpr::Url{
		    std::get<Lpkg::String>(pkg.at("source")).getContents()});
		log(
		    r.error.code != cpr::ErrorCode::OK, [&L]() { L.fail(); },
		    LogLevel::FATAL, "failed to get {} because: {}",
		    r.url.str().substr(r.url.str().find_last_of("/") + 1),
		    r.error.message);
		log(
		    r.status_code != 200, [&L]() { L.fail(); }, LogLevel::FATAL,
		    "failed to get {} because request failed with response "
		    "code: {}",
		    r.url.str().substr(r.url.str().find_last_of("/") + 1),
		    r.status_code);
		std::string path = format(
		    "/tmp/{}",
		    std::get<Lpkg::String>(pkg.at("name")).getContents());
		log(LogLevel::DEBUG, path);
		std::ofstream sourceOut(path);
		if (sourceOut.is_open()) {
			sourceOut << r.text;
			sourceOut.close();
		} else {
			log([&L]() { L.fail(); }, LogLevel::FATAL,
			    "sourceOut not open");
		}
		L.setProgress("extracting tarball");
		std::string extractedPath = format(
		    "/tmp/luna/build/{}/",
		    std::get<Lpkg::String>(pkg.at("name")).getContents());
		extract(path.c_str(), 0, extractedPath.c_str());
		L.setProgress("building");
		log(
		    system(
			format("cd {} && {}", extractedPath,
			       joinstr(std::get<Lpkg::Function>(pkg.at("build"))
					   .getContents(),
				       " && "))
			    .c_str()) != 0,
		    [&L]() { L.fail(); }, LogLevel::FATAL, "Build failed");
	};
	L.doLoader(format("installing {}",
			  std::get<Lpkg::String>(pkg.at("name")).getContents()),
		   func);
}
void installPackage(cxxopts::ParseResult args) {
	privEsc();
	std::ifstream lpkgFile(args["install"].as<std::string>());
	if (lpkgFile.good()) {
		Lpkg::Lpkg lpkg = ParseLpkg(lpkgFile).parse();
		install(args, lpkg);
	}
}
} // namespace install
