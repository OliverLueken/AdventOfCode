#ifndef READINPUT_H
#define READINPUT_H

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <limits.h>
#include <ranges>

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
        auto text{getStream(filename).str()};

        std::vector<std::vector<int>> v{};

        auto addToVec = [&v, &del2](auto left, const auto& right){
            std::vector<int> innerVec{};
            while(left < right){
                auto delimiterIt = std::find(left, right, del2);
                const std::string tempNum{left, delimiterIt};
                innerVec.push_back(std::stoi(tempNum));
                left = delimiterIt+1;
            }
            v.push_back(innerVec);
        };

        auto left = std::begin(text);
        const auto right = std::end(text);
        while(left < right){
            auto delimiterIt = std::find(left, right, del1);
            addToVec(left, delimiterIt);
            left = delimiterIt+1;
        }
        return v;
    }

    auto vectorOfStrings(const std::string& filename, char del = '\n'){
        auto wholeFile{getStream(filename).str()};

        std::vector<std::string> v{};

        auto left = std::begin(wholeFile);
        const auto right = std::end(wholeFile);
        while(left < right){
            auto delimiterIt = std::find(left, right, del);
            std::string temp{left, delimiterIt};
            v.push_back(temp);
            left = delimiterIt+1;
        }
        return v;
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
