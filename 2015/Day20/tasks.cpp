
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>

auto getFactors = [](const auto n){
    std::vector<unsigned int> factors{};
    unsigned int i=1u;
    while(i*i < n){
        if(n%i==0){
            factors.push_back(i);
            factors.push_back(n/i);
        }
        i++;
    }
    if(i*i==n) factors.push_back(i);
    return factors;
};

auto getLowestHouseNumberWithAtLeast = [](auto minPresents){
    minPresents/=10;
    for(auto houseNumber=1u;;houseNumber++){

        auto factors = getFactors(houseNumber);

        auto presents = std::accumulate(std::begin(factors), std::end(factors), 0u);
        if(presents>minPresents) return houseNumber;
    }
};

auto getLowestHouseNumberWithAtLeast2 = [](auto minPresents){
    minPresents=(unsigned int) std::ceil(minPresents/11);
    for(auto houseNumber=1u;;houseNumber++){

        auto factors = getFactors(houseNumber);
        std::erase_if(factors, [houseNumber](const auto n){return houseNumber/n > 50;});

        auto presents = std::accumulate(std::begin(factors), std::end(factors), 0u);
        if(presents>minPresents) return houseNumber;
    }
};

int main(){
    const auto presents = 34000000u;

    const auto houseNumber = getLowestHouseNumberWithAtLeast(presents);
    std::cout << "The lowest house number that gets at least " << presents << " presents is " <<houseNumber <<".\n";

    const auto houseNumber2 = getLowestHouseNumberWithAtLeast2(presents);
    std::cout << "With the new rules the lowest house number that gets at least " << presents << " presents is now " <<houseNumber2 <<".\n";
}