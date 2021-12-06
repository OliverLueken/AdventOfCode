
#include "../../lib/readFile.hpp"
#include "../../lib/md5.hpp"

#include <iostream>
#include <string>

// clear && g++ tasks.cpp -std=c++20 -Wpedantic -Wall -Wextra -Wconversion -L/usr/lib/cryptopp/ -lcryptopp

auto getLowestNumber(const auto& input, const auto& s, unsigned int num = 1u){
    while(true){
        const auto msg = input + std::to_string(num);
        const auto hash = MD5::getMD5Hash(msg);

        if(hash.starts_with(s)){
            return num;
        }
        num++;
    }
}

int main() {

    const std::string input = "yzbqklnj";

    //Task 1
    const auto numberProducing5Zeros = getLowestNumber(input, "00000");
    std::cout << numberProducing5Zeros << " is the lowest positive number producing 5 leading zeros.\n";

    //Task 2
    const auto numberProducing6Zeros = getLowestNumber(input, "000000", numberProducing5Zeros);
    std::cout << numberProducing6Zeros << " is the lowest positive number producing 6 leading zeros.\n";
 }

