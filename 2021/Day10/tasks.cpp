
#include "../../lib/readFile.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <ranges>
#include <regex>


auto separateCodes = [](const auto& input){
    std::vector<std::string> corruptedCodes{};
    std::vector<std::string> incompleteCodes{};
    for(auto string : input){
        const std::regex brackets(R"((?:\(\))|(?:\[\])|(?:\{\})|(?:<>))");
        std::smatch matchResult;
        while(std::regex_search(string, matchResult, brackets)){
            string.erase(matchResult[0].first, matchResult[0].second);
        }

        auto it = std::ranges::find_first_of(string, ")]}>");
        if(it!=std::end(string)){
            corruptedCodes.emplace_back(std::move(string));
        }
        else{
            incompleteCodes.emplace_back(std::move(string));
        }
    }
    return std::make_pair(std::move(corruptedCodes), std::move(incompleteCodes));
};

auto getSyntaxErrorScore = [](const auto& corruptedCodes){
    auto getPoints = [](const auto& corruptedCode){
        const auto it = std::ranges::find_first_of(corruptedCode, ")]}>");
        switch(*it){
            break; case ')': return 3;
            break; case ']': return 57;
            break; case '}': return 1197;
            break; case '>': return 25137;
        }
        return 0;
    };
    return Utilities::sum(corruptedCodes, 0u, getPoints);
};

auto getCompletionScore = [](const auto& incompleteCodes){
    auto getScore = [](const auto& code){
        auto score=0ul;
        for(const auto c : code | std::views::reverse){
            score*=5;
            switch(c){
                break; case '(': score+=1;
                break; case '[': score+=2;
                break; case '{': score+=3;
                break; case '<': score+=4;
            }
        }
        return score;
    };

    std::vector<unsigned long> scores{};
    std::ranges::transform(incompleteCodes, std::back_inserter(scores), getScore);
    const auto middle = std::begin(scores)+(scores.size()-1)/2;
    std::ranges::nth_element(scores, middle);
    return *middle;
};

int main(){
    const auto input = readFile::vectorOfStrings("input.txt");
    const auto [corruptedCodes, incompleteCodes] = separateCodes(input);

    //Task 1
    const auto syntaxErrorScore = getSyntaxErrorScore(corruptedCodes);
    std::cout << "The total syntax error score is " << syntaxErrorScore << ".\n";

    //Task 2
    const auto completionScore = getCompletionScore(incompleteCodes);
    std::cout << "The autocomplete tool got a score of " << completionScore << ".\n";
}