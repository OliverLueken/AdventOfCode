
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <algorithm>

auto parseInput(const auto& input){
    std::vector<long> numbers{};
    std::ranges::transform(input, std::back_inserter(numbers), [](const auto& s){return std::stol(s);});
    return numbers;
}

auto doesDequeContain(const auto& dequeOfSums, const auto val){
    auto containsVal = [val](const auto& deque){
        return Utilities::contains(deque, val);
    };
    return std::ranges::any_of(dequeOfSums, containsVal);
}

auto getFirstNumberNotFulfillingTheRule = [](const auto& numbers){
    std::deque<std::deque<long>> dequeOfSums{};
    auto addNextDeque = [](auto& _dequeOfSums, const auto& _numbers, const auto i){
        std::deque<long> nextDeque{};
        for(auto j=std::max(25u, i)-25; j<i; j++){
            nextDeque.push_back(_numbers[j]+_numbers[i]);
        }
        _dequeOfSums.emplace_back(std::move(nextDeque));
    };

    for(auto i=1u; i<25u; i++){
        addNextDeque(dequeOfSums, numbers, i);
    }

    auto removeOldSums = [](auto& _dequeOfSums){
        _dequeOfSums.pop_front();
        for( auto& deque : _dequeOfSums ){
            deque.pop_front();
        }
    };
    for(auto i=25u; i<numbers.size(); i++){
        if( doesDequeContain(dequeOfSums, numbers[i]) == false ) return numbers[i];
        removeOldSums(dequeOfSums);
        addNextDeque(dequeOfSums, numbers, i);
    }
    return 0l;
};

auto getEncryptionWeakness = [](const auto& numbers, const auto val){
    auto sum     = 0l;
    auto leftIt  = std::begin(numbers);
    auto rightIt = std::begin(numbers);
    while(rightIt < std::end(numbers) || sum >= val ){
        if(sum < val){
            sum+=*rightIt;
            rightIt++;
        }
        else if(sum == val){
            const auto [minIt, maxIt] = std::ranges::minmax_element(leftIt, rightIt);
            return *minIt+*maxIt;
        }
        else if( val < sum ){
            sum-=*leftIt;
            leftIt++;
        }
    }
    return 0l;
};

int main(){
    const auto numbers = parseInput(readFile::vectorOfStrings());

    //Task 1
    const auto badNumber = getFirstNumberNotFulfillingTheRule(numbers);
    std::cout << "The first number not being a sum of two of the 25 previous numbers is " << badNumber << ".\n";

    //Task 2
    const auto encryptionWeakness = getEncryptionWeakness(numbers, badNumber);
    std::cout << "The encryption weakness is " << encryptionWeakness << ".\n";

    VerifySolution::verifySolution(badNumber, encryptionWeakness);
}