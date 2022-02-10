
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

template<bool withTaskTwo>
std::string convertRules(const auto& rules, auto& donerules, std::string currrule) {
    // std::cout << currrule << std::endl;

    auto rule = std::string{};
    auto a = currrule.find("|");
    if (a != std::string::npos) {
        //std::string str1, str2;
        auto str1 = currrule.substr(0, a - 1);
        auto str2 = currrule.substr(a + 2);

        // std::cout << "\"" << str1 << "\"" << "|" << "\"" << str2 << "\"" <<
        // std::endl;
        rule = "(" + convertRules<withTaskTwo>(rules, donerules, str1) + "|" +
               convertRules<withTaskTwo>(rules, donerules, str2) + ")";
    } else {
        a = currrule.find(" ");
        if (a != std::string::npos) {
            auto splitvec = Utilities::split(currrule, ' ');
            
            // for(auto str:splitvec){
            //   std::cout << "\"" << str << "\"" << " ";
            // }
            // std::cout << std::endl;
            for (auto str : splitvec) {
                rule += convertRules<withTaskTwo>(rules, donerules, str);
            }
        } else {
            int n;
            try {
                n = stoi(currrule);
            } catch (...) {
                rule = "(" + currrule.substr(1, currrule.size() - 2) + ")";
                // std::cout << rule << std::endl;
                return rule;
            }

            auto it = donerules.find(n);
            if (it != donerules.end()) return it->second;
            rule = convertRules<withTaskTwo>(rules, donerules, rules.at(n));
            // //part 2 (not a good solution, but works)
            if constexpr(withTaskTwo){
                if(n==8) rule+="+";
                if(n==11){
                  std::string rule42 = convertRules<withTaskTwo>(rules, donerules, "42");
                  std::string rule31 = convertRules<withTaskTwo>(rules, donerules, "31");
                  std::vector<std::string> t;
                  rule=rule42 + rule31;
                  t.push_back(rule);
                  for(int i=0; i<5; i++){
                     rule = rule42 + t.back() + rule31;
                     t.push_back(rule);
                  }
                  rule=t[0];
                  for(auto i=1ul; i<t.size(); i++){
                    rule=rule + "|" +"(" + t[i] + ")";
                  }
                  rule = "("+rule+")";
                  // std::cout << rule << std::endl << std::endl;
                }
            }
            donerules[n] = rule;
        }
    }
    // std::cout << rule << std::endl;
    return rule;
}

template<bool withTaskTwo = false>
auto convertRulesToRegex(auto& rules) {
    auto donerules = std::unordered_map<int, std::string>{};
    const std::string rule = convertRules<withTaskTwo>(rules, donerules, rules[0]);
    // std::cout << rule << std::endl;
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
