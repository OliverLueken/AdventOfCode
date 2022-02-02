
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <ranges>
#include <unordered_map>

using ValidFieldRange = std::vector<std::pair<int,int>>;
using Ticket = std::vector<int>;

auto getRules = [](const auto& input, const auto numberOfRuleFields){
    auto rules = std::unordered_map<std::string, ValidFieldRange>{};

    for(const auto& row : input | std::views::take(numberOfRuleFields)){
        const auto split = Utilities::split(row, ':');
        auto& fieldName  = split[0];
        const auto tempFieldRange = Utilities::splitOnEach(split[1], " -");
        ValidFieldRange validFieldRange{};
        for(auto i=0u; i<tempFieldRange.size(); i+=3){ //3 instead of 2 to skip the 'or'
            validFieldRange.emplace_back( std::stoi(tempFieldRange[i]), std::stoi(tempFieldRange[i+1]) );
        }
        rules.emplace(std::move(fieldName), std::move(validFieldRange));
    }
    return rules;
};

auto getTickets = [](const auto& input, const auto numberOfRuleFields){
    auto tickets = std::vector<Ticket>{};
    auto addTicket = [&tickets](const auto& row){
        auto ticketValues = std::vector<int>{};
        std::ranges::transform(
            Utilities::split(row, ','),
            std::back_inserter(ticketValues),
            [](const auto& value){return std::stoi(value);}
        );
        tickets.emplace_back(std::move(ticketValues));
    };

    addTicket(input[numberOfRuleFields+1]);
    for(const auto& row : input | std::views::drop(numberOfRuleFields+3)){
        addTicket(row);
    }
    return tickets;
};

auto extractInfo(const auto& input){
    const auto numberOfRuleFields = 20;
    auto rules   = getRules  (input, numberOfRuleFields);
    auto tickets = getTickets(input, numberOfRuleFields);

    return std::make_pair(rules, tickets);
}

auto isValueSatisfyingRule = [](const int value, const auto& rule){
    const auto& [key, validRange] = rule;
    return std::ranges::any_of(validRange, [value](const auto& pair){
        return Utilities::isBetween(value, pair.first, pair.second+1);
    });
};


auto getSumOfInvalidFieldsAndFilterInvalidTickets = [](const auto& rules, auto& tickets) {
    auto isInvalidValue = [&rules](const int value) {
        return std::ranges::none_of(rules, [&value](const auto& rule){
            return isValueSatisfyingRule(value, rule);
        });
    };
    auto sumOfInvalidFields = 0;
    auto isInValidTicket = [&sumOfInvalidFields, &isInvalidValue](const auto& ticket) {
        bool isInvalid = false;
        for (const auto& value : ticket) {
            if(isInvalidValue(value)) {
                isInvalid = true;
                sumOfInvalidFields += value;
            }
        }
        return isInvalid;
    };

    std::erase_if(tickets, isInValidTicket);
    return sumOfInvalidFields;
};




auto mapRulesToPossibleFields(const auto& rules, const auto& tickets) {
    auto getPossibleFieldsForRule = [](const auto& rule, const auto& tickets_){
        auto possibleFieldsForRule = std::vector<unsigned int>{};
        for(const auto field : std::views::iota(0u, tickets_[0].size()) ){
            const auto allValuesPossible = std::ranges::all_of(tickets_, [field, &rule](const auto& ticket){
                return isValueSatisfyingRule(ticket[field], rule);
            });
            if(allValuesPossible) possibleFieldsForRule.push_back(field);
        }
        return possibleFieldsForRule;
    };

    auto ruleToPossibleFieldsMap = std::unordered_map<std::string, std::vector<unsigned int>>{};
    for (const auto& rule : rules) {
        ruleToPossibleFieldsMap[rule.first] = getPossibleFieldsForRule(rule, tickets);
    }
    return ruleToPossibleFieldsMap;
}


auto mapKeysToField = [](const auto& rules, const auto& tickets) {
    auto ruleToPossibleFieldsMap = mapRulesToPossibleFields(rules, tickets);
    auto keyToFieldMap = std::unordered_map<std::string, int>{};

    auto removeFieldPossibilityFromEveryRule = [](const auto field, auto& ruleToPossibleFieldsMap_){
        std::ranges::for_each( ruleToPossibleFieldsMap_ | std::views::values, [field](auto& vec){
            std::erase(vec, field);
        });
    };
    auto ruleHasUniqueField = [&removeFieldPossibilityFromEveryRule, &keyToFieldMap, &ruleToPossibleFieldsMap]
        (const auto& ruleToPossibleField){
        const auto [key, vec] = ruleToPossibleField;
        if (vec.size() == 1) {
            const auto field = vec[0];
            keyToFieldMap[key] = field;
            removeFieldPossibilityFromEveryRule(field, ruleToPossibleFieldsMap);
            return true;
        }
        return false;
    };

    const auto numberOfKeysToMap = ruleToPossibleFieldsMap.size();
    auto numberOfDeletedElements = 1lu;
    while(keyToFieldMap.size() < numberOfKeysToMap && numberOfDeletedElements > 0){
        numberOfDeletedElements = std::erase_if( ruleToPossibleFieldsMap, ruleHasUniqueField );
    }
    return keyToFieldMap;
};

auto getProductOfDepartureFields = [](const auto& rules, const auto& tickets) {

    const auto keyToFieldMap = mapKeysToField(rules, tickets);

    return std::transform_reduce(
        std::begin(keyToFieldMap),
        std::end(keyToFieldMap),
        1l,
        std::multiplies<>(),
        [&tickets](const auto& map){
            if(map.first.starts_with("departure")){
                return tickets[0][map.second];
            }
            return 1;
        }
    );
};

int main() {
    auto [rules, tickets] = extractInfo(readFile::vectorOfStrings());

    // Task 1
    const auto sumOfInvalidFields = getSumOfInvalidFieldsAndFilterInvalidTickets(rules, tickets);
    std::cout << "The sum of the invalid fields is " << sumOfInvalidFields << ".\n";

    // Task 2
    const auto productOfDepartureFields = getProductOfDepartureFields(rules, tickets);
    std::cout << "The product of the departure fields is " << productOfDepartureFields << ".\n";

    VerifySolution::verifySolution(sumOfInvalidFields, productOfDepartureFields);
}
