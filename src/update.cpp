#include "cpr/cpr.h"
#include "loader.hpp"
#include "logger.hpp"
#include "lutils.hpp"
#include <fstream>
#include <iostream>
#include <string>

namespace update
{
void updateRepos()
{
	privEsc();
	Loader L = Loader();
	std::function<void()> func = [&]() {
		std::ifstream reposListFile("/etc/luna/repos.conf");
		if (reposListFile.is_open())
		{
			std::stringstream buffer;
			buffer << reposListFile.rdbuf();
			std::string tmp = trim(buffer.str());
			std::vector<std::string> reposList = splitstr(tmp, "\n");
			for (int i = 0; i < reposList.size(); ++i)
			{
				L.setProgress(format("{}/{}", i + 1, reposList.size()));
				cpr::Response r = cpr::Get(cpr::Url{reposList.at(i)});
				log(
					r.error.code != cpr::ErrorCode::OK, [&L]() { L.fail(); }, LogLevel::FATAL,
					"failed to get {} because: {}", r.url.str().substr(r.url.str().find_last_of("/") + 1),
					r.error.message);
				log(
					r.status_code != 200, [&L]() { L.fail(); }, LogLevel::FATAL,
					"failed to get {} because request failed with response code: {}",
					r.url.str().substr(r.url.str().find_last_of("/") + 1), r.status_code);
				std::ofstream repoOut(
					format("/var/lib/luna/repos.d/{}", r.url.str().substr(r.url.str().find_last_of("/") + 1)));
				if (repoOut.is_open())
				{
					repoOut << r.text;
					repoOut.close();
				}
				else
				{
					log([&L]() { L.fail(); }, LogLevel::FATAL, "repoOut not open");
				}
			}
		}
		else
		{
			log([&L]() { L.fail(); }, LogLevel::FATAL, "reposListFile not open");
		}
		reposListFile.close();
	};
	L.doLoader("updating repos", func);
}
} // namespace update
