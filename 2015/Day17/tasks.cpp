
#include "../../lib/readFile.hpp"

#include <iostream>
#include <vector>
#include <algorithm>


std::function<void(const std::vector<int>&, const size_t, const size_t, const size_t, size_t&,
                   std::vector<size_t>&, size_t)> sumCombinationsRec //damn you recursion
    = [](const auto& containers, const auto currentSize, const auto goalSize, const auto level, auto& sum,
         auto& containerAmount, auto currentContainerAmount){

    if(currentSize == goalSize){
        sum++;
        containerAmount[currentContainerAmount]++; //task 2
        return;
    }
    if(currentSize > goalSize || level == containers.size() ) return;

    sumCombinationsRec(containers, currentSize, goalSize, level+1, sum, //branch in which container isn't used
                       containerAmount, currentContainerAmount);
    sumCombinationsRec(containers, currentSize+containers[level], goalSize, level+1, sum, //branch in wich container is used
                       containerAmount, currentContainerAmount+1);
};

auto getAmountOfCombinations = [](const auto& containers, const auto size){
    size_t sum = 0u, currentSize = 0u, level = 0u, currentContainerAmount=0u;
    std::vector<size_t> containerAmount(containers.size());
    sumCombinationsRec(containers, currentSize, size, level, sum,
                       containerAmount, currentContainerAmount); //information for task 2

    auto notZero = [](const auto& i){return i!=0;};

    return std::make_pair(sum,  //task 1
                    std::ranges::min(containerAmount | std::views::filter(notZero))); //task 2,
                                                      //zeros are those container amounts for which no combinations exist, so we need to omit those
};

int main(){
    const auto containers = readFile::vectorOfInts("input.txt");

    auto [combinations, minContainerAmount] = getAmountOfCombinations(containers, 150);
    std::cout << "There are a total of " << combinations << " combinations to fit the eggnog.\n";
    std::cout << "There are a total of " << minContainerAmount << " combinations with the minimal numbers of containers to fit the eggnog.\n";
}