
#include "../../lib/readFile.hpp"
#include "../../lib/utilities.hpp"
#include "../../lib/md5.hpp"
#include "../../lib/matrix.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <unordered_map>
#include <unordered_set>
#include <deque>
#include <algorithm>
#include <numeric>
#include <climits>
#include <memory>
#include <ranges>


auto binToDec(const std::string& binary){
    unsigned long decimal = binary[0]-'0';
    for(const auto c : binary | std::views::drop(1)){
        decimal<<=1;
        decimal+=c-'0';
    }
    return decimal;
}

struct Node;
struct Node{
    unsigned int version{};
    unsigned int type{};
    unsigned long value{0};
    std::vector<Node> nodes{};

};


unsigned long evaluateNode(const auto type, const auto& nodes){
    const auto init = nodes[0].value;
    switch(type){
        break; case 0:
            return std::transform_reduce(std::begin(nodes)+1, std::end(nodes), init, std::plus<>(),
                [](const auto& node){return node.value;}
            );
        break; case 1:
            return std::transform_reduce(std::begin(nodes)+1, std::end(nodes), init, std::multiplies<>(),
                [](const auto& node){return node.value;}
            );
        break; case 2:
            return std::ranges::min(nodes, {}, &Node::value).value;
        break; case 3:
            return std::ranges::max(nodes, {}, &Node::value).value;
        break; case 5:
            return nodes[0].value >  nodes[1].value;//? 1 : 0;
        break; case 6:
            return nodes[0].value <  nodes[1].value;// ? 1 : 0;
        break; case 7:
            return nodes[0].value == nodes[1].value;// ? 1 : 0;
    }
    return 0;
}

auto parseNumberPackets(const auto& numberPackets){
    std::string binaryNumber{};
    auto pos = 1u;
    do {
        pos+=5;
        binaryNumber+=numberPackets[pos+1];
        binaryNumber+=numberPackets[pos+2];
        binaryNumber+=numberPackets[pos+3];
        binaryNumber+=numberPackets[pos+4];
    } while(numberPackets[pos]!='0');

    return std::make_pair( binToDec(binaryNumber), pos+5 );
}

auto decode(std::string& s) -> Node{
    Node node{};
    node.version = static_cast<unsigned int>(binToDec(s.substr(0,3)));
    node.type    = static_cast<unsigned int>(binToDec(s.substr(3,3)));
    if(node.type == 4){
        const auto [value, end] = parseNumberPackets(s);
        node.value = value;
        s = s.substr(end);
    }
    else{
        if(s[6]=='0'){
            const auto length = binToDec(s.substr(7,15));
            auto subPackets = s.substr(22,length);
            while(!subPackets.empty()){
                node.nodes.push_back(decode(subPackets));
            }
            s = s.substr(22+length);
        }
        else{ //'1'
            const auto numOfSubPackets = binToDec(s.substr(7,11));

            s = s.substr(18);
            for(auto i=0u; i<numOfSubPackets; i++){
                node.nodes.push_back(decode(s));
            }
        }
        node.value = evaluateNode(node.type, node.nodes);
    }
    return node;
}

int getVersionSum(const Node& root){
    auto sum=root.version;
    for(const auto node : root.nodes){
        sum+=getVersionSum(node);
    }
    return sum;
}

auto buildBITSTree = [](const auto& input){

    const std::unordered_map<char, std::string> hexToBin =
    {
        {'0', "0000"},
        {'1', "0001"},
        {'2', "0010"},
        {'3', "0011"},
        {'4', "0100"},
        {'5', "0101"},
        {'6', "0110"},
        {'7', "0111"},
        {'8', "1000"},
        {'9', "1001"},
        {'A', "1010"},
        {'B', "1011"},
        {'C', "1100"},
        {'D', "1101"},
        {'E', "1110"},
        {'F', "1111"}
    };
    std::string binaryInput{};
    binaryInput.reserve(input.size()*4);
    
    for(const auto c : input){
        binaryInput+=hexToBin.at(c);
    }
    return decode(binaryInput);
};


int main(){
    const auto root = buildBITSTree(readFile::string("input.txt"));

    //Task 1
    const auto sumOfVersions = getVersionSum(root);
    std::cout << "The sum of all versions is " << sumOfVersions << ".\n";

    //Task 2
    const auto evaluatedBITS = root.value;
    std::cout << "The BITS transmission evaluates to " << evaluatedBITS << ".\n";
}