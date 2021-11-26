
#include "../../lib/readFile.hpp"
#include "../../lib/utilities.hpp"
#include "../../lib/md5.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// clear && g++ tasks.cpp -std=c++20 -Wpedantic -Wall -Wextra -Wconversion -L/usr/lib/cryptopp/ -lcryptopp

auto hasNLeadingZeros(const auto& s, const unsigned int n){
    bool hasNLeadingZeros = true;
    for(auto i=0u; i<n; i++){
        hasNLeadingZeros &= s[i]=='0';
    }
    return hasNLeadingZeros;
}

class getNextValidHash_{
    unsigned int i{0u};

public:
    auto operator()(const auto& input) {
        std::string msg{};
        while(true){
            msg = input + std::to_string(i++);
            const auto hash = MD5::getMD5Hash(std::move(msg));
            if(hasNLeadingZeros(hash, 5u)){
                std::cout << i << ' ' << hash << '\n';
                return hash;
            }
        }
    }

    void reset(){
        i=0u;
    }
};
inline getNextValidHash_ getNextValidHash{};

auto getFirstPassword = [](const auto& input){
    std::string pw{};
    while(pw.size() < 8){
        pw+=getNextValidHash(input)[5];
    }
    return pw;
};

auto getSecondPassword = [](const auto& input){

    getNextValidHash.reset();

    std::unordered_map<int, char> characterPositions;
    while(characterPositions.size() < 8){
        auto nextHash = getNextValidHash(input);
        auto pos = nextHash[5];
        if(std::isdigit(pos) && pos-'0' < 8 && !characterPositions.contains(pos-'0')){
            characterPositions[pos-'0'] = nextHash[6];
        }
    }
    std::string pw{"________"};
    for(const auto& [pos, c] : characterPositions){
        pw[pos] = c;
    }
    return pw;
};

int main(){
    const auto input = "uqwqemis";

    //Task 1
    const auto firstPW = getFirstPassword(input);
    std::cout << "The first password is " << firstPW << ".\n";

    //Task 2
    const auto secondPW = getSecondPassword(input);
    std::cout << "The second password is " << secondPW << ".\n";
}