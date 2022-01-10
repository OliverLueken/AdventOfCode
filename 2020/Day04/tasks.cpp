
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using Passport = std::unordered_map<std::string, std::string>;
auto parseInput(const auto& input){
    std::vector<Passport> passports{};
    Passport passport{};
    for(const auto& row : input){
        if(row.empty()){
            passports.emplace_back(std::move(passport));
            passport.clear();
        }
        else{
            auto split = Utilities::splitOnEach(row, " :");
            for(auto i=0u; i<split.size(); i+=2){
                passport.emplace(std::move(split[i]), std::move(split[i+1]));
            }
        }
    }
    passports.emplace_back(std::move(passport));
    return passports;
}

auto getValidCount1(const auto& passports){
    const std::vector<std::string> requiredFields = {"byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"};
    auto hasEveryRequiredField = [&requiredFields](const auto& passport){
        return std::ranges::all_of(requiredFields, [&passport](const auto& field){return passport.contains(field);});
    };
    return std::ranges::count_if( passports, hasEveryRequiredField );
}
auto getValidCount2(const auto& passports){
    const std::vector<std::string> requiredFields = {"byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"};
    auto hasEveryRequiredField = [&requiredFields](const auto& passport){
        return std::ranges::all_of(requiredFields, [&passport](const auto& field){return passport.contains(field);});
    };
    auto satisfiesFieldConditions = [](const auto& passport){
        auto satisfies_byr_Condition = [](const auto& _passport){
            const auto& val = _passport.at("byr");
            return Utilities::isNumber(val) && Utilities::isBetween(std::stoi(val), 1920, 2003);
        };
        auto satisfies_iyr_Condition = [](const auto& _passport){
            const auto& val = _passport.at("iyr");
            return Utilities::isNumber(val) && Utilities::isBetween(std::stoi(val), 2010, 2021);
        };
        auto satisfies_eyr_Condition = [](const auto& _passport){
            const auto& val = _passport.at("eyr");
            return Utilities::isNumber(val) && Utilities::isBetween(std::stoi(val), 2020, 2031);
        };
        auto satisfies_hgt_Condition = [](const auto& _passport){
            const auto& val = _passport.at("hgt");
            const auto unit = val.substr(val.size()-2);
            if(unit == "cm") return Utilities::isBetween(std::stoi(val), 150, 194);
            if(unit == "in") return Utilities::isBetween(std::stoi(val), 59,  77);
            return false;
        };
        auto satisfies_hcl_Condition = [](const auto& _passport){
            const auto& val = _passport.at("hcl");
            return val[0]=='#' && val.size()==7 && Utilities::isHexNumber(val.substr(1));
        };
        auto satisfies_ecl_Condition = [validEyeColors = std::array<std::string, 7>{"amb", "blu", "brn", "gry", "grn", "hzl", "oth"}](const auto& _passport){
            const auto& val = _passport.at("ecl");
            return Utilities::contains(validEyeColors, val);
        };
        auto satisfies_pid_Condition = [](const auto& _passport){
            const auto& val = _passport.at("pid");
            return Utilities::isNumber(val) && val.size()==9;
        };

        return satisfies_byr_Condition(passport)
            && satisfies_iyr_Condition(passport)
            && satisfies_eyr_Condition(passport)
            && satisfies_hgt_Condition(passport)
            && satisfies_hcl_Condition(passport)
            && satisfies_ecl_Condition(passport)
            && satisfies_pid_Condition(passport);
    };
    auto hasEveryRequiredFieldAndSatisfiesFieldConditions = [&hasEveryRequiredField, &satisfiesFieldConditions](const auto& passport){
        bool valid = hasEveryRequiredField(passport) && satisfiesFieldConditions(passport);
        return valid;
    };
    return std::ranges::count_if( passports, hasEveryRequiredFieldAndSatisfiesFieldConditions );
}

int main(){
    const auto passports = parseInput(readFile::vectorOfStrings("input.txt", '\n', true));

    //Task 1
    const auto validCount1 = getValidCount1(passports);
    std::cout << "Valid passports " << validCount1 << ".\n";

    //Task 2
    const auto validCount2 = getValidCount2(passports);
    std::cout << "Valid passports " << validCount2 << ".\n";

    VerifySolution::verifySolution(validCount1, validCount2);
}