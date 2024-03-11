#ifndef CONFPARSER_H
#define CONFPARSER_H

#include <map>
#include <string>

using confmap = std::map<const std::string, const std::string>;
using confentry = std::pair<const std::string, const std::string>;
confmap parseConf(const std::string path);

#endif