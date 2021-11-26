
#include "../../lib/readFile.hpp"
#include "../../lib/md5.hpp"

#include <functional>
#include <iostream>
#include <string>
#include <sstream>

// clear && g++ tasks.cpp -std=c++20 -Wpedantic -Wall -Wextra -Wconversion -L/usr/lib/cryptopp/ -lcryptopp

auto getLowestNumber(const auto& input, const auto& s, unsigned int start = 1u){
    for(unsigned int i=start; ; i++){
        auto msg = input + std::to_string(i);

        auto hash = MD5::getMD5Hash(msg);
        if(hash.starts_with(s)){
            return i;
        }
    }
}

int main() {

    const std::string input = "yzbqklnj";

    //Task 1
    auto number = getLowestNumber(input, "00000");
    std::cout << number << " is the lowest positive number producing 5 leading zeros.\n";

    //Task 2
    number = getLowestNumber(input, "000000", number);
    std::cout << number << " is the lowest positive number producing 6 leading zeros.\n";
 }

