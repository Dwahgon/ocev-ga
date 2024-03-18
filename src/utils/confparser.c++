#include "confparser.h"
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>


confmap parseConf(const std::string path){
    std::ifstream confFile(path);
    confmap confs;
    std::string line;
    if(confFile.is_open()){
        while(std::getline(confFile, line)){
            const std::size_t equalIndex = line.find('=');
            confs.insert(confentry(line.substr(0, equalIndex), line.substr(equalIndex+1)));
        }
        confFile.close();
    }
    return confs;
}