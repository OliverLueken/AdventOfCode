
#include "../../lib/readFile.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <iomanip>

using auntDetails=std::unordered_map<std::string, unsigned int>;

auto removeLastChar = [](const auto& s){
    return s.substr(0, s.size()-1);
};

auto parseAuntdata = [](const auto& input){
    std::unordered_map<int, auntDetails> auntData;
    for(const auto& s : input){
        const auto split = Utilities::split(s);
        auntDetails temp{};
        for(auto pos=2u; pos < split.size()-1; pos+=2){
            temp[removeLastChar(split[pos])] = std::stoi(split[pos+1]);
        }
        auntData[std::stoi(split[1])] = temp;
    }
    return auntData;
};

auto parseSpecialAuntData = [](const auto& input){
    auntDetails details;
    for(const auto s : input){
        const auto split = Utilities::split(s);
        details[removeLastChar(split[0])] = std::stoi(split[1]);
    }
    return details;
};

auto isTaskOneSpecial = [](const auto& aunt, const auto& specialAunt){
    for(const auto& [key, value] : aunt){
        if(value != specialAunt.at(key)){
            return false;
        }
    }
    return true;
};

auto isTaskTwoSpecial = [](const auto& aunt, const auto& specialAunt){
    for(const auto& [key, value] : aunt){
        if(key == "cats" || key == "trees"){
            if(value <= specialAunt.at(key)){
                return false;
            }
        }
        else if(key =="pomeranians" || key == "goldfish"){
            if(value >= specialAunt.at(key)){
                return false;
            }
        }
        else{
            if(value != specialAunt.at(key)){
                return false;
            }
        }
    }
    return true;
};

auto findSpecialAunt = [](const auto& auntData, const auto& specialAunt, const auto& isSpecial){
    for(const auto& [auntNumber, aunt] : auntData ){
        if(isSpecial(aunt, specialAunt)){
            return auntNumber;
        }
    }
    return 0;
};


int main(){
    const auto input1 = readFile::vectorOfStrings("input1.txt");
    const auto input2 = readFile::vectorOfStrings("input2.txt");

    auto auntData = parseAuntdata(input1);
    const auto specialAunt = parseSpecialAuntData(input2);

    //Task 1
    auto aunt = findSpecialAunt(auntData, specialAunt, isTaskOneSpecial);
    std::cout << "The aunt Sue that got you the gift is aunt #" << aunt << ".\n";

    //Task 2
    aunt = findSpecialAunt(auntData, specialAunt, isTaskTwoSpecial);
    std::cout << "The aunt Sue that got you the gift is aunt #" << aunt << ".\n";
}