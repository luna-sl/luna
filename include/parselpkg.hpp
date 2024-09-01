#pragma once
#include "logger.hpp"
#include "lutils.hpp"
#include <fstream>
#include <map>
#include <string>
#include <variant>
#include <vector>

namespace Lpkg {
enum Type { STRING, ARRAY, FUNCTION };
class Value {

      private:
	Type ty;

      public:
	virtual ~Value() {}
	Value(Type ty) : ty(ty) {}
	Type getType() { return ty; }
};
class String : public Value {
      private:
	std::string contents;
	typedef Value super;

      public:
	String(std::string content) : super(Type::STRING), contents(content) {}
	std::string getContents() { return contents; }
};
class Array : public Value {
      private:
	std::vector<std::string> contents;
	typedef Value super;

      public:
	Array(std::vector<std::string> content)
	    : super(Type::ARRAY), contents(content) {}
	std::vector<std::string> getContents() { return contents; }
};
class Function : public Value {
      private:
	std::vector<std::string> contents;
	typedef Value super;

      public:
	Function(std::vector<std::string> content)
	    : super(Type::FUNCTION), contents(content) {}
	std::vector<std::string> getContents() { return contents; }
};
using Lpkg = std::map<std::string,
		      std::variant<Lpkg::String, Lpkg::Array, Lpkg::Function>>;
} // namespace Lpkg
class ParseLpkg {
      private:
	std::ifstream &stream;
	char currentTok;
	std::string runtotok(char target) {
		std::string skipped = "";
		while (stream.get(currentTok)) {
			if (currentTok == target) {
				break;
			} else {
				skipped += currentTok;
			}
		}
		return skipped;
	}

      public:
	ParseLpkg(std::ifstream &stream) : stream(stream) {}
	Lpkg::Lpkg parse() {
		Lpkg::Lpkg result;
		while (stream.get(currentTok)) {
			switch (currentTok) {
				default: {
					break;
				}
				case '#': {
					std::string comment = runtotok('!');
					break;
				}
				case 's': {
					std::string strname =
					    trim(runtotok('"'));
					std::string strval = runtotok('"');
					result.insert(
					    {strname, Lpkg::String(strval)});
					break;
				}
				case 'a': {
					std::string arrname =
					    trim(runtotok('('));
					std::vector<std::string> arrvals;
					while (true) {
						runtotok('"');
						arrvals.push_back(
						    runtotok('"'));
						if (stream.peek() == ')') {
							break;
						}
					}
					result.insert(
					    {arrname, Lpkg::Array(arrvals)});
					break;
				}
				case 'f': {
					std::string funcname =
					    trim(runtotok('{'));
					std::vector<std::string> funccmds;
					while (true) {
						funccmds.push_back(
						    trim(runtotok('\n')));
						if (stream.peek() == '}') {
							break;
						}
					}
					if (funccmds.at(0) == "") {
						funccmds.erase(
						    funccmds.begin());
					}
					result.insert(
					    {funcname,
					     Lpkg::Function(funccmds)});
					break;
				}
			}
		}
		return result;
	}
};