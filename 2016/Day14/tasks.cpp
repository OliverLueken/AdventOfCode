
#include "../../lib/readFile.hpp"
#include "../../lib/utilities.hpp"
#include "../../lib/md5.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>


auto adjacent_find_n = [](const auto& r, const unsigned int n){
    auto first = std::begin(r);
    while(first != std::end(r)-n+1){
        const auto nSameAdjacent = std::ranges::all_of( first+1, first+n,
            [first](const auto& val){ return val == *first;}
        );
        if( nSameAdjacent ) break;
        first++;
    }
    return std::make_pair(first!=std::end(r)-n+1, first);
};

auto toLower = [](std::string& s){
    std::ranges::transform(s, std::begin(s),
        [](auto c){return (char)std::tolower(c);}
    );
};

auto hash2017Times(std::string s){
    for(int i=0; i<2017; i++){
        s=MD5::getMD5Hash(std::move(s));
        toLower(s);
    }
    return s;
}

auto lookForKey = [](const auto& input, auto& i, auto& letterToIDs, auto& keys, const auto hashFunc){
    const auto hash = hashFunc(input + std::to_string(i));

    const auto [hasThreeAdjacentLetters, it3] = adjacent_find_n(hash, 3);
    if(hasThreeAdjacentLetters){
        const auto character = *it3;
        letterToIDs[character].emplace_back(i);
    }

    const auto [hasFiveAdjacentLetters, it5] = adjacent_find_n(hash, 5);
    if(hasFiveAdjacentLetters){
        const auto character = *it5;
        std::erase_if(letterToIDs[character],
            [i, &keys](const auto id){
                if( id+1000 >= i && id < i ){
                    keys.push_back(id);
                    return true;
                }
                return false;
            }
        );
        return;
    }
};

auto getLastKeyID(const auto& input, const std::function<std::string(std::string)> hashFunc = MD5::getMD5Hash){
    std::unordered_map<char, std::vector<unsigned int>> letterToIDs;
    std::vector<unsigned int> keys;
    auto i = 1u;
    while(keys.size()<64){
        lookForKey(input, i, letterToIDs, keys, hashFunc);
        i++;
    }

    //need to search the next 1000 indeces i in case the 5er is higher than the last one
    //but it has keys with lower id than the maxID found so far
    const auto maxID = std::ranges::max(keys);
    while(i < maxID+1000){
        lookForKey(input, i, letterToIDs, keys, hashFunc);
        i++;
    }

    std::ranges::sort(keys);
    return keys[63];
}

int main(){
    const auto input = "ihaygndm";

    //Task 1
    const auto lastKeyID = getLastKeyID(input);
    std::cout << "The 64th key is produced by the index " << lastKeyID << ".\n";

    //Task 2
    const auto lastStretchedKeyID = getLastKeyID(input, hash2017Times);
    std::cout << "The 64th stretched key is produced by the index " << lastStretchedKeyID << ".\n";
}