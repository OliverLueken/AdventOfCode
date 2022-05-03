
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <numeric>
#include <ranges>

auto getResult = [](const auto& passphrases){
    auto isValidPassphrase = [](const auto& passphrase){
        auto words = Utilities::split(passphrase);
        const auto wordCount = words.size();
        auto uniqueWords = std::unordered_set<std::string>{};
        std::ranges::move(words, std::inserter(uniqueWords, uniqueWords.begin()));
        return wordCount == uniqueWords.size();
    };

    return std::ranges::count_if(passphrases, isValidPassphrase);
};

auto getResult2 = [](const auto& parsedInput){

    return 0;
};

int main(){
    const auto passphrases = readFile::vectorOfStrings();

    //Task 1
    const auto result = getResult(passphrases);
    std::cout << "Task 1: " << result << ".\n";

    // //Task 2
    // const auto result2 = getResult2(parsedInput);
    // std::cout << "Task 2: " << result2 << ".\n";

    // VerifySolution::verifySolution(result, result2);
}