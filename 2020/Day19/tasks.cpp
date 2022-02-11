
#include "../../lib/readFile.hpp"
#include "../../lib/utilities.hpp"
#include "../../lib/verifySolution.hpp"

#include <algorithm>
#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <unordered_map>
#include <cassert>


template<bool withTaskTwo>
std::string convertRule(const auto& inputRules, auto& convertedRules, const auto n){
    if (convertedRules.contains(n)) return convertedRules[n];

    auto brokenDownRule = std::vector<std::vector<int>>{};
    std::ranges::transform( Utilities::split( inputRules.at(n), '|'), std::back_inserter(brokenDownRule), [](const auto& ruleSequence){
        auto ruleNumbers = std::vector<int>{};
        std::ranges::transform(Utilities::split(ruleSequence, ' '), std::back_inserter(ruleNumbers), [](const auto& stringNumber){
            return std::stoi(stringNumber);
        });
        return ruleNumbers;
    });


    auto convertSubRule = [&inputRules, &convertedRules](const auto& ruleSequence) -> std::string {
        auto convertedSubRule = std::string{'('};
        convertedSubRule.reserve(50);
        std::ranges::for_each(ruleSequence, [&convertedSubRule, &inputRules, &convertedRules](const auto ruleNumber){
            convertedSubRule += convertRule<withTaskTwo>(inputRules, convertedRules, ruleNumber);
        });
        convertedSubRule += ')';
        return convertedSubRule;
    };

    auto convertedRule = std::string{'('};
    convertedRule.reserve(50);
    std::ranges::for_each(
        brokenDownRule | std::views::take(brokenDownRule.size()-1),
        [&convertedRule, &convertSubRule](const auto& subRule){
            convertedRule += convertSubRule(subRule) + '|';
        }
    );
    convertedRule += convertSubRule(brokenDownRule.back()) + ')';

    // for Task 2 (not a good solution, but works)
    if constexpr(withTaskTwo){
        if(n==8) convertedRule+="+";
        if(n==11){
            const auto rule42 = convertRule<withTaskTwo>(inputRules, convertedRules, 42);
            const auto rule31 = convertRule<withTaskTwo>(inputRules, convertedRules, 31);
            auto t = rule42 + rule31;
            convertedRule="((" + t + ')';
            for(auto i=0; i<5; i++){
                t = rule42 + t + rule31;
                convertedRule+= "|(" + t + ')';
            }
            convertedRule += ')';
        }
    }
    convertedRules[n] = convertedRule;
    return convertedRule;
}


auto makeConvertedRules = [](const auto& inputRules){
    auto convertedRules = std::unordered_map<int, std::string>{};
    for(const auto& [id, rule] : inputRules){
        const auto it = std::ranges::find(rule, '"');
        if(it!=rule.end()){
            convertedRules[id] = std::string{*(it+1)};
        }
    }
    return convertedRules;
};

template<bool withTaskTwo = false>
auto convertRulesToRegex(const auto& inputRules) {
    auto convertedRules = makeConvertedRules(inputRules);
    const auto rule = convertRule<withTaskTwo>(inputRules, convertedRules, 0);
    return std::regex(rule);
}

template<bool isTaskTwo = false>
auto countValidMessages(const auto& messages, const auto& inputRules) {
    const auto rules = convertRulesToRegex<isTaskTwo>(inputRules);
    return std::ranges::count_if(messages, [&rules](const auto& message){
        return std::regex_match(message, rules);
    });
}

auto parseInput(auto&& input) {
    const auto messagesStartIt = std::ranges::find(input, "")+1;

    auto messages = std::vector<std::string>{};
    messages.reserve(std::distance(messagesStartIt, std::end(input)));
    std::ranges::move(messagesStartIt, std::end(input), std::back_inserter(messages));

    auto inputRules = std::unordered_map<int, std::string> {};
    std::ranges::transform(std::begin(input), messagesStartIt-1, std::inserter(inputRules, std::begin(inputRules)), [](const auto& rule){
        const auto split = Utilities::split(rule, ':');
        return std::make_pair( stoi(split[0]), split[1].substr(1) );
    });

    assert(inputRules.contains(0));
    return std::make_pair( std::move(messages), std::move(inputRules) );
}

int main(){
    const auto [messages, inputRules] = parseInput(readFile::vectorOfStrings("input.txt", '\n', true));

    //Task 1
    const auto validTaskOneMessages = countValidMessages<false>(messages, inputRules);
    std::cout << "There are " << validTaskOneMessages << " messages matching rule 0.\n";

    //Task 2
    const auto validTaskTwoMessages = countValidMessages<true>(messages, inputRules);
    std::cout << "After updating rules 8 and 11, there are " << validTaskTwoMessages << " messages matching rule 0.\n";

    VerifySolution::verifySolution(validTaskOneMessages, validTaskTwoMessages);
}
