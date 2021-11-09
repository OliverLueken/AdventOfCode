#ifndef HELPER_H
#define HELPER_H

#include <string>
#include <vector>

namespace Utilities{
    auto split = [](std::string s, char delim = ' '){
        std::vector<std::string> parts;

        size_t pos1=0, pos2=0;
        while(pos2 != s.npos){
            pos2 = s.find(delim, pos1);
            if(pos1 < pos2) parts.push_back(s.substr(pos1, pos2-pos1));
            pos1=pos2+1;
        }
        return parts;
    };
}

#endif