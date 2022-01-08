
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>


using Point  = std::array<int,3>;

struct Cuboid{
    Point upperLeft {};
    Point lowerRight{};

    auto getVolume() const {
        const auto [x1,y1,z1] = upperLeft;
        const auto [x2,y2,z2] = lowerRight;
        uint64_t volume = (std::abs(x2-x1)+1);
        volume*=(std::abs(y2-y1)+1);
        volume*=(std::abs(z2-z1)+1);
        return volume;
    }

    auto hasVolume() const {
        const auto [x1,y1,z1] = upperLeft;
        const auto [x2,y2,z2] = lowerRight;
        return x1<=x2 && y1<=y2 && z1<=z2;
    }

};

auto narrowCuboid(Cuboid cuboid, const auto leftBound, const auto rightBound){
    auto& [a1,b1,c1] = cuboid.upperLeft;
    auto& [a2,b2,c2] = cuboid.lowerRight;

    a1 = std::ranges::clamp(a1, leftBound  , rightBound+1);
    b1 = std::ranges::clamp(b1, leftBound  , rightBound+1);
    c1 = std::ranges::clamp(c1, leftBound  , rightBound+1);
    a2 = std::ranges::clamp(a2, leftBound-1, rightBound  );
    b2 = std::ranges::clamp(b2, leftBound-1, rightBound  );
    c2 = std::ranges::clamp(c2, leftBound-1, rightBound  );

    return cuboid;
}

struct Step{
    Cuboid cuboid{};
    bool on{true};

    Step(Cuboid&& _cuboid, const std::string& _on) : cuboid{std::move(_cuboid)}, on{_on=="on"}{}
};

auto parseInput = [](const auto& input){
    std::vector<Step> steps;
    for(const auto& row : input){
        const auto split = Utilities::splitOnEach(row, " =.,");
        auto a = Point{std::stoi(split[2]), std::stoi(split[5]), std::stoi(split[8])};
        auto b = Point{std::stoi(split[3]), std::stoi(split[6]), std::stoi(split[9])};
        Cuboid cuboid{std::move(a), std::move(b)};
        steps.emplace_back( std::move(cuboid), split[0] );
    }
    return steps;
};

void add(std::vector<Cuboid>& activeCuboids, const int x1, const int y1, const int z1, const int x2, const int y2, const int z2) {
    if(x1<=x2 && y1<=y2 && z1<=z2){
        Point a{ x1, y1, z1 };
        Point b{ x2, y2, z2 };
        activeCuboids.emplace_back( std::move(a), std::move(b) );
    }
}

void particionActiveCuboid(std::vector<Cuboid>& activeCuboids, const Cuboid& cuboid, const Cuboid& activeCuboid){
    const auto& [a1,b1,c1] = cuboid.upperLeft;
    const auto& [a2,b2,c2] = cuboid.lowerRight;
    const auto& [x1,y1,z1] = activeCuboid.upperLeft;
    const auto& [x2,y2,z2] = activeCuboid.lowerRight;
    add(activeCuboids, x1,                 y1,                 z1,                 std::min(x2,a1-1), y2,                z2                );
    add(activeCuboids, std::max(x1, a1)  , y1,                 z1,                 std::min(x2,a2  ), std::min(y2,b1-1), z2                );
    add(activeCuboids, std::max(x1, a1)  , std::max(y1, b1  ), z1,                 std::min(x2,a2  ), std::min(y2,b2  ), std::min(z2,c1-1) );
    add(activeCuboids, std::max(x1, a1)  , std::max(y1, b1  ), std::max(z1, c2+1), std::min(x2,a2  ), std::min(y2,b2  ), z2                );
    add(activeCuboids, std::max(x1, a1)  , std::max(y1, b2+1), z1,                 std::min(x2,a2  ), y2,                z2                );
    add(activeCuboids, std::max(x1, a2+1), y1,                 z1,                 x2,                y2,                z2                );
}

void addCuboid(std::vector<Cuboid>& activeCuboids, const Cuboid& newCuboid, const bool on){
    std::vector<Cuboid> newCuboids{};
    for(auto activeCuboid : activeCuboids){
        particionActiveCuboid(newCuboids, newCuboid, activeCuboid);
    }
    if(on){
        newCuboids.emplace_back(newCuboid);
    }
    std::swap(activeCuboids, newCuboids);
}

auto getActiveCubesCountInSmallRegion = [](const auto& steps){
    std::vector<Cuboid> activeCuboids{};
    std::ranges::for_each(steps, [&activeCuboids](auto& step){
        const Cuboid cuboid = narrowCuboid(step.cuboid, -50, 50);
        if(cuboid.hasVolume()){
            addCuboid(activeCuboids, cuboid, step.on);
        }
    });
    return Utilities::sum(activeCuboids, 0ull, [](const auto& cuboid){return cuboid.getVolume();});
};

auto getActiveCubesCount = [](const auto& steps){
    std::vector<Cuboid> activeCuboids{};
    std::ranges::for_each(steps, [&activeCuboids](const auto& step){
        addCuboid(activeCuboids, step.cuboid, step.on);
    });
    return Utilities::sum(activeCuboids, 0ull, [](const auto& cuboid){return cuboid.getVolume();});
};

int main(){
    const auto steps = parseInput(readFile::vectorOfStrings("input.txt"));

    //Task 1
    const auto activeCubesCountInSmallRegion = getActiveCubesCountInSmallRegion(steps);
    std::cout << "Inside the small region, there are " << activeCubesCountInSmallRegion << " activated cubes.\n";

    //Task 2
    const auto activeCubesCount = getActiveCubesCount(steps);
    std::cout << "All in all, there are " << activeCubesCount << " activated cubes.\n";

    VerifySolution::verifySolution(activeCubesCountInSmallRegion, activeCubesCount);
}