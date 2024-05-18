#include "loader.hpp"
#include "lutils.hpp"
#include <atomic>
#include <chrono>
#include <functional>
#include <iostream>
#include <stdio.h>
#include <string>
#include <thread>
template <typename... Args, typename ... FArgs> void Loader::doLoader(std::string taskName, std::function<void(Args...)> task, FArgs... fargs)
{
	this->taskName = taskName;
	std::thread loader(
		[this](std::atomic<bool> &stopping, std::atomic<bool> &failing) {
			const char *icons[4] = {"\\", "|", "/", "-"};
			while (!stopping)
			{
				if (failing)
				{
					std::cout << std::flush << std::endl;
					return;
				}
				for (int i = 0; i < 4; ++i)
				{
					std::cout << "\r" << icons[i] << " " << this->taskName
							  << format(" {}", this->progress != "" ? format("({})", this->progress) : "");
					std::cout.flush();
					// Clear the current line
					std::cout << "\r\x1b[K";
					std::this_thread::sleep_for(std::chrono::milliseconds(150));
				}
			}
			std::cout << "\r" << this->taskName << "... done!" << std::flush << std::endl;
		},
		std::ref(stopping), std::ref(failing));

	task(*this, std::forward<FArgs>(fargs)...);
	stopping.store(true);
	loader.join();
}

void Loader::setProgress(std::string progress)
{
	this->progress = progress;
}
void Loader::fail()
{
	failing.store(true);
}