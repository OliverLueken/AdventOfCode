
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

typedef std::set<std::string> strset;
typedef std::vector<strset> strsetvec;
typedef std::vector<std::string> strvec;
typedef std::pair<int, int> point;

unsigned long result1 = 0;

void calcEncriptionKey(long k1, int l2){
    long n = 1;
    for (int i = 0; i < l2; i++){
        n *= k1;
        n %= 20201227;
    }
    result1 = n;
}

int getLoopSize(long key){
    long n = 1;
    int loopsize = 0;
    while(n != key){
        n *= 7;
        n %= 20201227;
        loopsize++;
    }
    return loopsize;
}

void doStuff(strvec& input){
    long key1 = stol(input[0]);
    long key2 = stol(input[1]);

    int l2 = getLoopSize(key2);

    calcEncriptionKey(key1, l2);
}

int main(){
    auto input = readFile::vectorOfStrings();

    doStuff(input);

    std::cout << result1 << "\n";

    VerifySolution::verifySolution(result1);
}
