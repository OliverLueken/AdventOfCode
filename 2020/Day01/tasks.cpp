
#include "../../lib/readFile.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>

auto getProductOfNNumbersSummingTo(const auto& numbers, const auto n, const auto sumToReach){
    if(sumToReach<=0) return 0;
    if(n==1){
        return sumToReach*bool(Utilities::contains(numbers, sumToReach));
    }

    for (const auto& number : numbers) {
        const auto product = getProductOfNNumbersSummingTo(numbers, n-1, sumToReach-number);
        if(product > 0){
            return number*product;
        }
    }
    return 0;
}

int main() {
    const auto numbers = readFile::vectorOfInts("input.txt");

    //Task 1
    const auto productOfTwo   = getProductOfNNumbersSummingTo(numbers, 2, 2020);
    std::cout << "The product of two numbers summing to 2020 is "   << productOfTwo   << ".\n";

    //Task 2
    const auto productOfThree = getProductOfNNumbersSummingTo(numbers, 3, 2020);
    std::cout << "The product of three numbers summing to 2020 is " << productOfThree << ".\n";
}
