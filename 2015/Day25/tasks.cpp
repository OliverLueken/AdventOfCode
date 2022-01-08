
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>

int main(){
    const auto strings = Utilities::split(readFile::string("input.txt"));

    const auto row = std::stoul(strings[15]);
    const auto col = std::stoul(strings[17]);
    const auto n   = col+(row+col-2)*(row+col-1)/2;

    auto code = 20'151'125ul;
    for(auto i=1ul; i<n; i++){
        code*=252'533ul;
        code%=33'554'393ul;
    }

    std::cout << "The machine is requesting the code " << code << ".\n";

    VerifySolution::verifySolution(code);
}