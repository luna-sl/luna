#pragma once

#include <deque>
#include <sstream>
#include <string>

std::deque<std::string> splitstr(std::string in, std::string del);
std::string color(u_int8_t r, u_int8_t g, u_int8_t b);
std::string colorBg(u_int8_t r, u_int8_t g, u_int8_t b);
std::string gradient(std::string input, u_int8_t fro[3], u_int8_t to[3], bool bg);
std::string bold();
std::string colorTerminate();
// what does this do again lmao
template <typename T> std::string sstr(const T &val)
{
	std::ostringstream sstr;
	// fold expression
	(sstr << std::dec << val);
	return sstr.str();
}
template <typename... Args> std::string format(std::string fmt, Args... args)
{
	std::string fmt_target = "{}";
	std::deque<std::string> argDeque;
	// https://stackoverflow.com/a/60136761
	([&]{
		argDeque.push_back(sstr(args));
	}(), ...);
	size_t found = fmt.find(fmt_target);
	int index = 0;
	while (found != std::string::npos)
	{
		fmt = fmt.replace(found, fmt_target.length(), argDeque.at(index));
		found = fmt.find(fmt_target);
		++index;
	}
	return fmt;
}