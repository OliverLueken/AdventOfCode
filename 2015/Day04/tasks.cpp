#include "../../readFile.h"

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include "/usr/lib/cryptopp/md5.h"
#include "/usr/lib/cryptopp/hex.h"
#include "/usr/lib/cryptopp/files.h"

#include <functional>
#include <iostream>
#include <string>
#include <sstream>

// clear && g++ tasks.cpp -std=c++20 -Wpedantic -Wall -Wextra -Wconversion -L/usr/lib/cryptopp/ -lcryptopp

auto getMD5Hash(std::string message){
    using namespace CryptoPP;

    static Weak::MD5 hash;
    static std::string digest;

    std::stringstream stream;
    hash.Update((const byte*) &message[0], message.size());
    digest.resize(hash.DigestSize());
    hash.TruncatedFinal((byte*)&digest[0], digest.size());

    static HexEncoder encoder(new FileSink(stream));
    StringSource(digest, true, new Redirector(encoder));
    return stream.str();
}

auto hasNLeadingZeros(const auto& s, unsigned int n){
    bool hasNLeadingZeros = true;
    for(auto i=0u; i<n; i++){
        hasNLeadingZeros &= s[i]=='0';
    }
    return hasNLeadingZeros;
}

auto getLowestNumber(const auto& input, unsigned int n, unsigned int start = 1u){
    for(unsigned int i=start; ; i++){
        auto msg = input + std::to_string(i);

        auto hash = getMD5Hash(msg);
        if(hasNLeadingZeros(hash, n)){
            return i;
        }
    }
}

int main() {

    const std::string input = "yzbqklnj";

    //Task 1
    auto number = getLowestNumber(input, 5u);
    std::cout << number << " is the lowest positive number producing 5 leading zeros.\n";

    //Task 2
    number = getLowestNumber(input, 6u, number);
    std::cout << number << " is the lowest positive number producing 6 leading zeros.\n";
 }
