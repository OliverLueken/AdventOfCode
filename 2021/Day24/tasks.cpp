
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <deque>

struct Parameter{
    int x{0};
    int y{0};
    bool divZ{false};
};

auto parseInput = [](const auto& input){
    std::vector<Parameter> monadParameter{};
    Parameter parameter{};
    for(const auto& line : input | std::views::drop(1)){
        if(line == "inp w"){
            monadParameter.emplace_back(std::move(parameter));
            parameter = Parameter{};
            continue;
        }
        const auto subLine = line.substr(6);
        if(line.starts_with("add x ") && (std::isdigit(subLine[0]) || subLine[0]=='-')){
            parameter.x = std::stoi(subLine);
        }
        else if(line.starts_with("add y ") && (std::isdigit(subLine[0]) || subLine[0]=='-') && subLine[0]!='2'){
            parameter.y = std::stoi(subLine);
        }
        else if(line == "div z 26"){
            parameter.divZ = true;
        }
    }
    monadParameter.emplace_back(std::move(parameter));
    return monadParameter;
};

struct validDigitPairs{
    int firstDigitPosition{0};
    int secondDigitPosition{0};
    std::vector<std::pair<int,int>> digitPairs{};
};

auto addValidDigitPairs(const int iDigitPosition, const int jDigitPosition, const int x, const int y, auto& validModelNumberDigits){
    validDigitPairs validDigitPairs{jDigitPosition, iDigitPosition};
    for(auto jDigit=1; jDigit<10; jDigit++){
        const auto iDigit = jDigit + y + x;
        if(0<iDigit && iDigit<10){
            auto p = std::make_pair( jDigit, iDigit);
            validDigitPairs.digitPairs.emplace_back( p );
        }
    }
    validModelNumberDigits.emplace_back(std::move(validDigitPairs));
}

auto getValidModelNumberDigits(const auto& monadParameter){
    std::deque<std::pair<int, int>> stack{};
    std::vector<validDigitPairs> validModelNumberDigits{};
    int iDigitPosition=0;
    for(const auto parameter : monadParameter){
        if(parameter.x>9){
            stack.push_back(std::make_pair(iDigitPosition, parameter.y));
        }
        else{
            const auto jDigitPosition = stack.back().first;
            const auto y = stack.back().second;
            stack.pop_back();
            addValidDigitPairs(iDigitPosition, jDigitPosition, parameter.x, y, validModelNumberDigits);
        }
        iDigitPosition++;
    }
    return validModelNumberDigits;
}

template<class Cmp = std::less<> >
auto getSmallestValidModelNumber(const auto& validModelNumberDigits, const Cmp cmp = std::less<>()){
    std::string modelNumber(14, 'A');
    for(const auto& validDigitPairs : validModelNumberDigits){
        const auto minDigitPair = std::ranges::min_element(validDigitPairs.digitPairs, cmp, [](const auto& pair){return pair.first;} );
        modelNumber[validDigitPairs.firstDigitPosition ] = static_cast<char>( minDigitPair->first  + '0' );
        modelNumber[validDigitPairs.secondDigitPosition] = static_cast<char>( minDigitPair->second + '0' );
    }
    return modelNumber;
}

int main(){
    const auto monadParameter = parseInput(readFile::vectorOfStrings("input.txt"));
    const auto validModelNumberDigits  = getValidModelNumberDigits(monadParameter);

    //Task 1
    const auto largestValidModelNumber = getSmallestValidModelNumber(validModelNumberDigits, std::greater<>{});
    std::cout << "The largest model number accepted by MONAD is " << largestValidModelNumber << ".\n";

    //Task 2
    const auto smallestValidModelNumber = getSmallestValidModelNumber(validModelNumberDigits);
    std::cout << "The smallest model number accepted by MONAD is " << smallestValidModelNumber << ".\n";

    VerifySolution::verifySolution(largestValidModelNumber, smallestValidModelNumber);
}