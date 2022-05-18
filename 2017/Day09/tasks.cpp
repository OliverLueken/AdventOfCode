
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>

auto getScoreAndGarbageCount = [](const auto& characterStream){
    auto moveToClosingGarbage = [](auto& it, const auto end, auto& characterCount){
        ++it;
        while(it!=end){
            switch(*it){
            break; case '!':
                ++it;
            break; case '>':
                return;
            break; default:
                ++characterCount;
            }
            ++it;
        }
        return;
    };

    auto nestingLevel = 0;
    auto score = 0;
    auto garbageCharacterCount = 0;
    for(auto it = std::begin(characterStream); it!=std::end(characterStream); ++it){
        switch(*it){
        break; case '!':
            ++it;
        break; case '{':
            ++nestingLevel;
            score+=nestingLevel;
        break; case '<':
            moveToClosingGarbage(it, std::end(characterStream), garbageCharacterCount);
        break; case '}':
            --nestingLevel;
        }
    }
    return std::make_pair(score, garbageCharacterCount);
};

int main(){
    const auto characterStream = readFile::string();

    //Task 1
    const auto [score, garbageCharacterCount] = getScoreAndGarbageCount(characterStream);
    std::cout << "The score of the groups in the stream is " << score << ".\n";

    // //Task 2
    std::cout << "There are " << garbageCharacterCount << " non-canceled characters in the garbage.\n";

    VerifySolution::verifySolution(score, garbageCharacterCount);
}