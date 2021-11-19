
#include "../../readFile.h"
#include "../../utilities.h"

#include <iostream>
#include <string>

int main(){
    const auto strings = Utilities::split(readFile::string("input.txt"));

    const auto row = std::stoul(strings[15]);
    const auto col = std::stoul(strings[17]);
    const auto n = col+(row+col-2)*(row+col-1)/2;

    unsigned long long int code = 20151125;
    for(auto i=1u; i<n; i++){
        code*=252533u;
        code%=33554393u;
    }

    std::cout << "The machine is requesting the code " << code << ".\n";
}