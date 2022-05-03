
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <numeric>
#include <ranges>
#include <cassert>

struct isValidPassphrase{
    std::vector<bool(*)(const std::vector<std::string>&)> lambdas{};

    template<typename... Lambdas>
    isValidPassphrase(Lambdas&&... lambdas_) : lambdas{std::forward<Lambdas>(lambdas_)...}{}

    bool isValid(const std::string& passphrase){
        const auto words = Utilities::split(passphrase);
        return std::ranges::all_of(lambdas, [&words](auto& lambda){
            return lambda(words);
        });
    }
};

auto wordsAreUnique = [](const auto& words){
    const auto wordCount = words.size();
    auto uniqueWords = std::unordered_set<std::string>{};
    std::ranges::move(words, std::inserter(uniqueWords, uniqueWords.begin()));
    return wordCount == uniqueWords.size();
};

auto wordsAreNotAnagrams = [](const auto& words){
    auto wordLengthToHashes = std::unordered_map<size_t, std::unordered_set<size_t>>{};
    auto getHash = [](const auto& word){
        const auto wordLength = word.size();
        assert(wordLength <= 13);
        const auto hash = Utilities::sum(word, 0ul, [wordLength](const char c){
            const auto val = c-'a';
            return std::pow(val, wordLength); //this breaks with words above length 13
        });
        return hash;
    };
    auto isNotAnagram = [&](const auto& word){
        const auto hash = getHash(word);
        return wordLengthToHashes[word.size()].insert(hash).second;
    };
    return std::ranges::all_of(words, isNotAnagram);
};

template<typename... Conditions>
auto countValidPassphrases(const std::vector<std::string>& passphrases, Conditions&&... conditions){
    isValidPassphrase passphraseValidator{std::forward<Conditions>(conditions)...};
    return std::ranges::count_if(passphrases, [&](const auto& passphrase){
        return passphraseValidator.isValid(passphrase);
    });
};

int main(){
    const auto passphrases = readFile::vectorOfStrings();

    //Task 1
    const auto numberOfValidPassphrases = countValidPassphrases(passphrases, wordsAreUnique);
    std::cout << "There are " << numberOfValidPassphrases << " passphrases without duplicate words.\n";

    //Task 2
    const auto newNumberOfValidPassphrases = countValidPassphrases(passphrases, wordsAreUnique, wordsAreNotAnagrams);
    std::cout << "There are " << newNumberOfValidPassphrases << " passphrases without duplicate words or anagrams.\n";

    VerifySolution::verifySolution(numberOfValidPassphrases, newNumberOfValidPassphrases);
}