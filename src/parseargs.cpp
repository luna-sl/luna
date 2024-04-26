#include "parseargs.hpp"
#include "logger.hpp"
#include "lutils.hpp"
#include <deque>
#include <optional>
#include <string>

Arg::Arg(std::string names, std::string desc, Func func, int flags)
{
	this->names = names;
	this->desc = desc;
	this->flags = flags;
	this->value = func;
}
Arg::Arg(std::string names, std::string desc, bool *val, int flags)
{
	this->names = names;
	this->desc = desc;
	this->flags = flags;
	this->value = val;
}
Arg::Arg(std::string names, std::string desc, std::string *val, int flags)
{
	this->names = names;
	this->desc = desc;
	this->flags = flags;
	this->value = val;
}
void ParseArgs::addArgument(std::string names, std::string desc, Arg::Func func, int flags)
{
	arguments.push_back(Arg(names, desc, func, flags));
}
void ParseArgs::addArgument(std::string names, std::string desc, bool *val, int flags)
{
	arguments.push_back(Arg(names, desc, val, flags));
}
void ParseArgs::addArgument(std::string names, std::string desc, std::string *val, int flags)
{
	arguments.push_back(Arg(names, desc, val, flags));
}
int ParseArgs::hasArgument(std::string arg)
{
	for (int i = 0; i < arguments.size(); ++i)
	{
		std::vector<std::string> indivargs = splitstr(arguments.at(i).names, "|");
		if (std::find(indivargs.begin(), indivargs.end(), arg) != indivargs.end())
		{
			if (std::holds_alternative<std::string *>(arguments.at(i).value))
			{
				return 2;
			}
			return 1;
		}
	}
	return 0;
}
void ParseArgs::checkUnrecognized(std::vector<std::string> argsin)
{
	// TODO: Add support for string args with checkUnrecognized
	for (int i = 0; i < argsin.size(); ++i)
	{
		int hasArg = hasArgument(argsin.at(i));
		if (!hasArg && hasArg != 2)
		{
			log(LogLevel::FATAL, "unrecognized argument {}", argsin.at(i));
		}
		else if (hasArg == 2)
		{
			++i;
		}
	}
}
bool ParseArgs::parseArgs(std::vector<std::string> argsin)
{
	bool matched = false;
	if (this->flags & ParseArgs::CHECK_UNRECOGNIZED)
	{
		checkUnrecognized(argsin);
	}
	for (int i = 0; i < arguments.size(); ++i)
	{
		Arg arg = arguments.at(i);
		if (argsin[0] == "-h")
		{
			std::vector<std::string> namessplit = splitstr(arg.names, "|");
			namessplit.back() = "or " + namessplit.back();
			log(LogLevel::INFO, "{}     {}", joinstr(namessplit, ", "), arguments.at(i).desc);
			matched = true;
		}
		std::vector<std::string> indivargs = splitstr(arguments.at(i).names, "|");
		for (int j = 0; j < indivargs.size(); ++j)
		{
			if (argsin[0] == indivargs[j])
			{
				argsin.erase(argsin.begin());
				if (std::holds_alternative<Arg::Func>(arg.value))
				{
					std::get<Arg::Func>(arg.value)(argsin);
				}
				else if (std::holds_alternative<bool *>(arg.value))
				{
					bool *boolPtr = std::get<bool *>(arg.value);
					*boolPtr = !(*boolPtr);
				}
				else if (std::holds_alternative<std::string *>(arg.value))
				{
					std::string *stringPtr = std::get<std::string *>(arg.value);
					stringPtr->assign(argsin[0]);
					argsin.erase(argsin.begin());
				}
				if (this->flags & ParseArgs::STOP_ON_MATCH)
				{
					return true;
				}
				matched = true;
				break;
			}
		}
	}
	if (matched)
		return true;
	return false;
};

void ParseArgs::setFlags(int flags)
{
	this->flags = flags;
}