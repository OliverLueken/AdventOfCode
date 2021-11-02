#ifndef READINPUT_H
#define READINPUT_H

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

namespace readInput{

    auto string = [](std::string filename){
        std::fstream file(filename);
        std::stringstream stream{};
        std::string s{};
        if(!file.is_open()){
            std::cout << "Could not open " + filename + ".\n";
        }
        else{
            stream << file.rdbuf();
            s = stream.str();
        }
        return s;
    };

}

#endif
