
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

struct isValidPassphrase{
    std::vector<bool(*)(const std::vector<std::string>&)> lambdas{};

    template<typename... Lambdas>
    isValidPassphrase(Lambdas&&... lambdas_) : lambdas{std::forward<Lambdas>(lambdas_)...}{}

    bool isValid(const std::string& passphrase){
        const auto words = Utilities::split(passphrase);
        return std::ranges::all_of(lambdas, [&words](auto& lambda){
            return lambda(words);}
        );
    }
};

auto wordsAreUnique = [](const auto& words){
    const auto wordCount = words.size();
    auto uniqueWords = std::unordered_set<std::string>{};
    std::ranges::move(words, std::inserter(uniqueWords, uniqueWords.begin()));
    return wordCount == uniqueWords.size();
};


auto other = [](const auto& words){
    return true;
};

auto getResult = [](const auto& passphrases){
    isValidPassphrase everyWordIsUnique{wordsAreUnique};
    return std::ranges::count_if(passphrases, [&](const auto& passphrase){
        return everyWordIsUnique.isValid(passphrase);
    });
};

auto getResult2 = [](const auto& passphrases){

    isValidPassphrase everyWordIsUnique{wordsAreUnique, other};
    return std::ranges::count_if(passphrases, [&](const auto& passphrase){
        return everyWordIsUnique.isValid(passphrase);
    });
};

int main(){
    const auto passphrases = readFile::vectorOfStrings();

    //Task 1
    const auto result = getResult(passphrases);
    std::cout << "Task 1: " << result << ".\n";

    //Task 2
    const auto result2 = getResult2(passphrases);
    std::cout << "Task 2: " << result2 << ".\n";

    // VerifySolution::verifySolution(result, result2);
}