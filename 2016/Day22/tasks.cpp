
#include "../../lib/readFile.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <ranges>
#include <numeric>
#include <cmath>
#include <deque>

using Position = std::pair<int, int>;

Position operator+(const Position& lhs, const Position& rhs){
    return std::make_pair(lhs.first+rhs.first, lhs.second+rhs.second);
}

struct positionHash{
    std::size_t operator()(const Position& position) const noexcept{
        return (size_t) position.first << 32 | position.second;
    }
};

struct Node;
struct Node{
    Position pos;
    int size{0};
    int used{0};
    int avail{0};
    int usePerc{0};
    std::vector<Node*> neighbors{};
};


auto addNeighbors = [](auto& nodes){
    auto addNeighbor = [&nodes](const auto& pos, auto& node, int d_x, int d_y){
        const auto neighborPos = pos+Position{d_x, d_y};
        if(nodes.contains(neighborPos) && node.used <= nodes[neighborPos].size ) {
            //We only consider those nodes neighbors to whom we can move our data
            node.neighbors.push_back(&nodes[neighborPos]);
        }
    };
    for(auto& [pos, node] : nodes){
        addNeighbor(pos, node,  1,  0);
        addNeighbor(pos, node, -1,  0);
        addNeighbor(pos, node,  0,  1);
        addNeighbor(pos, node,  0, -1);
    }
};

auto parseInput = [](const auto& input){
    std::unordered_map<Position, Node, positionHash> nodes{};
    for(const auto& in : input | std::views::drop(2)){
        const auto split = Utilities::splitOnEach(in, " xy");

        const auto x       = std::stoi(split[1]);
        const auto y       = std::stoi(split[2]);
        const auto size    = std::stoi(split[3]);
        const auto used    = std::stoi(split[4]);
        const auto avail   = std::stoi(split[5]);
        const auto usePerc = std::stoi(split[6]);

        nodes.emplace(std::make_pair( Position{x,y} , Node{Position{x,y}, size, used, avail, usePerc} ));
    }
    addNeighbors(nodes);
    return nodes;
};


auto dataTransferable = [](const auto& senderNode, const auto& receiverNode){
    return senderNode.used <= receiverNode.avail && senderNode.used > 0;
};

auto getViableTransferCount = [](const auto& nodes){

    auto validNeighborTransferCount = [&nodes](const auto& pair){
        const auto& [senderPos, senderNode] = pair;
        return std::ranges::count_if(nodes,
            [&senderPos, &senderNode](const auto pair){
                const auto& [receiverPos, receiverNode] = pair;
                return senderPos != receiverPos && dataTransferable(senderNode, receiverNode);
            }
        );
    };

    return std::transform_reduce(std::begin(nodes), std::end(nodes), 0u, std::plus<>(), validNeighborTransferCount);
};

auto getFreeNodePos = [](const auto& nodes){
    auto freeNodeIt = std::ranges::find_if(std::begin(nodes), std::end(nodes),
        [](const auto& pair){return pair.second.used == 0;}
    );
    return freeNodeIt->first;
};

auto getShortestPath = [](const auto& nodes, const auto& startPos, const auto& endPos){
    std::unordered_map<Position, unsigned int, positionHash> shortestPathTo{};
    for(const auto& [pos, node] : nodes){
        shortestPathTo[pos] = UINT_MAX;
    }
    shortestPathTo[startPos] = 0;
    std::deque<Position> next{};
    next.push_back(startPos);

    while(shortestPathTo[endPos] == UINT_MAX){
        const auto currentPos = next.front();
        next.pop_front();
        for(auto& neighborPtr : nodes.at(currentPos).neighbors){
            if( shortestPathTo[neighborPtr->pos] > shortestPathTo[currentPos]+1 ){
                shortestPathTo[neighborPtr->pos] = shortestPathTo[currentPos]+1;
                next.push_back(neighborPtr->pos);
            }
        }
    }
    return shortestPathTo[endPos];
};

//This assumes two things:
//(1) Data is interchangable between all nodes that are not very large and very full
//(2) There is no very large, very full data node on the first two rows
//Then we can just move the goal data in a straight line to the left
//while using the second row to move data around
auto getFewestMoves = [](const auto& nodes){
    const auto freeNodePos = getFreeNodePos(nodes);
    const auto N = (int) std::sqrt(nodes.size());
    const auto movesNeededForFreeNode = getShortestPath(nodes, freeNodePos, std::make_pair(N-2, 0));
    return movesNeededForFreeNode + 1 + 5*(N-2);
};

int main(){
    const auto nodes = parseInput(readFile::vectorOfStrings("input.txt"));

    //Task 1
    const auto viableTransferCount = getViableTransferCount(nodes);
    std::cout << "There are " << viableTransferCount << " viable pairs to move data between.\n";

    //Task 2
    const auto leastMovesCount = getFewestMoves(nodes);
    std::cout << "To access the data, at least " << leastMovesCount << " moves are needed.\n";
}