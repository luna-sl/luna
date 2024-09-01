#pragma once

#include <cxxopts.hpp>
#include <string>
#include <vector>

namespace install {
void installPackage(cxxopts::ParseResult args);
}