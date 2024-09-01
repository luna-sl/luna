#pragma once
#include "logger.hpp"
#include "lutils.hpp"
#include <fstream>
#include <map>
#include <stdexcept>
#include <string>
#include <regex>
#include <vector>
class Lpkg {
      private:
	std::string filename;
	std::map<std::string, std::string> values{};

      public:
	std::string getFilename() { return filename; }
	void setFilename(std::string filename) { this->filename = filename; }
	std::string getValue(std::string key) {
		try {
			return values.at(key);
		} catch (std::out_of_range ex) {
			log(LogLevel::FATAL, "value {} in {} does not exist. this is an issue with the package, not luna.", key, filename);
			return 0;
		}
	}
	void addValue(std::string key, std::string value) {
		values[key] = value;
	}
};
class ParseLpkg {
      private:
	std::ifstream stream;
	std::string filename;

      public:
	ParseLpkg(std::string filename)
	    : filename(filename) {}
	Lpkg parse() {
		Lpkg result;
		stream = std::ifstream(filename);
		result.setFilename(filename);
		std::string line;
		std::regex pattern(R"(^[a-zA-Z_][a-zA-Z0-9_]*=)"); 
		if(stream.good()){
			while (getline(stream, line)) {
			if(std::regex_search(line, pattern)){
				std::vector<std::string> split = splitstr(line, "=");
				result.addValue(split.at(0), split.at(1));
			}
		}
		}
		stream.close();
		return result;
	}
};