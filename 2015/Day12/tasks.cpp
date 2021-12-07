
#include "../../lib/readFile.hpp"

#include <iostream>
#include <string>
#include <regex>

auto sumNumbers = [](const auto& string) -> int {
    static const std::regex number(R"(-?\d+)");
    auto sum = 0;
    for(auto foundIt = std::sregex_iterator(std::begin(string), std::end(string), number); foundIt!=std::sregex_iterator(); foundIt++){
        sum+=std::stoi(foundIt->str());
    }
    return sum;
};

auto sumWithRedIgnored = [](std::string& string) -> int {
    const std::regex curlyWithRed (R"(\{[^\{\}\[\]]*red[^\{\}\[\]]*\})");
    const std::regex curlyOrSquare(R"((?:\{|\[)[^\{\}\[\]]*(?:\}|\]))");

    while(true){
        std::smatch matchResult;
        while(std::regex_search(string, matchResult, curlyWithRed)){
            string.erase(matchResult[0].first, matchResult[0].second);
        }

        const auto foundMatch = std::regex_search(string, matchResult, curlyOrSquare);
        if(!foundMatch) break;
        const auto sum = std::to_string(sumNumbers(matchResult.str()));
        string = string.replace(matchResult[0].first, matchResult[0].second, sum);
    }

    return std::stoi(string);
};

int main(){
    auto input = readFile::string("input.txt");

    const auto sum = sumNumbers(input);
    std::cout << "The sum of all numbers is " << sum << ".\n";

    const auto sumWithoutRed = sumWithRedIgnored(input);
    std::cout << "The sum of all numbers without red is " << sumWithoutRed << ".\n";

}