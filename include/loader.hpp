#pragma once

#include <atomic>
#include <functional>
#include <string>
class Loader
{
  public:
	template <typename... Args, typename... FArgs>
	void doLoader(std::string taskName, std::function<void(Args...)> task, FArgs...);
	void setProgress(std::string progress);
	void fail();

  private:
	std::string taskName;
	std::string progress = "";
	std::atomic<bool> stopping = false;
	std::atomic<bool> failing = false;
};