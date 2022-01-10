#ifndef READFILE_H
#define READFILE_H

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <limits.h>
#include <ranges>

#include "utilities.hpp"

namespace readFile{

    auto getStream(const std::string& filename){
        std::fstream file(filename);
        std::stringstream stream{};
        if(!file.is_open()){
            std::cout << "Could not open " + filename + ".\n";
        }
        else{
            stream << file.rdbuf();
        }
        return stream;
    }


    auto string = [](const std::string& filename){
        std::string s{getStream(filename).str()};
        return s;
    };

    auto vectorOfVectorOfInts(const std::string& filename, char del1 = '\n', char del2 = ' '){
        const auto text{getStream(filename).str()};

        const auto rows = Utilities::split(text, del1);
        std::vector<std::vector<int>> v{};
        std::ranges::for_each(
            rows,
            [&v, del2](auto& row){
                const auto split = Utilities::split(row, del2);
                std::vector<int> elements{};
                std::ranges::transform(split, std::back_inserter(elements), [](const auto& s){ return std::stoi(s);} );
                v.push_back( std::move(elements) );
            }
        );
        return v;
    }

    auto vectorOfStrings(const std::string& filename, char del = '\n', const bool keepEmpty = false){
        auto wholeFile{getStream(filename).str()};
        return Utilities::split(wholeFile, del, keepEmpty);
    }


    auto vectorOfInts(const std::string& filename, char del = '\n'){
        auto wholeFile{getStream(filename).str()};

        std::vector<int> v{};

        auto left = std::begin(wholeFile);
        const auto right = std::end(wholeFile);
        while(left < right){
            auto delimiterIt = std::find(left, right, del);
            std::string temp{left, delimiterIt};
            v.push_back(std::stoi(temp));
            left = delimiterIt+1;
        }
        return v;
    }
}

#endif
