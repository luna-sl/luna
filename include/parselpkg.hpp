#pragma once
#include "liblpkg.hpp"
#include <fstream>
#include <string>
#include "logger.hpp"
class ParseLpkg
{
  private:
	std::ifstream &stream;
	char currentTok;
	std::string runtotok(char target)
	{
		std::string skipped = "";
		while (stream.get(currentTok))
		{
			if (currentTok == target)
			{
				break;
			}
			else
			{
				skipped += currentTok;
			}
		}
		return skipped;
	}

  public:
	ParseLpkg(std::ifstream &stream) : stream(stream)
	{
	}
	Lpkg parse()
	{
		while (stream.get(currentTok))
		{
			switch (currentTok)
			{
				default:
					break;
				case '#':
					std::string comment = runtotok('!');
                    log(LogLevel::DEBUG, comment);
					break;
			}
		}
	}
};