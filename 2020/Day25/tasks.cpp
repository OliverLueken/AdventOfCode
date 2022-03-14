
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"

#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <cmath>
#include <fstream>
#include <iostream>
#include <queue>
#include <regex>
#include <set>
#include <string>
#include <vector>

auto calcEncriptionKey(long k1, int l2){
    long n = 1l;
    for (int i = 0; i < l2; i++){
        n *= k1;
        n %= 20201227;
    }
    return n;
}

int getLoopSize(long key){
    long n = 1l;
    int loopsize = 0;
    while(n != key){
        n *= 7;
        n %= 20201227;
        loopsize++;
    }
    return loopsize;
}

auto doStuff(const auto key1, const auto key2){
    const auto l2 = getLoopSize(key2);
    return calcEncriptionKey(key1, l2);
}

auto parseInput(auto&& input){
    return std::make_pair(
        std::stoul(input[0]),
        std::stoul(input[1])
    );
}

int main(){
    const auto [key1, key2] = parseInput(readFile::vectorOfStrings());

    //Task 1
    const auto encryptionKey = doStuff(key1, key2);
    std::cout << "The handshake is trying to establish the encryption key " << encryptionKey << ".\n";

    VerifySolution::verifySolution(encryptionKey);
}
