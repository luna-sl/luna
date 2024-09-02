#include "cpr/api.h"
#include "cpr/cprtypes.h"
#include "cpr/response.h"
#include "cpr/util.h"
#include "logger.hpp"
#include "lutils.hpp"
#include <filesystem>
#include <string>
void fetchTarball(std::string link) {
	const std::string filename = cpr::util::split(link, '/').back();
	const std::string foldername = format("/tmp/luna/src/{}/", filename);
	cpr::Response r = cpr::Get(cpr::Url{link});
	log(r.error.code != cpr::ErrorCode::OK, LogLevel::FATAL,
	    "failed to get {} because: {}",
	    r.url.str().substr(r.url.str().find_last_of("/") + 1),
	    r.error.message);
	log(r.status_code != 200, LogLevel::FATAL,
	    "failed to get {} because request failed with response code: {}",
	    r.url.str().substr(r.url.str().find_last_of("/") + 1),
	    r.status_code);
	std::filesystem::create_directories(foldername);
	std::ofstream sourceOut(foldername + filename);
	if (sourceOut.is_open()) {
		sourceOut << r.text;
		sourceOut.close();
	} else {
		log(LogLevel::FATAL, "sourceOut not open");
	}
	extract((foldername + filename).c_str(), 0, foldername.c_str());
	log(LogLevel::INFO, foldername);
}
void makeStagingDir(std::string name) {
	std::string directory = format("/tmp/luna/staging/{}", name);
	std::filesystem::create_directories(directory);
	log(LogLevel::INFO, directory);
}