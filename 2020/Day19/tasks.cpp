
#include "../../lib/readFile.hpp"
#include "../../lib/utilities.hpp"
#include "../../lib/verifySolution.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <unordered_map>
#include <cassert>

template<bool>
std::string convertRules(const auto&, auto&, const std::string);

template<bool withTaskTwo>
std::string innerConversion(const auto& rules, auto& donerules, const auto n){
    if (donerules.contains(n)) return donerules[n];

    auto rule = convertRules<withTaskTwo>(rules, donerules, rules.at(n));

    // //part 2 (not a good solution, but works)
    if constexpr(withTaskTwo){
        if(n==8) rule+="+";
        if(n==11){
            const auto rule42 = innerConversion<withTaskTwo>(rules, donerules, 42);
            const auto rule31 = innerConversion<withTaskTwo>(rules, donerules, 31);
            auto t = rule42 + rule31;
            rule="((" + t + ")";
            for(auto i=0; i<5; i++){
                t = rule42 + t + rule31;
                rule+= "|(" + t + ")";
            }
            rule += ')';
        }
    }
    donerules[n] = rule;
    return rule;
}

template<bool withTaskTwo>
std::string convertRules(const auto& rules, auto& donerules, const std::string currrule) {
    auto rule = std::string{};
    auto brokenDownRule = std::vector<std::vector<std::string>>{};
    std::ranges::transform( Utilities::split( currrule, '|'), std::back_inserter(brokenDownRule), [](const auto& ruleSequence){
        return Utilities::split(ruleSequence, ' ');
    });
    rule+='(';
    for(const auto& ruleSequence : brokenDownRule){
        rule+='(';
        for(const auto& ruleNumber : ruleSequence){
            rule+=innerConversion<withTaskTwo>(rules, donerules, std::stoi(ruleNumber));
        }
        rule+=')';
        if(ruleSequence != brokenDownRule.back()){
            rule+='|';
        }
    }
    rule+=')';
    return rule;
}

template<bool withTaskTwo = false>
auto convertRulesToRegex(auto& rules) {
    auto donerules = std::unordered_map<int, std::string>{};
    donerules[64] = "a"; //hardcoding for now
    donerules[50] = "b"; //make dynamic later
    const std::string rule = convertRules<withTaskTwo>(rules, donerules, rules[0]);
    return std::regex(rule);
}

template<bool isTaskTwo = false>
auto countValidMessages(const auto& messages, auto& inputRules) {
    const auto rules = convertRulesToRegex<isTaskTwo>(inputRules);
    return std::ranges::count_if(messages, [&rules](const auto& message){
        return std::regex_match(message, rules);
    });
}

auto parseInput(auto&& input) {
    const auto it = std::ranges::find(input, "")+1;

    auto messages = std::vector<std::string>{};
    messages.reserve(std::distance(it, std::end(input)));
    std::ranges::move(it, std::end(input), std::back_inserter(messages));

    auto inputRules = std::unordered_map<int, std::string> {};
    std::ranges::transform(std::begin(input), it-1, std::inserter(inputRules, std::begin(inputRules)), [](const auto& rule){
        const auto split = Utilities::split(rule, ':');
        return std::make_pair( stoi(split[0]), split[1].substr(1) );
    });

    assert(inputRules.contains(0));
    return std::make_pair( std::move(messages), std::move(inputRules) );
}

int main(){
    auto [messages, inputRules] = parseInput(readFile::vectorOfStrings("input.txt", '\n', true));

    //Task 1
    const auto validTaskOneMessages = countValidMessages<false>(messages, inputRules);
    std::cout << "There are " << validTaskOneMessages << " messages matching rule 0.\n";

    //Task 2
    const auto validTaskTwoMessages = countValidMessages<true>(messages, inputRules);
    std::cout << "After updating rules 8 and 11, there are " << validTaskTwoMessages << " messages matching rule 0.\n";

    VerifySolution::verifySolution(validTaskOneMessages, validTaskTwoMessages);
}
