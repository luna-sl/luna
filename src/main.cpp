#include "doctor.hpp"
#include "install.hpp"
#include "logger.hpp"
#include "update.hpp"
#include <cxxopts.hpp>
#include <unistd.h>
#define VERS "v0.1"

int ARGC;
char **ARGV;

int main(int argc, char *argv[]) {
	if (argc <= 1) {
		log(LogLevel::FATAL, "no arguments provided");
	}
	ARGC = argc;
	ARGV = argv;
	cxxopts::Options options(
	    "luna", "luna - lightweight source-based package manager");
	options.add_options()("i,install", "installs a package",
			      cxxopts::value<std::string>(), "packagename")(
	    "u,update", "updates repository list")(
	    "d,doctor", "check for issues with your luna installation")(
	    "h,help", "prints this message");
	cxxopts::ParseResult result;
	try {
		result = options.parse(argc, argv);
	} catch (cxxopts::exceptions::exception ex) {
		log(LogLevel::FATAL, "failed to parse arguments: {}",
		    ex.what());
	}
	if (result.count("help")) {
		std::cout << options.help() << std::endl;
		exit(0);
	}
	if (result.count("doctor")) {
		doctor::runDoctor();
	}
	if (result.count("update")) {
		update::updateRepos();
	}
	if (result.count("install")) {
		install::installPackage(result);
	}
	return 0;
}
