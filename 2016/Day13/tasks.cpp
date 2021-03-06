
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <bit>
#include <unordered_map>
#include <unordered_set>
#include <limits.h>

using Position = Utilities::Position<unsigned int>;

class Mace{
    unsigned int maceDesignersFavoriteNumber{};
    std::unordered_map<Position, int> spaceMap{};

    int generate(const auto& pos) const{
        const auto& [x,y] = pos;
        auto n = x*x+3*x+2*x*y+y+y*y+maceDesignersFavoriteNumber;
        return std::popcount(n)%2==0 ? INT_MAX : -1;
    }

public:
    Mace(unsigned int input) : maceDesignersFavoriteNumber{input}{ }

    int getValueAt(const Position& pos) {
        if(!spaceMap.contains(pos)){
            spaceMap[pos] = generate(pos);
        }
        return spaceMap[pos];
    }

    bool isWall(const Position& pos) {
        return getValueAt(pos)==-1;
    }

    void setValueAt(const Position& pos, int val){
        spaceMap[pos] = val;
    }

    friend int findShortestPath(const Position& start, const Position& end);
};


class Spreader{
    Mace* macePtr{nullptr};
    std::unordered_set<Position> nextSpreadFrom{};
    static const std::array<Utilities::Position<int>, 4> directions;

    void spreadTo(const Position& currentPos, const auto& direction){
        if( (int)currentPos.x + direction.x < 0 || (int)currentPos.y + direction.y < 0 ) return;
        const auto nextPos = currentPos+direction;
        if(macePtr->isWall(nextPos)) return;
        const auto currentVal = macePtr->getValueAt(currentPos);
        if(macePtr->getValueAt(nextPos) > currentVal+1){
            macePtr->setValueAt(nextPos, currentVal+1);
            nextSpreadFrom.insert(nextPos);
        }
    }

public:
    Spreader(Mace* _macePtr) : macePtr{_macePtr}{}
    std::unordered_set<Position>& getNextSpreadFrom(){
        return nextSpreadFrom;
    }

    void operator()(const Position& pos){
        for(const auto& direction : directions){
            spreadTo(pos, direction);
        }
    }
};
const std::array<Utilities::Position<int>, 4> Spreader::directions
    = {std::make_pair(1,0),std::make_pair(-1,0),std::make_pair(0,1),std::make_pair(0,-1)};



int findShortestPath(Mace mace, const Position& start, const Position& end){
    mace.setValueAt(start, 0);
    std::unordered_set<Position> spreadFrom{start};
    while(!spreadFrom.contains(end)){
        Spreader spread{&mace};

        for(const auto& pos : spreadFrom){
            spread(pos);
        }
        std::swap(spreadFrom, spread.getNextSpreadFrom());
    }
    return mace.getValueAt(end);
}

auto getLocationsCountIn(Mace mace, const Position& start, int steps){
    mace.setValueAt(start, 0);

    std::unordered_set<Position> spreadFrom{start};
    auto locationsCount=spreadFrom.size();

    while(steps>0){
        Spreader spread{&mace};

        for(const auto& pos : spreadFrom){
            spread(pos);
        }
        std::swap(spreadFrom, spread.getNextSpreadFrom());
        locationsCount+=spreadFrom.size();
        steps--;
    }
    return locationsCount;
}


int main(){
    const auto input = 1364u;

    Mace mace{input};
    const Position start = std::make_pair(1u,1u);
    const Position end   = std::make_pair(31u,39u);

    const auto shortestPath = findShortestPath(mace, start, end);
    std::cout << "To reach the end there are at least " << shortestPath << " steps required.\n";

    const auto locationsCount = getLocationsCountIn(mace, start, 50);
    std::cout << "You can reach at most " << locationsCount << " different locations in 50 steps.\n";

    VerifySolution::verifySolution(shortestPath, locationsCount);
}