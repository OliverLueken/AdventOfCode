
#include "../../lib/readFile.hpp"
#include "../../lib/utilities.hpp"
#include "../../lib/verifySolution.hpp"

#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <cmath>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

template<bool withTaskTwo>
std::string convertRules(const auto& rules,
                         std::map<int, std::string>& donerules,
                         std::string currrule) {
    // std::cout << currrule << std::endl;

    std::string rule;
    size_t a = currrule.find("|");
    if (a != std::string::npos) {
        std::string str1, str2;
        str1 = currrule.substr(0, a - 1);
        str2 = currrule.substr(a + 2);

        // std::cout << "\"" << str1 << "\"" << "|" << "\"" << str2 << "\"" <<
        // std::endl;
        rule = "(" + convertRules<withTaskTwo>(rules, donerules, str1) + "|" +
               convertRules<withTaskTwo>(rules, donerules, str2) + ")";
    } else {
        a = currrule.find(" ");
        if (a != std::string::npos) {
            std::vector<std::string> splitvec;

            boost::split(splitvec, currrule, boost::is_any_of(" "));
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
                  for(int i=1; i<t.size(); i++){
                    rule=rule + "|" +"(" + t[i] + ")";
                  }
                  rule = "("+rule+")";
                  std::cout << rule << std::endl << std::endl;
                }
            }
            donerules[n] = rule;
        }
    }
    // std::cout << rule << std::endl;
    return rule;
}

template<bool withTaskTwo = false>
std::regex convertRulesToRegex(auto& rules) {
    std::map<int, std::string> donerules;
    std::string rule = convertRules<withTaskTwo>(rules, donerules, rules[0]);
    std::cout << rule << std::endl;
    return std::regex(rule);
}

auto countValidMessages(const auto& messages, const std::regex& rule) {
    return std::ranges::count_if(messages, [&rule](const auto& message){
        return std::regex_match(message, rule);
    });
}

auto parseInput(auto&& input) {
    const auto it = std::ranges::find(input, "")+1;

    std::vector<std::string> messages{};
    messages.reserve(std::distance(it, std::end(input)));
    std::ranges::move(it, std::end(input), std::back_inserter(messages));

    std::unordered_map<int, std::string> inputRules{};
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
    const auto rules = convertRulesToRegex<false>(inputRules);
    const auto sumOfSolutionsWithLeftToRight = countValidMessages(messages, rules);
    std::cout << "The sum of the solutions evaluated left to right is " << sumOfSolutionsWithLeftToRight << ".\n";

    //Task 2
    auto rules2 = convertRulesToRegex<true>(inputRules);
    const auto sumOfSolutionsWithAdditionBeforeMultiplication = countValidMessages(messages, rules2);
    std::cout << "The sum of the solutions evaluated with the new rules is " << sumOfSolutionsWithAdditionBeforeMultiplication << ".\n";

    VerifySolution::verifySolution(sumOfSolutionsWithLeftToRight, sumOfSolutionsWithAdditionBeforeMultiplication);
}
