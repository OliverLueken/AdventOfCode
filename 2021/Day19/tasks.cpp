
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <vector>
#include <array>
#include <unordered_set>
#include <algorithm>
#include <ranges>

using Position    = Utilities::Position<int>;
using Coordinate  = std::array<int,3>;
using Coordinates = std::vector<Coordinate>;

class nextRotation_{
    static int rotationAxis;
    static int faceRotation;
    static int rotationCounts;

    auto rotateAroundRotationAxis(Coordinate& coord) const{
        const auto first  = (4-rotationAxis)%3;
        const auto second = (3-rotationAxis)%3;
        coord[first] = -std::exchange(coord[second], coord[first]);
    }
public:
    bool operator()(Coordinates& coordinates) {
        if(rotationCounts==23){
            rotationAxis   = 0;
            faceRotation   = 0;
            rotationCounts = 0;
            return false;
        }
        for(auto& coord : coordinates){
            rotateAroundRotationAxis(coord);
        }
        faceRotation++;
        if(faceRotation>=3) rotationAxis=(rotationAxis+1)%3;
        if(faceRotation==4) faceRotation=0;
        rotationCounts++;
        return true;
    }
};
inline nextRotation_ next_rotation;
int nextRotation_::rotationAxis   = 0;
int nextRotation_::faceRotation   = 0;
int nextRotation_::rotationCounts = 0;

auto operator+(const Coordinate& lhs, const Coordinate& rhs){
    return Coordinate{ lhs[0]+rhs[0],lhs[1]+rhs[1],lhs[2]+rhs[2] };
}

auto operator-(const Coordinate& lhs, const Coordinate& rhs){
    return Coordinate{ lhs[0]-rhs[0],lhs[1]-rhs[1],lhs[2]-rhs[2] };
}

template<>
struct std::hash<Coordinate>{
    std::size_t operator()(const Coordinate& z) const noexcept{
        const std::size_t h1 = std::hash<int>{}(z[0]);
        const std::size_t h2 = std::hash<int>{}(z[1]);
        const std::size_t h3 = std::hash<int>{}(z[2]);
        return h1^(h2<<20)^(h3<<40);
    }
};

auto dist(const Coordinate y, const Coordinate x) -> unsigned int{
    return std::abs(x[0]-y[0])+std::abs(x[1]-y[1])+std::abs(x[2]-y[2]);
}


auto parseInput = [](const auto& input){
    Coordinates coordinates{};
    std::vector<Coordinates> scanners{};
    for(const auto& row : input | std::views::drop(1)){
        if(row[1]=='-'){
            scanners.emplace_back(std::move(coordinates));
            coordinates.clear();
        }
        else{
            const auto split = Utilities::split(row, ',');
            Coordinate  s{};
            s[0] = std::stoi(split[0]);
            s[1] = std::stoi(split[1]);
            s[2] = std::stoi(split[2]);
            coordinates.emplace_back(std::move(s));
        }
    }
    scanners.emplace_back(std::move(coordinates));
    return scanners;
};


auto testCoordinatesOverlapWithBeacons(const auto& coordinates, const auto& beaconCoordinates){
    for(const auto& beacon : beaconCoordinates){
        for(const auto& coord1 : coordinates){
            const auto count = std::ranges::count_if(coordinates,
                [&beaconCoordinates, &coord1, &beacon](const auto& coord){
                    return beaconCoordinates.contains(beacon-coord1+coord);
                }
            );

            if(count>=12){
                return std::make_tuple(true, beacon, coord1);
            }
        }
    }
    return std::make_tuple(false, coordinates[0], coordinates[0]);
}


auto findNewScanner(auto& scanners, auto& beaconCoordinates, auto& scannerCoordinates){
    for(auto& coordinates : scanners ){
        do{
            const auto [testSuccessful, beacon, coord1] = testCoordinatesOverlapWithBeacons(coordinates, beaconCoordinates);
            if(testSuccessful){
                scannerCoordinates.emplace_back(beacon-coord1);
                for(const auto& coord : coordinates){
                    beaconCoordinates.emplace(beacon-coord1+coord);
                }
                coordinates.clear();
                break;
            }
        }while( next_rotation(coordinates) );
    }
}

auto getResult = [](auto& scanners){
    Coordinates scannerCoordinates{Coordinate{0,0,0}};
    std::unordered_set<Coordinate> beaconCoordinates{};
    for(const auto& coord : scanners[0]){
        beaconCoordinates.emplace(std::move(coord));
    }
    scanners[0].clear();

    while(scanners.size()>0){
        std::erase_if(scanners, [](const auto& coordinates){return coordinates.size()==0;});
        findNewScanner(scanners, beaconCoordinates, scannerCoordinates);
    }

    return std::make_pair(beaconCoordinates, scannerCoordinates);
};


auto getMaxScannerDistance(const auto& scannerCoordinates){
    auto max = 0u;
    for(auto i=0u; i<scannerCoordinates.size(); i++){
        for(auto j=i+1; j<scannerCoordinates.size(); j++){
            max=std::max(max, dist(scannerCoordinates[i], scannerCoordinates[j]) );
        }
    }
    return max;
};


int main(){
    auto scanners = parseInput(readFile::vectorOfStrings("input.txt"));

    //Task 1
    const auto [beaconCoordinates, scannerCoordinates] = getResult(scanners);
    const auto beaconCount = beaconCoordinates.size();
    std::cout << "There are " << beaconCount << " different beacons in the ocean.\n";

    //Task 2
    const auto maxScannerDistance = getMaxScannerDistance(scannerCoordinates);
    std::cout << "The maximum manhatten distance between two scanners is " << maxScannerDistance << ".\n";

    VerifySolution::verifySolution(beaconCount, maxScannerDistance);
}