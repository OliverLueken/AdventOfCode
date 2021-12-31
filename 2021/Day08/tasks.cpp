
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


auto getNumberOfObviousDigitsInOutputPatterns = [](const auto& input){
    auto getOutputPatterns = [](const auto& patterns){
        return patterns | std::views::drop(10);
    };
    auto patternIsObviousDigit = [](const auto& pattern){
        return pattern.size() == 2 || pattern.size() == 3 || pattern.size() == 4 || pattern.size() == 7;
    };
    return std::ranges::count_if(input | std::views::transform(getOutputPatterns) | std::views::join, patternIsObviousDigit);
};


auto getDecoder(const auto& signalPatterns){
    std::unordered_map<char, size_t> decoder{};
    for(const auto& pattern : signalPatterns){
        for(const auto& signal : pattern){
            decoder[signal]+=2*pattern.size()%7;
        }
    }
    return decoder;
}

auto decodePattern(const auto& s, const auto& decoder){
    return Utilities::sum(s, 0u,
        [&decoder](const auto c){
            return decoder.at(c);
        }
    );
}

class DecodedPatternValueToDigit{
    auto initialize() const {
        std::unordered_map<int, int> decodedPatternValueToDigit{};
        const std::vector<std::string> digitPatterns ={
            "abcefg",   //0
            "cf",       //1
            "acdeg",    //2
            "acdfg",    //3
            "bcdf",     //4
            "abdfg",    //5
            "abdefg",   //6
            "acf",      //7
            "abcdefg",  //8
            "abcdfg"    //9
        };

        const auto decoder = getDecoder(digitPatterns);

        for(auto digit=0u; digit<digitPatterns.size(); digit++){
            const auto decodedPatternValue = decodePattern(digitPatterns[digit], decoder);
            decodedPatternValueToDigit[decodedPatternValue] = digit;
        }
        return decodedPatternValueToDigit;
    }

public:
    const std::unordered_map<int, int> decodedPatternValueToDigit{};

    DecodedPatternValueToDigit() : decodedPatternValueToDigit{initialize()}{}
};

auto& getDecodedPatternValueToDigit(){
    static const DecodedPatternValueToDigit wrapper{};
    return wrapper.decodedPatternValueToDigit;
}



auto decodeOutput = [](const auto& patterns){
    const auto decodedPatternValueToDigit = getDecodedPatternValueToDigit();
    const auto signalPatterns = patterns | std::views::take(10);
    const auto outputPatterns = patterns | std::views::drop(10);
    const auto decoder = getDecoder(signalPatterns);

    auto output = 0;
    for(const auto& pattern : outputPatterns){
        const auto decodedPatternValue = decodePattern(pattern, decoder);
        output*=10;
        output+=decodedPatternValueToDigit.at(decodedPatternValue);
    }
    return output;
};


int main(){
    const auto input = parseInput(readFile::vectorOfStrings("input.txt"));

    //Task 1
    const auto numberOfObviousDigitsInOutputPatterns = getNumberOfObviousDigitsInOutputPatterns(input);
    std::cout << "A total of " << numberOfObviousDigitsInOutputPatterns << " output values would display 1, 4, 7, or 8.\n";

    //Task 2
    const auto sumOfOutputValues = Utilities::sum(input, 0, decodeOutput);
    std::cout << "The sum of all output values is " << sumOfOutputValues << ".\n";
}