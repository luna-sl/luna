#include "cpr/api.h"
#include "cpr/cprtypes.h"
#include "cpr/response.h"
#include "loader.hpp"
#include "logger.hpp"
#include "lutils.hpp"
#include "parselpkg.hpp"
#include <exception>
#include <fstream>
#include <parseargs.hpp>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <vector>

namespace install
{
void install(std::vector<std::string> args, Lpkg::Lpkg pkg)
{
	Loader L = Loader();
	std::function<void()> func = [&]() {
		cpr::Response r = cpr::Get(cpr::Url{std::get<Lpkg::String>(pkg.at("source")).getContents()});
		log(
			r.error.code != cpr::ErrorCode::OK, [&L]() { L.fail(); }, LogLevel::FATAL, "failed to get {} because: {}",
			r.url.str().substr(r.url.str().find_last_of("/") + 1), r.error.message);
		log(
			r.status_code != 200, [&L]() { L.fail(); }, LogLevel::FATAL,
			"failed to get {} because request failed with response code: {}",
			r.url.str().substr(r.url.str().find_last_of("/") + 1), r.status_code);
		std::ofstream sourceOut(format("/tmp/{}", r.url.str().substr(r.url.str().find_last_of("/") + 1)));
		if (sourceOut.is_open())
		{
			sourceOut << r.text;
			sourceOut.close();
		}
		else
		{
			log([&L]() { L.fail(); }, LogLevel::FATAL, "sourceOut not open");
		}
	};
	L.doLoader(format("installing {}", std::get<Lpkg::String>(pkg.at("name")).getContents()), func);
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
