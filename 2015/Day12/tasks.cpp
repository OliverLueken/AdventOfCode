
#include "../../readFile.h"

#include <iostream>
#include <string>
#include <regex>

auto sumNumbers = [](const auto& string){
    std::regex number(R"(-?\d+)");
    int sum = 0;
    for(auto i = std::sregex_iterator(std::begin(string), std::end(string), number); i!=std::sregex_iterator(); i++){
        sum+=std::stoi(i->str());
    }
    return sum;
};

auto sumWithRedIgnored = [](std::string& string){
    std::regex curlyWithRed(R"(\{[^\{\}\[\]]*red[^\{\}\[\]]*\})");
    std::regex curlyOrSquare("(\\\{[^\\\{\\\}\\\[\\\]]*\\\})|(\\\[[^\\\{\\\}\\\[\\\]]*\\\])");

    while(true){
        std::smatch matchResult;
        while(std::regex_search(string, matchResult, curlyWithRed)){
            string = string.replace(matchResult[0].first, matchResult[0].second, "");
        }

        auto foundMatch = std::regex_search(string, matchResult, curlyOrSquare);
        if(!foundMatch) break;
        auto sum = std::to_string(sumNumbers(matchResult.str()));
        string = string.replace(matchResult[0].first, matchResult[0].second, sum);
    }

    return std::stoi(string);
};

int main(){
    auto input = readFile::string("input.txt");

    auto sum = sumNumbers(input);
    std::cout << "The sum of all numbers is " << sum << ".\n";

    sum = sumWithRedIgnored(input);
    std::cout << "The sum of all numbers without red is " << sum << ".\n";

}