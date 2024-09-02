#include <filesystem>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "cpr/cpr.h"
#include "loader.hpp"
#include "logger.hpp"
#include "lutils.hpp"
namespace doctor {
void runDoctor() {
	privEsc();
	Loader L = Loader();
	std::function<void()> func = [&]() {
		int issues = 0;
		if (!std::filesystem::exists("/etc/luna/")) {
			++issues;
			L.setProgress("creating /etc/luna/");
			std::filesystem::create_directory("/etc/luna");
		}
		if (!std::filesystem::exists("/usr/pkg")) {
			++issues;
			L.setProgress("creating /usr/pkg");
			std::filesystem::create_directory("/usr/pkg");
		}
		if (!std::filesystem::exists("/var/lib/luna/repos.d/")) {
			++issues;
			L.setProgress("creating /var/lib/luna/repos.d/");
			std::filesystem::create_directories(
			    "/var/lib/luna/repos.d/");
		}
		if (!std::filesystem::exists("/etc/luna/repos.conf")) {
			++issues;
			L.setProgress("getting repos.conf");
			cpr::Response r = cpr::Get(cpr::Url{
			    "https://raw.githubusercontent.com/luna-lnx/repo/"
			    "main/repos.conf.defaults"});
			log(
			    r.error.code != cpr::ErrorCode::OK,
			    [&L]() { L.fail(); }, LogLevel::FATAL,
			    "failed to get {} because: {}",
			    r.url.str().substr(r.url.str().find_last_of("/") +
					       1),
			    r.error.message);
			log(
			    r.status_code != 200, [&L]() { L.fail(); },
			    LogLevel::FATAL,
			    "failed to get {} because request failed with "
			    "response "
			    "code: {}",
			    r.url.str().substr(r.url.str().find_last_of("/") +
					       1),
			    r.status_code);
			std::ofstream reposConfOut("/etc/luna/repos.conf");
			reposConfOut << r.text;
			reposConfOut.close();
		}
		log(issues > 0, LogLevel::INFO, "fixed {}{}{} issue(s)",
		    color(255, 145, 145) + bold(), issues, colorTerminate());
		log(issues == 0, LogLevel::INFO, "found {}no{} issues (yay)",
		    color(125, 255, 125) + bold(), colorTerminate());
	};
	L.doLoader("running doctor", func);
}
} // namespace doctor
  // https://raw.githubusercontent.com/luna-lnx/repo/main/repos.conf.defaults