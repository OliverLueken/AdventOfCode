
#include "../../lib/readFile.hpp"
#include "../../lib/utilities.hpp"

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include "/usr/lib/cryptopp/md5.h"
#include "/usr/lib/cryptopp/hex.h"
#include "/usr/lib/cryptopp/files.h"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// clear && g++ tasks.cpp -std=c++20 -Wpedantic -Wall -Wextra -Wconversion -L/usr/lib/cryptopp/ -lcryptopp

auto getMD5Hash(std::string message){
    using namespace CryptoPP;

    static Weak::MD5 hash;
    static std::string digest;

    std::stringstream stream;
    hash.Update((const byte*) &message[0], message.size());
    digest.resize(hash.DigestSize());
    hash.TruncatedFinal((byte*)&digest[0], digest.size());

    HexEncoder encoder(new FileSink(stream));
    StringSource(digest, true, new Redirector(encoder));
    return stream.str();
}

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
            const auto hash = getMD5Hash(std::move(msg));
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