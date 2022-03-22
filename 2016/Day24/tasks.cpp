
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <deque>
#include <unordered_map>
#include <numeric>

using Position = Utilities::Position<long int>;

auto getPlacesOfInterest(const auto& mace){
    std::unordered_map<int, Position> placesOfInterest;
    auto rowIdx = 0u;
    auto addPOIs = [&rowIdx, &placesOfInterest](const auto& row){
        const std::string pois = "0123456789";
        auto it = std::begin(row);
        while(true){
            it = std::ranges::find_first_of(it, std::end(row), std::begin(pois), std::end(pois));
            if( it==std::end(row) ) break;

            const auto colIdx = std::distance(std::begin(row), it);
            placesOfInterest[*it-'0']= Utilities::make_position(rowIdx, colIdx);
            it++;
        }
        rowIdx++;
    };

    std::ranges::for_each(mace, addPOIs);
    return placesOfInterest;
}

auto advance = [](const auto& currPos, const auto dx, const auto dy, auto& next, const auto& mace, auto& distances){
    const auto x = currPos.first+dx;
    const auto y = currPos.second+dy;
    const auto pos = std::make_pair(x,y);
    if(mace[x][y] != '#' && !distances.contains(pos)) {
        distances[pos] = distances[currPos]+1;
        next.push_back(pos);
    }
};

auto getDistancesFrom(const auto& start, const auto& mace){
    std::unordered_map<Position, int> distances{};
    distances[start]=0;
    std::deque<Position> next;
    next.push_back(start);
    while(!next.empty()){
        const auto pos = next.front();
        next.pop_front();
        advance(pos,  1,  0, next, mace, distances);
        advance(pos, -1,  0, next, mace, distances);
        advance(pos,  0,  1, next, mace, distances);
        advance(pos,  0, -1, next, mace, distances);
    }
    return distances;
}

auto addDistancesToMatrix = [](const auto& distances, const auto digit, const auto& placesOfInterest, auto& distanceMatrix){
    for(auto& [col, pos] : placesOfInterest){
        distanceMatrix[digit][col] = distances.at(pos);
    }
};

auto getdistanceMatrix(const auto& mace){
    const std::unordered_map<int, Position> placesOfInterest = getPlacesOfInterest(mace);
    const auto N =placesOfInterest.size();
    std::vector<std::vector<int>> distanceMatrix(N, std::vector(N, 0));

    for(const auto& [digit, start] : placesOfInterest){
        const auto distances = getDistancesFrom(start, mace);
        addDistancesToMatrix(distances, digit, placesOfInterest, distanceMatrix);
    }

    return distanceMatrix;
}

auto getShortestRoute = [](const auto& distanceMatrix, const bool returnToStart = false){
    auto shortestRouteLength = UINT_MAX;
    std::vector<int> route(distanceMatrix.size()-1, 0);
    std::iota(std::begin(route), std::end(route), 1);

    do{
        auto curPos = 0;
        auto routeLength = 0u;
        for(const auto nextPos : route){
            routeLength+=distanceMatrix[curPos][nextPos];
            curPos = nextPos;
        }
        if(returnToStart){
            routeLength+=distanceMatrix[curPos][0];
        }
        if(routeLength < shortestRouteLength){
            shortestRouteLength = routeLength;
        }
    }while(std::ranges::next_permutation(route).found);

    return shortestRouteLength;
};

int main(){
    const auto mace = readFile::vectorOfStrings("input.txt");
    const auto distanceMatrix = getdistanceMatrix(mace);

    //Task 1
    const auto fewestSteps = getShortestRoute(distanceMatrix);
    std::cout << "The shortest route for the robot takes " << fewestSteps << " steps.\n";

    //Task 2
    const auto fewestStepsWithBacktrack = getShortestRoute(distanceMatrix, true);
    std::cout << "The shortest round trip for the robot takes " << fewestStepsWithBacktrack << " steps.\n";

    VerifySolution::verifySolution(fewestSteps, fewestStepsWithBacktrack);
}