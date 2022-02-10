
#include "../../lib/readFile.hpp"
#include "../../lib/utilities.hpp"
#include "../../lib/verifySolution.hpp"

#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <cmath>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

typedef std::vector<std::string> strvec;

unsigned long result1 = 0, result2 = 0;

std::string convertRules(const strvec& rules,
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
        rule = "(" + convertRules(rules, donerules, str1) + "|" +
               convertRules(rules, donerules, str2) + ")";
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
                rule += convertRules(rules, donerules, str);
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
            rule = convertRules(rules, donerules, rules[n]);
            // //part 2 (not a good solution, but works)
            // if(n==8) rule+="+";
            // if(n==11){
            //   std::string rule42 = convertRules(rules, donerules, "42");
            //   std::string rule31 = convertRules(rules, donerules, "31");
            //   std::vector<std::string> t;
            //   rule=rule42 + rule31;
            //   t.push_back(rule);
            //   for(int i=0; i<5; i++){
            //      rule = rule42 + t.back() + rule31;
            //      t.push_back(rule);
            //   }
            //   rule=t[0];
            //   for(int i=1; i<t.size(); i++){
            //     rule=rule + "|" +"(" + t[i] + ")";
            //   }
            //   rule = "("+rule+")";
            //   std::cout << rule << std::endl << std::endl;
            // }
            donerules[n] = rule;
        }
    }
    // std::cout << rule << std::endl;
    return rule;
}

std::regex convertRulesToRegex(strvec& rules) {
    std::map<int, std::string> donerules;
    std::string rule = convertRules(rules, donerules, rules[0]);
    std::cout << rule << std::endl;
    return std::regex(rule);
}

auto countValidMessages(const strvec& messages, const std::regex& rule) {
    return std::ranges::count_if(messages, [&rule](const auto& message){
        return std::regex_match(message, rule);
    });
}

auto extractRuleFromMessages(strvec& messages) {
    std::regex rule;
    strvec rules;
    for (auto it = messages.begin(); it != messages.end();) {
        // std::cout << *it << std::endl;
        if (it->empty()) {
            messages.erase(it);
            break;
        }

        auto pos = stoul(*it);
        std::string rulestr = it->substr(it->find(":") + 2);
        if (pos >= rules.size()) rules.resize(pos + 1);
        rules[pos] = rulestr;

        it = messages.erase(it);
    }

    return convertRulesToRegex(rules);
}

int main(){
    auto input = readFile::vectorOfStrings("input.txt", '\n', true);
    const auto rule = extractRuleFromMessages(input);

    //Task 1
    const auto sumOfSolutionsWithLeftToRight = countValidMessages(input, rule);
    std::cout << "The sum of the solutions evaluated left to right is " << sumOfSolutionsWithLeftToRight << ".\n";

    //Task 2
    const auto sumOfSolutionsWithAdditionBeforeMultiplication = countValidMessages(input, rule);
    std::cout << "The sum of the solutions evaluated with the new rules is " << sumOfSolutionsWithAdditionBeforeMultiplication << ".\n";

    VerifySolution::verifySolution(sumOfSolutionsWithLeftToRight, sumOfSolutionsWithAdditionBeforeMultiplication);
}
