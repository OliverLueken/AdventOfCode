
#include "../../lib/readFile.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <regex>

auto getNextMarker(const auto first, const auto last){
    static const std::regex marker(R"(\(\d+x\d+\))");
    std::smatch match;
    std::regex_search(first, last, match, marker);
    return match;
}

auto getDecompressedLength = [](const auto& string){
    auto first = std::begin(string);
    auto last  = std::end(string);
    auto match = getNextMarker(first, last);
    auto count = 0l;
    while(!match.empty()){
        count+=std::distance(first, match[0].first);
        first = match[0].first;
        last  = match[0].second;
        auto split = Utilities::split(first+1, last-1, 'x');
        const auto length = std::stoi(split[0]);
        const auto repeat = std::stoi(split[1]);
        count+=length*repeat;
        first=last+length;
        match = getNextMarker(first, std::end(string));
    }
    return count + std::distance(first, std::end(string));
};


auto getDecompressedLengthV2(auto first, const auto end) -> long int{
    auto last = end;
    auto match = getNextMarker(first, end);
    auto count = 0l;
    while(!match.empty()){
        count+=std::distance(first, match[0].first);
        first = match[0].first;
        last  = match[0].second;
        auto split = Utilities::split(first+1, last-1, 'x');
        const auto length = std::stoi(split[0]);
        const auto repeat = std::stoi(split[1]);
        first=last+length;
        count+=repeat*getDecompressedLengthV2(last, first);
        match = getNextMarker(first, end);
    }
    return count + std::distance(first, end);
}

int main(){
    const auto string = readFile::string("input.txt");

    //Task 1
    const auto decompressedLength = getDecompressedLength(string);
    std::cout << "The decompressed file has length " << decompressedLength << ".\n";

    //Task 2
    const auto decompressedLengthV2 = getDecompressedLengthV2(std::begin(string), std::end(string));
    std::cout << "With decompression 2.0, the file has length " << decompressedLengthV2 << ".\n";
}