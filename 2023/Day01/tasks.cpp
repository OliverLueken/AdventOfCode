
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <array>
#include <algorithm>
#include <numeric>
#include <ranges>
#include <optional>

auto isDigit = [](const char c){return std::isdigit(c);};
auto getFirstDigitIt = [](const auto& s){
    return std::ranges::find_if(s, isDigit);
};
auto convertItToInt = [](const auto it){return *it - '0';};

auto getResult = [](const auto& parsedInput){
    auto getCalibrationValue = [](const auto& line){
        int firstDigit = getFirstDigitIt(line) | convertItToInt;
        int lastDigit = getFirstDigitIt(line | std::ranges::views::reverse) | convertItToInt;
        return 10*firstDigit+lastDigit;
    };
    return Utilities::sum(parsedInput, 0, getCalibrationValue);
};

// auto getResult2 = [](const auto& parsedInput){
//     auto containsStringNumber = [](const auto begin, const auto end) -> int{
//         auto numbers = std::unordered_map<int, std::string>{
//             {1,"one"},
//             {2, "two"},
//             {3, "three"},
//             {4, "four"},
//             {5, "five"},
//             {6, "six"},
//             {7, "seven"},
//             {8, "eight"},
//             {9, "nine"},
//         };
//         auto firstIt = end;
//         std::optional<int> digit = std::nullopt;
//         for(const auto& [number, string] : numbers){
//             auto it = std::ranges::search(begin, end, string.begin(), string.end()).begin();
//             if(it < end){
//                 firstIt = it;
//                 digit = number;
//             }
//         }
//         if(digit){
//             return digit.value();
//         }
//         return 0;
//
//     };
//
//     auto getTrueCalibrationValue = [&](const auto& line){
//         auto firstDigit = getFirstDigit(line);
//         auto firstDigit = containsStringNumber(line.begin(), line.end());
//         Utilities::print(firstDigit, '\n');
//     };
//     for(const auto& line : parsedInput)
//     {
//         getTrueCalibrationValue(line);
//     }
//     return 0;
// };

int main(){
    const auto parsedInput = readFile::vectorOfStrings();

    //Task 1
    const auto result = getResult(parsedInput);
    std::cout << "Task 1: " << result << ".\n";

    //Task 2
    //const auto result2 = getResult2(parsedInput);
    //std::cout << "Task 2: " << result2 << ".\n";

    // VerifySolution::verifySolution(result, result2);
}
