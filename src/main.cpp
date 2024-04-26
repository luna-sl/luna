#include "doctor.hpp"
#include "install.hpp"
#include "liblpkg.hpp"
#include "logger.hpp"
#include "lutils.hpp"
#include "parseargs.hpp"
#include "update.hpp"
#include <deque>
#include <iostream>
#include <unistd.h>
#define VERS "v0.1"

int ARGC;
char **ARGV;

int main(int argc, char *argv[])
{
	if (argc <= 1)
	{
		log(LogLevel::FATAL, "no arguments provided");
	}
	ARGC = argc;
	ARGV = argv;
	std::vector<std::string> arguments(argv + 1, argv + argc);
	ParseArgs pa;
	pa.addArgument("-i|--install|install|-S", "installs a package", &install::installPackage);
	pa.addArgument("-u|--update|update|-y", "updates the repos", &update::updateRepos);
	pa.addArgument("-d|--doctor|doctor", "performs a lunapm health check", &doctor::runDoctor);
	pa.setFlags(ParseArgs::STOP_ON_MATCH);
	log(!pa.parseArgs(arguments), LogLevel::FATAL, "no argument matched");
	return 0;
}
