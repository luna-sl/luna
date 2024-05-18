#include "lutils.hpp"
#include "logger.hpp"
#include "main.hpp"
#include "parselpkg.hpp"
#include <algorithm>
#include <cmath>
#include <string>
#include <unistd.h>
#include <vector>
std::vector<std::string> splitstr(const std::string in, const std::string del)
{
	std::vector<std::string> out;
	size_t start = 0;
	size_t end = in.find(del);

	while (end != std::string::npos)
	{
		out.push_back(in.substr(start, end - start));
		start = end + del.length();
		end = in.find(del, start);
	}

	out.push_back(in.substr(start));

	return out;
}
std::string joinstr(std::vector<std::string> in, std::string delim)
{ 
	std::string out;
	for (int i = 0; i < in.size(); ++i)
	{
		out += in.at(i);
		if (i < in.size() - 1)
			out += delim;
	}
	return out;
}
std::string replace(std::string str, const std::string &from, const std::string &to)
{
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos)
	{
		str.replace(start_pos, from.length(), to);
		start_pos += to.length();
	}
	return str;
}
std::string trim(const std::string s)
{
	auto start = std::find_if_not(s.begin(), s.end(), [](unsigned char ch) { return std::isspace(ch); });

	auto end = std::find_if_not(s.rbegin(), s.rend(), [](unsigned char ch) { return std::isspace(ch); }).base();

	return (start < end ? std::string(start, end) : std::string());
}
std::string color(u_int8_t r, u_int8_t g, u_int8_t b)
{
	return "\x1b[38;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m";
}
std::string colorBg(u_int8_t r, u_int8_t g, u_int8_t b)
{
	return "\x1b[48;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m";
}
std::string bold()
{
	return "\x1b[1m";
}
std::string gradient(std::string input, u_int8_t fro[3], u_int8_t to[3], bool bg)
{
	std::string builder = "";
	for (int i = 0; i < input.length(); ++i)
	{
		// i am not mathing, ty chatgpt
		int r = fro[0] + static_cast<int>(std::round((to[0] - fro[0]) * (i / static_cast<double>(input.length() - 1))));
		int g = fro[1] + static_cast<int>(std::round((to[1] - fro[1]) * (i / static_cast<double>(input.length() - 1))));
		int b = fro[2] + static_cast<int>(std::round((to[2] - fro[2]) * (i / static_cast<double>(input.length() - 1))));
		builder += (bg ? colorBg(r, g, b) : color(r, g, b)) + input[i];
	}
	return builder;
}
std::string colorTerminate()
{
	return "\x1b[0m";
}
void privEsc()
{
	if (getuid() != 0)
	{
		std::vector<std::string> arguments(ARGV + 1, ARGV + ARGC);
		log(LogLevel::WARN, "missing permissions. attempting to rerun as {}root{}...", color(200, 145, 145) + bold(), /*"root"*/ colorTerminate());
		system(format("su -c \"{} {}\"", ARGV[0], joinstr(arguments, " ")).c_str());
		exit(0);
	}
}