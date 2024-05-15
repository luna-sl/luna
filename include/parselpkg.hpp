#pragma once
#include <cstdio>
#include <fstream>
#include <string>
#include <vector>
#include "logger.hpp"
#include "lutils.hpp"
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
	void parse()
	{
		while (stream.get(currentTok))
		{
			switch (currentTok)
			{
				default:{
                    break;
                }
				case '#':{
                    std::string comment = runtotok('!');
					break;
                }
                case 's':{
                    std::string strname = trim(runtotok('"'));
                    std::string strval = runtotok('"');
                    log(LogLevel::DEBUG, "{} = {}", strname, strval);
                    break;
                }
                case 'a':{
                    std::string arrname = trim(runtotok('('));
                    std::vector<std::string> arrvals;
                    while(true){
                        runtotok('"');
                        arrvals.push_back(runtotok('"'));
                        if(stream.peek() == ')'){
                            break;
                        }
                    }
                    log(LogLevel::DEBUG, "{} = {}", arrname, joinstr(arrvals, ", "));
                    break;
                }
                case 'f':{
                    std::string funcname = trim(runtotok('{'));
                    std::vector<std::string> funccmds;
                    while(true){
                        funccmds.push_back(trim(runtotok('\n')));
                        if(stream.peek() == '}'){
                            break;
                        }
                    }
                    if(funccmds.at(0) == ""){
                        funccmds.erase(funccmds.begin());
                    }
                    log(LogLevel::DEBUG, "{} = {}", funcname, joinstr(funccmds, ", "));
                    break;
                }
			}
		}
	}
};