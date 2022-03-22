
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/matrix.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <deque>
#include <unordered_map>
#include <numeric>
#include <ranges>

using Position = Utilities::Position<long int>;

auto getPlacesOfInterest(const auto& mace){
    std::unordered_map<int, Position> placesOfInterest;

    for(auto it = std::begin(mace); it!=std::end(mace); ++it){
        if(std::isdigit(*it)){
            const auto longIndex = std::distance(mace.begin(), it);
            const long int m = mace.cols();
            placesOfInterest[*it-'0']= Utilities::make_position(longIndex/m, longIndex%m);

        }
    }
    return placesOfInterest;
}

auto advance = [](const auto& currPos, const auto dx, const auto dy, auto& next, const auto& mace, auto& distances){
    const auto pos = currPos+Utilities::make_position(dx, dy);
    if(mace(currPos) != '#' && !distances.contains(pos)) {
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

auto parseInput(const auto& input){
    const auto n = input.size();
    const auto m = input[0].size();
    return Matrix::Matrix<char>{n, m, input | std::views::join};
}

int main(){
    const auto mace = parseInput(readFile::vectorOfStrings("input.txt"));
    const auto distanceMatrix = getdistanceMatrix(mace);

    //Task 1
    const auto fewestSteps = getShortestRoute(distanceMatrix);
    std::cout << "The shortest route for the robot takes " << fewestSteps << " steps.\n";

    //Task 2
    const auto fewestStepsWithBacktrack = getShortestRoute(distanceMatrix, true);
    std::cout << "The shortest round trip for the robot takes " << fewestStepsWithBacktrack << " steps.\n";

    VerifySolution::verifySolution(fewestSteps, fewestStepsWithBacktrack);
}