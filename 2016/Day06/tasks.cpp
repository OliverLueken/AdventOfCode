
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

auto getCorrectLetter = [](const auto& messages, const auto i, const auto& cmp){
    std::unordered_map<char, unsigned int> characterQuantity{};
    for(const auto& message : messages){
        characterQuantity[message[i]]++;
    }

    return std::ranges::max_element(characterQuantity,
                [&cmp](const auto& lhs, const auto& rhs){return cmp(lhs.second,rhs.second);}
           )->first;
};

auto getCorrectedMessage = []<typename comparator = std::less<>>
                           (const auto& messages, const comparator& cmp = std::less<>()){
    std::string message{};
    for(auto i=0u; i<messages[0].size(); i++){
        message+=getCorrectLetter(messages, i, cmp);
    }
    return message;
};

int main(){
    const auto messages = readFile::vectorOfStrings("input.txt");

    //Task 1
    const auto correctedMessage = getCorrectedMessage(messages);
    std::cout << "The corrected message says: " << correctedMessage << ".\n";

    //Task 2
    const auto originalMessage = getCorrectedMessage(messages, std::greater_equal<>());
    std::cout << "The original message says: " << originalMessage << ".\n";

    VerifySolution::verifySolution(correctedMessage, originalMessage);
}