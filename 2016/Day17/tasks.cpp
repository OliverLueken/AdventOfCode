
#include "../../lib/readFile.hpp"
#include "../../lib/utilities.hpp"
#include "../../lib/md5.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using Position = Utilities::Position<int>;

struct Path{
    std::string passcode{};
    Position pos{};

    Path(std::string passcode) : passcode{std::move(passcode)}{}
    Path(std::string passcode, Position pos) : passcode{std::move(passcode)}, pos(std::move(pos)){}
};

auto advance = [](const auto& path, auto& nextPaths){
    const auto hash = MD5::getMD5Hash(path.passcode);

    if(Utilities::contains("BCDEF", hash[0])){ //Up
        if(path.pos.second > 0){
            auto newPos{path.pos};
            newPos.second-=1;
            nextPaths.emplace_back(path.passcode+'U', newPos);
        }
    }
    if(Utilities::contains("BCDEF", hash[1])){ //down
        if(path.pos.second < 3){
            auto newPos{path.pos};
            newPos.second+=1;
            nextPaths.emplace_back(path.passcode+'D', newPos);
        }

    }
    if(Utilities::contains("BCDEF", hash[2])){ //left
        if(path.pos.first > 0){
            auto newPos{path.pos};
            newPos.first-=1;
            nextPaths.emplace_back(path.passcode+'L', newPos);
        }

    }
    if(Utilities::contains("BCDEF", hash[3])){ //right
        if(path.pos.first < 3){
            auto newPos{path.pos};
            newPos.first+=1;
            nextPaths.emplace_back(path.passcode+'R', newPos);
        }
    }
};

auto getShortestPath = [](const auto& input){
    std::vector<Path> paths{Path{input}};
    while(!paths.empty()){
        std::vector<Path> nextPaths{};
        for(const auto& path : paths){
            advance(path, nextPaths);
        }
        for(const auto& path : nextPaths){
            if(path.pos.first == 3 && path.pos.second == 3){
                return path.passcode.substr(input.size());
            }
        }
        std::swap(nextPaths, paths);
    }
    return std::string("not found");
};

auto getLongestPathLength = [](const auto& input){
    auto longestPathLength = 0lu;

    auto reachedEnd = [&longestPathLength, &input](const auto& path){
        if(path.pos.first == 3 && path.pos.second == 3){
            longestPathLength = path.passcode.size()-input.size();
            return true;
        }
        return false;
    };

    std::vector<Path> paths{Path{input}};
    while(!paths.empty()){
        std::vector<Path> nextPaths{};
        for(const auto& path : paths){
            advance(path, nextPaths);
        }
        std::erase_if(nextPaths, reachedEnd);
        std::swap(nextPaths, paths);
    }
    return longestPathLength;
};


int main(){
    const std::string input = "awrkjxxr";

    //Task 1
    const auto shortestPath = getShortestPath(input);
    std::cout << shortestPath << " is the shortest path to the vault.\n";

    //Task 2
    const auto longestPathLength = getLongestPathLength(input);
    std::cout << "The longest path to the vault has length " << longestPathLength << ".\n";
}