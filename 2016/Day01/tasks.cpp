
#include "../../readFile.h"
#include "../../utilities.h"

#include <iostream>
#include <string>
#include <vector>
#include <complex>
#include <unordered_set>


struct complexHash{
    std::size_t operator()(const std::complex<int>& position) const noexcept{
        return (size_t) position.real() << 32 | position.imag();
    }
};

auto infNorm(const auto& z){
    return std::abs(z.real()) + std::abs(z.imag());
}

auto followOneInstruction = [](const auto& instruction, auto& facing, auto& currentPosition){
    using namespace std::complex_literals;

    const auto turnDirection = instruction[0];
    facing*= turnDirection=='L' ? 1i : -1i;

    const auto numberOfBlocksToWalk = std::stoi(instruction.substr(1));
    currentPosition += facing*numberOfBlocksToWalk;

    return facing;
};

auto distanceToDestination = [](const auto& instructions){
    using namespace std::complex_literals;

    std::complex<int> facing{1i};
    std::complex<int> currentPosition{0};
    for(const auto& instruction : instructions){
        followOneInstruction(instruction, facing, currentPosition);
    }
    return infNorm(currentPosition);
};


auto distanceToFirstPlaceVisitedTwice = [](const auto& instructions){
    using namespace std::complex_literals;
    std::unordered_set<std::complex<int>, complexHash> visitedPlaces{0};

    std::complex<int> facing{1i};
    std::complex<int> currentPosition{0}, nextPosition{0};
    for(const auto& instruction : instructions){
        nextPosition = currentPosition;
        facing = followOneInstruction(instruction, facing, nextPosition);
        while(currentPosition != nextPosition){
            currentPosition+=facing;
            auto inserted = visitedPlaces.insert(currentPosition).second;
            if(!inserted){
                return infNorm(currentPosition);
            }
        }
    }
    return 0;
};

int main(){
    const auto instructions = Utilities::split(readFile::string("input.txt"));

    //Task 1
    auto distance = distanceToDestination(instructions);
    std::cout << "The Easter Bunny HQ is " << distance << " blocks away.\n";

    //Task 2
    distance = distanceToFirstPlaceVisitedTwice(instructions);
    std::cout << "The Easter Bunny HQ is actually " << distance << " blocks away.\n";
}