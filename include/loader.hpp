#pragma once

#include <atomic>
#include <functional>
#include <string>
class Loader {
      public:
	void doLoader(std::string taskName, std::function<void()> &task);
	void setProgress(std::string progress);
	void fail();

      private:
	std::string taskName;
	std::string progress = "";
	std::atomic<bool> stopping = false;
	std::atomic<bool> failing = false;
};