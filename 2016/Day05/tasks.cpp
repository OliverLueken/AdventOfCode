
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"
#include "../../lib/md5.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>


auto getNextValidHash = [i=1u](const auto& input) mutable {
    std::string msg{};
    while(true){
        msg = input + std::to_string(i++);
        const auto hash = MD5::getMD5Hash(std::move(msg));
        if( hash.starts_with("00000") ){
            std::cout << i << ' ' << hash << '\n';
            return hash;
        }
    }
};

auto getFirstPassword = [](const auto& input){
    std::string pw{};
    std::vector<std::string> validHashs{};
    while(pw.size() < 8){
        validHashs.emplace_back(getNextValidHash(input));
        pw+=validHashs.back()[5];
    }
    return std::make_pair(pw, std::move(validHashs));
};

auto getSecondPassword = [](const auto& input, auto& validHashs){
    auto addCharacterToPassword = [foundPasswordCharacters=0u](const auto& nextHash, auto& pw) mutable {
        const auto pos = nextHash[5]-'0';
        if(pos>=0 && pos<8 && pw[pos]=='_'){
            pw[pos] = nextHash[6];
            foundPasswordCharacters++;
            std::cout << pw << '\n';
        }
        return foundPasswordCharacters == pw.size();
    };

    std::string pw{"________"};
    bool passwordComplete = false;
    for(const auto& nextHash : validHashs){
        passwordComplete = addCharacterToPassword(nextHash, pw);
    }

    while( !passwordComplete ){
        const auto nextHash = getNextValidHash(input);
        passwordComplete = addCharacterToPassword(nextHash, pw);
    }
    return pw;
};

int main(){
    const auto input = "uqwqemis";

    //Task 1
    auto [firstPW, validHashs] = getFirstPassword(input);
    std::cout << "The first password is " << firstPW << ".\n";

    //Task 2
    const auto secondPW = getSecondPassword(input, validHashs);
    std::cout << "The second password is " << secondPW << ".\n";

    VerifySolution::verifySolution(firstPW, secondPW);
}