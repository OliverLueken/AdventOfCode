
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

unsigned long result1 = 0, result2 = 0;

void calcEncriptionKey(long k1, long k2, int l1, int l2){
    std::cout << k1 << ": " << l1 << std::endl << k2 << ": " << l2 << std::endl;
    // if(l2 < l1){
    //   l1=l2;
    // }else{
    //   k1=k2;
    // }
    long s = k1;
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
    long key1 = stol(input[0]), key2 = stol(input[1]);

    int l1 = getLoopSize(key1);
    int l2 = getLoopSize(key2);

    calcEncriptionKey(key1, key2, l1, l2);
}

strvec readfile(std::string file){
    std::string line;
    std::ifstream input(file);
    strvec lines;

    if(input.is_open()){
        while(getline(input, line)){
            lines.push_back(line);
        }
        input.close();
    }
    else{
        std::cout << "Unable to open file\n";
    }
    return lines;
}

int main(){
    strvec input = readfile("input.txt");

    doStuff(input);

    std::cout << result1 << "\n";

    VerifySolution::verifySolution(result1);
}
