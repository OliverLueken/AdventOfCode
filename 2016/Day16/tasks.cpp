
#include "../../lib/readFile.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>


auto grow = [](std::string& s){
    s.push_back('0');
    std::transform(std::rbegin(s)+1, std::rend(s), std::back_inserter(s),
        [](const unsigned char& c)->unsigned char{ return c == '1' ? '0' : '1';}
    );
};

auto shrink = [](std::string& s){
    auto out = std::begin(s);
    for(auto it = std::begin(s); it<std::end(s); it+=2){
        *out = *it == *(it+1) ? '1' : '0';
        out++;
    }
    s.resize(s.size()/2);
};

auto getChecksum = [](std::string s, unsigned int size){
    const auto i = (unsigned int) std::ceil(std::log2(( (double)size+1 )/( (double)s.size()+1 ) ));
    const auto c = (unsigned int) (1<<i)*(s.size()+1)-1;
    s.reserve(c);

    while(s.size() < size){
        grow(s);
    }
    s.resize(size);

    while(s.size()%2==0){
        shrink(s);
    }
    return s;
};

int main(){
    std::string s = "00111101111101000";

    //Task 1
    const auto checksum1 = getChecksum(s, 272);
    std::cout << checksum1 << '\n';

    //Task 2
    const auto checksum2 = getChecksum(s, 35651584);
    std::cout << checksum2 << '\n';
}