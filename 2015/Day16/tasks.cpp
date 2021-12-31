
#include "../../lib/readFile.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <iomanip>

using auntDetails = std::unordered_map<std::string, unsigned int>;


auto parseAuntdata = [](const auto& input){
    std::unordered_map<int, auntDetails> auntData{};
    for(const auto& s : input){
        const auto split = Utilities::splitOnEach(s, " :");
        auntDetails thisAuntDetails{};
        for(auto pos=2u; pos < split.size()-1; pos+=2){
            const auto object = split[pos];
            const auto amount = std::stoi(split[pos+1]);
            thisAuntDetails[object] = amount;
        }
        const auto auntID = std::stoi(split[1]);
        auntData[auntID] = thisAuntDetails;
    }
    return auntData;
};

auto parseSpecialAuntData = [](const auto& input){
    auntDetails specialAuntDetails{};
    for(const auto& s : input){
        const auto split = Utilities::splitOnEach(s, " :");
        const auto object = split[0];
        const auto amount = std::stoi(split[1]);
        specialAuntDetails[object] = amount;
    }
    return specialAuntDetails;
};

auto isTaskOneSpecial = [](const auto& auntObjects, const auto& specialAuntObjects){
    auto sameAmountAsSpecialAuntObject = [&specialAuntObjects](const auto& auntObject){
        const auto& [object, amount] = auntObject;
        return amount == specialAuntObjects.at(object);
    };

    return std::ranges::all_of(auntObjects, sameAmountAsSpecialAuntObject);
};

auto isTaskTwoSpecial = [](const auto& auntObjects, const auto& specialAuntObjects){
    auto sameAmountAsSpecialAuntObject = [&specialAuntObjects](const auto& auntObject){
        const auto& [object, amount] = auntObject;
        if(object == "cats" || object == "trees"){
            return amount > specialAuntObjects.at(object);
        }
        else if(object =="pomeranians" || object == "goldfish"){
            return amount < specialAuntObjects.at(object);
        }
        return amount == specialAuntObjects.at(object);
    };

    return std::ranges::all_of(auntObjects, sameAmountAsSpecialAuntObject);
};

auto findSpecialAunt = [](const auto& allAuntsObjectData, const auto& specialAuntObjects, const auto& isSpecial){
    auto isSpecialAunt = [&specialAuntObjects, &isSpecial](const auto& auntData){
        const auto& auntObjects = auntData.second;
        return isSpecial(auntObjects, specialAuntObjects);
    };

    const auto specialAuntIt = std::ranges::find_if(allAuntsObjectData, isSpecialAunt);
    return specialAuntIt->first;
};


int main(){
    const auto allAuntsObjectData = parseAuntdata(readFile::vectorOfStrings("input1.txt"));
    const auto specialAuntData = parseSpecialAuntData(readFile::vectorOfStrings("input2.txt"));

    //Task 1
    const auto actuallyNotSpecialAuntID = findSpecialAunt(allAuntsObjectData, specialAuntData, isTaskOneSpecial);
    std::cout << "The aunt Sue that got you the gift is aunt #" << actuallyNotSpecialAuntID << ".\n";

    //Task 2
    const auto specialAuntID = findSpecialAunt(allAuntsObjectData, specialAuntData, isTaskTwoSpecial);
    std::cout << "The aunt Sue that got you the gift is aunt #" << specialAuntID << ".\n";
}