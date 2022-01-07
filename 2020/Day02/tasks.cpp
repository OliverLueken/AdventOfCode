
#include "../../lib/readFile.hpp"
#include "../../lib/utilities.hpp"

#include <algorithm>
#include <iostream>

struct PasswordPolicy{
    int  leftInteger{};
    int  rightInteger{};
    char letter{};
    std::string password{};
};

auto parseInput(const auto& input){
    std::vector<PasswordPolicy> passwords{};
    std::ranges::transform(input, std::back_inserter(passwords), [](const auto& row){
        const auto split = Utilities::splitOnEach(row, " -");
        return PasswordPolicy{std::stoi(split[0]), std::stoi(split[1]), split[2][0], split[3]};
    });
    return passwords;
}

auto getValidPasswordCount1(const auto& passwords){
    auto letterOccuranceInRange = [](const auto& password){
        const auto letterCount = std::ranges::count(password.password, password.letter);
        return password.leftInteger <= letterCount
                     && letterCount <= password.rightInteger;
    };
    return std::ranges::count_if(passwords, letterOccuranceInRange);
}

auto getValidPasswordCount2(const auto& passwords){
    auto letterInPos1AndNotInPos2 = [](const auto& password){
        return (password.password[password.leftInteger -1]==password.letter)
             ^ (password.password[password.rightInteger-1]==password.letter);
    };
    return std::ranges::count_if(passwords, letterInPos1AndNotInPos2);
}

int main() {
    const auto passwords = parseInput(readFile::vectorOfStrings("input.txt"));

    //Task 1
    const auto validPasswordCount1 = getValidPasswordCount1(passwords);
    std::cout << "With the first policy interpretation there are "  << validPasswordCount1 << " valid passwords.\n";

    //Task 2
    const auto validPasswordCount2 = getValidPasswordCount2(passwords);
    std::cout << "With the second policy interpretation there are " << validPasswordCount2 << " valid passwords.\n";
}
