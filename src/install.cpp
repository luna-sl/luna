#include "cpr/api.h"
#include "cpr/cprtypes.h"
#include "cpr/response.h"
#include "loader.hpp"
#include "logger.hpp"
#include "lutils.hpp"
#include "parselpkg.hpp"
#include <fstream>
#include <parseargs.hpp>
#include <string>
#include <vector>

namespace install
{

void install(std::vector<std::string> args, Lpkg::Lpkg pkg)
{
    Loader L();
    L.doLoader(format("installing {}", pkg["name"]), [&](const Lpkg::Lpkg& pkg){
        cpr::Response file = cpr::Get(cpr::Url{pkg["source"]});
    }, pkg);
}
void installPackage(std::vector<std::string> args)
{
    privEsc();
	log(LogLevel::DEBUG, "{}", args.at(0));
	std::ifstream lpkgFile(args.at(0));
	if (lpkgFile.good())
	{
		Lpkg::Lpkg lpkg = ParseLpkg(lpkgFile).parse();
        args.erase(args.begin());
		install(args, lpkg);
	}
}
} // namespace install
