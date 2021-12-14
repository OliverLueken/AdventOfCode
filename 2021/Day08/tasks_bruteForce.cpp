
#include "../../lib/readFile.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <numeric>
#include <ranges>


auto parseInput = [](const auto& input){
    std::vector<std::vector<std::string>> parsed;
    for(const auto& row : input){
        parsed.emplace_back(Utilities::splitOnEach(row, " |"));
    }
    return parsed;
};

class PatternToDigitMap{
public:
    std::unordered_map<std::string, int> patternToDigitMap{};

    PatternToDigitMap(){
        patternToDigitMap["abcefg"]  = 0;
        patternToDigitMap["cf"]      = 1;
        patternToDigitMap["acdeg"]   = 2;
        patternToDigitMap["acdfg"]   = 3;
        patternToDigitMap["bcdf"]    = 4;
        patternToDigitMap["abdfg"]   = 5;
        patternToDigitMap["abdefg"]  = 6;
        patternToDigitMap["acf"]     = 7;
        patternToDigitMap["abcdefg"] = 8;
        patternToDigitMap["abcdfg"]  = 9;
    }
};

auto& getPatternToDigitMap(){
    static const PatternToDigitMap wrapper{};
    return wrapper.patternToDigitMap;
}

auto getOutputPatterns = [](const auto& patterns){
    return patterns | std::views::reverse | std::views::take(4) | std::views::reverse;
};

auto getNumberOfObviousDigitsInOutputPatterns = [](const auto& input){
    auto patternIsObviousDigit = [](const auto& pattern){
        return pattern.size() == 2 || pattern.size() == 3 || pattern.size() == 4 || pattern.size() == 7;
    };
    return std::ranges::count_if(input | std::views::transform( getOutputPatterns ) | std::views::join, patternIsObviousDigit);
};


auto decode = [](const std::string& pattern, const auto& decoder){
    std::string decoded{};
    for( const auto signal : pattern){
        decoded+=decoder[signal-'a'];
    }
    std::ranges::sort(decoded);
    return decoded;
};


auto getDecoder = [](const auto& patterns){
    const auto patternToDigitMap = getPatternToDigitMap();
    std::string decoder = "abcdefg";

    auto notDecodableByDecoder = [&patternToDigitMap, &decoder](const auto& pattern){
        const auto decodedPattern = decode(pattern, decoder);
        return !patternToDigitMap.contains(decodedPattern);
    };

    while(std::ranges::any_of(patterns, notDecodableByDecoder)){
        std::ranges::next_permutation(decoder);
    }
    return decoder;
};

auto getOutput = [](const auto& patterns){
    const auto decoder = getDecoder(patterns);
    const auto patternToDigitMap = getPatternToDigitMap();

    auto value = 0;
    for(const auto& pattern : getOutputPatterns(patterns)){
        const auto decodedPattern = decode(pattern, decoder);
        value*=10;
        value+=patternToDigitMap.at(decodedPattern);
    }
    return value;
};


int main(){
    const auto input = parseInput(readFile::vectorOfStrings("input.txt"));

    //Task 1
    auto numberOfObviousDigitsInOutputPatterns = getNumberOfObviousDigitsInOutputPatterns(input);
    std::cout << "A total of " << numberOfObviousDigitsInOutputPatterns << " output values would display 1, 4, 7, or 8.\n";

    //Task 2
    const auto sumOfOutputValues = Utilities::sum(input, 0, getOutput);
    std::cout << "The sum of all output values is " << sumOfOutputValues << ".\n";
}