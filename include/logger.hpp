#pragma once
#include "lutils.hpp"
#include <deque>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
enum LogLevel { DEBUG, INFO, WARN, ERR, FATAL };

template <typename... Args>
void log(LogLevel lv, std::string fmt, Args... args) {
	std::string formatted = format(fmt, args...);
	std::string pretty = "";
	switch (lv) {
		case WARN:
			pretty +=
			    color(235, 80, 80) + "warn" + colorTerminate();
			break;
		case ERR:
			pretty += color(255, 0, 0) + "error" + colorTerminate();
			break;
		case FATAL:
			pretty += color(255, 255, 255) + colorBg(235, 80, 80) +
				  bold() + "fatal" + colorTerminate();
			break;
		case DEBUG:
			pretty += color(127, 127, 127) +
				  colorBg(164, 164, 164) + bold() + "dbg" +
				  colorTerminate();
			break;
		default:
			break;
	}
	if (pretty != "") {
		pretty = "[" + pretty + "] ";
	}
	pretty = pretty + formatted;
	std::cout << pretty.c_str() << std::endl;

	if (lv == LogLevel::FATAL) {
		std::cout << "a fatal exception has occurred. if you believe "
			     "that this is not user error, run luna --doctor "
			     "before making an issue."
			  << std::endl;
		exit(-1);
	}
}
template <typename... Args>
void log(bool shouldLog, LogLevel lv, std::string fmt, Args... args) {
	if (shouldLog) {
		log(lv, fmt, args...);
	}
}
template <typename... Args>
void log(bool shouldLog, std::function<void()> task, LogLevel lv,
	 std::string fmt, Args... args) {
	if (shouldLog) {
		task();
		log(lv, fmt, args...);
	}
}

template <typename... Args>
void log(std::function<void()> task, LogLevel lv, std::string fmt,
	 Args... args) {
	task();
	log(lv, fmt, args...);
}
