
#include "../../lib/verifySolution.hpp"

#include <iostream>
#include <string>
#include <algorithm>

auto playLookAndSay = [](auto& number, const size_t n){
    auto lookAndSay = [](auto& num){
        std::string newNumber{};
        auto it1 = std::begin(num);
        auto it2 = it1;
        do{
            it2 = std::mismatch(it2, std::end(num), it2+1).second;
            newNumber += std::to_string(std::distance(it1, it2));
            newNumber += *it1;
            it1=it2;
        }while(it2 != std::end(num));
        std::swap(num, newNumber);
    };

    for(auto i = 0u; i < n; i++){
        lookAndSay(number);
    }
};

int main(){
    std::string number = "1321131112";

    //Task 1
    playLookAndSay(number, 40);
    const auto solution1 = number.size();
    std::cout << "The input after 40 iterations has length " << solution1 << ".\n";

    //Task 2
    playLookAndSay(number, 10);
    const auto solution2 = number.size();
    std::cout << "The input after 50 iterations has length " << solution2 << ".\n";

    VerifySolution::verifySolution(solution1, solution2);
}